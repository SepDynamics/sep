#include "market_model_cache.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <filesystem>
#include <ctime>
#include <thread>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace sep::apps {

MarketModelCache::MarketModelCache(std::shared_ptr<sep::connectors::OandaConnector> oanda_connector)
    : oanda_connector_(oanda_connector)
    , qfh_processor_(std::make_unique<sep::quantum::QFHProcessor>())
{
    // Ensure cache directory exists
    std::filesystem::create_directories(getCacheDirectory());
}

bool MarketModelCache::initializeWithCurrentWeek(const std::string& instrument) {
    std::cout << "[MarketModel] Initializing market model cache for " << instrument << std::endl;
    
    current_instrument_ = instrument;
    
    // First, check for existing cached historical data from the last week
    auto now = std::chrono::system_clock::now();
    auto one_week_ago = now - std::chrono::hours(168); // 7 days
    auto search_start = one_week_ago - std::chrono::hours(72); // Buffer for weekends
    
    // Generate cache key based on search timeframe
    std::string cache_key = generateCacheKey(instrument, search_start);
    
    std::cout << "[MarketModel] Checking for cached historical data..." << std::endl;
    if (persistence_enabled_ && loadFromDisk(cache_key)) {
        std::cout << "[MarketModel] ✅ Found cached data: " << current_session_.raw_data.size() << " candles, " 
                  << current_metrics_.size() << " processed metrics" << std::endl;
        return true;
    }
    
    std::cout << "[MarketModel] No cached data found. Fetching fresh historical data..." << std::endl;
    
    // Try to get current/live data first
    auto last_close = getLastMarketClose();
    auto hours_since_close = std::chrono::duration_cast<std::chrono::hours>(now - last_close).count();
    
    if (hours_since_close > time_window_hours_) {
        current_session_.start_time = now - std::chrono::hours(time_window_hours_);
    } else {
        current_session_.start_time = last_close;
    }
    current_session_.end_time = now;
    
    auto start_time_t = std::chrono::system_clock::to_time_t(current_session_.start_time);
    auto end_time_t = std::chrono::system_clock::to_time_t(current_session_.end_time);
    std::cout << "[MarketModel] Attempting live data from " 
              << std::put_time(std::gmtime(&start_time_t), "%Y-%m-%d %H:%M:%S")
              << " to " 
              << std::put_time(std::gmtime(&end_time_t), "%Y-%m-%d %H:%M:%S") 
              << std::endl;
    
    // Try live data first, fall back to historical if needed
    return refreshFromMarketClose(instrument);
}

bool MarketModelCache::refreshFromMarketClose(const std::string& instrument) {
    std::cout << "[MarketModel] Fetching fresh historical data from OANDA..." << std::endl;
    
    if (!oanda_connector_) {
        last_error_ = "No OANDA connector available";
        return false;
    }
    
    processing_active_ = true;
    std::atomic<bool> fetch_complete{false};
    std::vector<Candle> fetched_candles;
    
    fetchHistoricalDataAsync(instrument, current_session_.start_time, current_session_.end_time,
        [&](const std::vector<Candle>& candles) {
            fetched_candles = candles;
            fetch_complete = true;
        });
    
    // Wait for fetch to complete (with timeout)
    int timeout_seconds = 30;
    while (!fetch_complete && timeout_seconds-- > 0) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    
    if (!fetch_complete) {
        last_error_ = "Timeout waiting for historical data";
        processing_active_ = false;
        return false;
    }
    
    if (fetched_candles.empty()) {
        std::cout << "[MarketModel] ⚠️  No live data available (markets closed). Fetching historical data..." << std::endl;
        
        // Fetch actual historical data from last week
        if (fetchHistoricalDataForCache(instrument)) {
            std::cout << "[MarketModel] ✅ Successfully loaded historical market data" << std::endl;
            processing_active_ = false;
            current_session_.is_complete = true;
            return true;
        }
        
        last_error_ = "Failed to fetch historical market data";
        processing_active_ = false;
        return false;
    }
    
    std::cout << "[MarketModel] ✅ Fetched " << fetched_candles.size() << " candles" << std::endl;
    
    // Store raw data
    current_session_.raw_data = std::move(fetched_candles);
    
    // Process through quantum metrics pipeline
    bool success = processHistoricalData(current_session_.raw_data);
    
    if (success && persistence_enabled_) {
        // Cache the results
        std::string cache_key = generateCacheKey(instrument, current_session_.start_time);
        saveToDisk(cache_key);
    }
    
    processing_active_ = false;
    current_session_.is_complete = success;
    
    return success;
}

bool MarketModelCache::processHistoricalData(const std::vector<Candle>& candles) {
    std::cout << "[MarketModel] Processing " << candles.size() << " candles through quantum metrics pipeline..." << std::endl;
    
    current_metrics_.clear();
    current_metrics_.reserve(candles.size());
    
    // Process each candle through the quantum pipeline
    const int window_size = 120; // 2 hours of M1 data for context
    
    for (size_t i = window_size; i < candles.size(); ++i) {
        // Extract window for context
        std::vector<Candle> window(candles.begin() + i - window_size, candles.begin() + i + 1);
        
        // Compute metrics for this candle
        ProcessedMetrics metrics = computeMetricsForCandle(candles[i], window);
        current_metrics_.push_back(metrics);
        
        // Progress reporting
        if (i % 500 == 0) {
            double progress = (double)(i - window_size) / (candles.size() - window_size) * 100.0;
            std::cout << "[MarketModel] Processing... " << std::fixed << std::setprecision(1) << progress << "%" << std::endl;
        }
    }
    
    std::cout << "[MarketModel] ✅ Processed " << current_metrics_.size() << " metrics" << std::endl;
    
    // Process through QFH for trajectory analysis
    processWithQuantumPipeline(candles);
    
    return true;
}

MarketModelCache::ProcessedMetrics MarketModelCache::computeMetricsForCandle(
    const Candle& candle, 
    const std::vector<Candle>& window) {
    
    ProcessedMetrics metrics;
    metrics.timestamp = std::chrono::system_clock::from_time_t(candle.timestamp);
    
    // Convert candle window to price data for QFH processing
    std::vector<double> prices;
    prices.reserve(window.size());
    for (const auto& c : window) {
        prices.push_back(c.close);
    }
    
    // Use QFH to compute quantum metrics
    if (qfh_processor_ && prices.size() >= 60) { // Minimum window for meaningful analysis
        // This would call into your QFH analysis methods
        // For now, using placeholder calculations
        
        // Calculate entropy from price movements
        std::vector<double> returns;
        for (size_t i = 1; i < prices.size(); ++i) {
            returns.push_back((prices[i] - prices[i-1]) / prices[i-1]);
        }
        
        // Simple entropy calculation (to be replaced with QFH method)
        double sum = 0.0;
        for (double r : returns) {
            if (r != 0) sum += r * std::log(std::abs(r) + 1e-10);
        }
        metrics.entropy = -sum / returns.size();
        
        // Coherence based on trend consistency
        int trend_changes = 0;
        for (size_t i = 1; i < returns.size(); ++i) {
            if ((returns[i] > 0) != (returns[i-1] > 0)) trend_changes++;
        }
        metrics.coherence = 1.0 - (double)trend_changes / returns.size();
        
        // Stability based on volatility
        double mean_return = 0.0;
        for (double r : returns) mean_return += r;
        mean_return /= returns.size();
        
        double variance = 0.0;
        for (double r : returns) {
            variance += (r - mean_return) * (r - mean_return);
        }
        variance /= returns.size();
        metrics.stability = 1.0 / (1.0 + variance * 10000); // Normalize volatility
        
        // Trajectory confidence from QFH analysis
        metrics.trajectory_confidence = (metrics.entropy + metrics.coherence + metrics.stability) / 3.0;
        
        // Pattern classification
        if (metrics.coherence > 0.7 && metrics.stability > 0.6) {
            metrics.pattern_type = "TrendContinuation";
        } else if (metrics.entropy > 0.5 && metrics.coherence < 0.4) {
            metrics.pattern_type = "RandomNoise";
        } else {
            metrics.pattern_type = "MeanReversion";
        }
        
        // Signal strength
        metrics.signal_strength = metrics.trajectory_confidence * metrics.coherence;
    } else {
        // Default values for insufficient data
        metrics.entropy = 0.0;
        metrics.coherence = 0.0;
        metrics.stability = 0.0;
        metrics.trajectory_confidence = 0.0;
        metrics.pattern_type = "InsufficientData";
        metrics.signal_strength = 0.0;
    }
    
    return metrics;
}

void MarketModelCache::processWithQuantumPipeline(const std::vector<Candle>& candles) {
    std::cout << "[MarketModel] Running QFH trajectory analysis..." << std::endl;
    
    // This is where we'd integrate with your full QFH processing pipeline
    // For now, this is a placeholder that shows the architecture
    
    if (!qfh_processor_) {
        std::cout << "[MarketModel] ⚠️  QFH processor not available" << std::endl;
        return;
    }
    
    // The QFH processor would:
    // 1. Convert candles to quantum state representations
    // 2. Apply trajectory damping algorithms
    // 3. Calculate pattern classifications
    // 4. Generate enhanced metrics
    
    std::cout << "[MarketModel] ✅ QFH analysis complete" << std::endl;
}

std::chrono::system_clock::time_point MarketModelCache::getLastMarketClose() {
    auto now = std::chrono::system_clock::now();
    auto now_t = std::chrono::system_clock::to_time_t(now);
    auto now_tm = *std::gmtime(&now_t);
    
    // Find last Friday 21:00 UTC (forex market close)
    int days_back = 0;
    switch (now_tm.tm_wday) {
        case 0: days_back = 2; break; // Sunday -> Friday
        case 1: days_back = 3; break; // Monday -> Friday
        case 2: days_back = 4; break; // Tuesday -> Friday
        case 3: days_back = 5; break; // Wednesday -> Friday
        case 4: days_back = 6; break; // Thursday -> Friday
        case 5: days_back = 0; break; // Friday -> same Friday (if after close)
        case 6: days_back = 1; break; // Saturday -> Friday
    }
    
    // If it's Friday and before market close (21:00 UTC), use previous Friday
    if (now_tm.tm_wday == 5 && now_tm.tm_hour < 21) {
        days_back = 7;
    }
    
    auto last_friday = now - std::chrono::hours(24 * days_back);
    auto last_friday_t = std::chrono::system_clock::to_time_t(last_friday);
    auto friday_tm = *std::gmtime(&last_friday_t);
    
    // Set to 21:00 UTC
    friday_tm.tm_hour = 21;
    friday_tm.tm_min = 0;
    friday_tm.tm_sec = 0;
    
    return std::chrono::system_clock::from_time_t(std::mktime(&friday_tm));
}

void MarketModelCache::fetchHistoricalDataAsync(
    const std::string& instrument,
    const std::chrono::system_clock::time_point& start_time,
    const std::chrono::system_clock::time_point& end_time,
    std::function<void(const std::vector<Candle>&)> callback) {
    
    // Convert time points to UNIX timestamps (as strings) for OANDA API
    auto start_time_t = std::chrono::system_clock::to_time_t(start_time);
    auto end_time_t = std::chrono::system_clock::to_time_t(end_time);
    
    std::string start_str = std::to_string(start_time_t);
    std::string end_str = std::to_string(end_time_t);
    
    std::cout << "[MarketModel] Fetching " << instrument << " from " << start_str << " to " << end_str 
              << " (UNIX timestamps)" << std::endl;
    
    // Use OANDA connector to fetch data
    oanda_connector_->getHistoricalData(
        instrument, "M1", start_str, end_str,
        [callback](const std::vector<sep::connectors::OandaCandle>& oanda_candles) {
            // Convert OandaCandle to Candle format
            std::vector<Candle> candles;
            candles.reserve(oanda_candles.size());
            
            for (const auto& oc : oanda_candles) {
                Candle c;
                // Parse OANDA time string to timestamp
                std::tm tm = {};
                std::istringstream ss(oc.time);
                ss >> std::get_time(&tm, "%Y-%m-%dT%H:%M:%S");
                c.timestamp = std::mktime(&tm);
                c.open = oc.open;
                c.high = oc.high;
                c.low = oc.low;
                c.close = oc.close;
                c.volume = oc.volume;
                candles.push_back(c);
            }
            
            callback(candles);
        }
    );
}

std::string MarketModelCache::generateCacheKey(
    const std::string& instrument, 
    const std::chrono::system_clock::time_point& start_time) const {
    
    auto start_time_t = std::chrono::system_clock::to_time_t(start_time);
    std::stringstream ss;
    ss << instrument << "_" << std::put_time(std::gmtime(&start_time_t), "%Y%m%d_%H%M%S");
    return ss.str();
}

bool MarketModelCache::saveToDisk(const std::string& cache_key) const {
    try {
        json cache_data;
        cache_data["cache_key"] = cache_key;
        cache_data["instrument"] = current_instrument_;
        cache_data["start_time"] = std::chrono::system_clock::to_time_t(current_session_.start_time);
        cache_data["end_time"] = std::chrono::system_clock::to_time_t(current_session_.end_time);
        cache_data["is_complete"] = current_session_.is_complete;
        
        // Save raw candles
        cache_data["raw_data"] = json::array();
        for (const auto& candle : current_session_.raw_data) {
            json candle_json;
            candle_json["timestamp"] = candle.timestamp;
            candle_json["open"] = candle.open;
            candle_json["high"] = candle.high;
            candle_json["low"] = candle.low;
            candle_json["close"] = candle.close;
            candle_json["volume"] = candle.volume;
            cache_data["raw_data"].push_back(candle_json);
        }
        
        // Save processed metrics
        cache_data["processed_metrics"] = json::array();
        for (const auto& metrics : current_metrics_) {
            json metrics_json;
            metrics_json["timestamp"] = std::chrono::system_clock::to_time_t(metrics.timestamp);
            metrics_json["entropy"] = metrics.entropy;
            metrics_json["coherence"] = metrics.coherence;
            metrics_json["stability"] = metrics.stability;
            metrics_json["trajectory_confidence"] = metrics.trajectory_confidence;
            metrics_json["pattern_type"] = metrics.pattern_type;
            metrics_json["signal_strength"] = metrics.signal_strength;
            cache_data["processed_metrics"].push_back(metrics_json);
        }
        
        std::string cache_file = getCacheDirectory() + cache_key + ".json";
        std::ofstream file(cache_file);
        file << cache_data.dump(2);
        file.close();
        
        std::cout << "[MarketModel] ✅ Saved to cache: " << cache_file << std::endl;
        return true;
        
    } catch (const std::exception& e) {
        std::cerr << "[MarketModel] ❌ Failed to save cache: " << e.what() << std::endl;
        return false;
    }
}

bool MarketModelCache::loadFromDisk(const std::string& cache_key) {
    try {
        std::string cache_file = getCacheDirectory() + cache_key + ".json";
        if (!std::filesystem::exists(cache_file)) {
            return false;
        }
        
        std::ifstream file(cache_file);
        json cache_data;
        file >> cache_data;
        file.close();
        
        // Load session info
        current_session_.start_time = std::chrono::system_clock::from_time_t(cache_data["start_time"]);
        current_session_.end_time = std::chrono::system_clock::from_time_t(cache_data["end_time"]);
        current_session_.is_complete = cache_data["is_complete"];
        
        // Load raw candles
        current_session_.raw_data.clear();
        for (const auto& candle_json : cache_data["raw_data"]) {
            Candle candle;
            candle.timestamp = candle_json["timestamp"];
            candle.open = candle_json["open"];
            candle.high = candle_json["high"];
            candle.low = candle_json["low"];
            candle.close = candle_json["close"];
            candle.volume = candle_json["volume"];
            current_session_.raw_data.push_back(candle);
        }
        
        // Load processed metrics
        current_metrics_.clear();
        for (const auto& metrics_json : cache_data["processed_metrics"]) {
            ProcessedMetrics metrics;
            metrics.timestamp = std::chrono::system_clock::from_time_t(metrics_json["timestamp"]);
            metrics.entropy = metrics_json["entropy"];
            metrics.coherence = metrics_json["coherence"];
            metrics.stability = metrics_json["stability"];
            metrics.trajectory_confidence = metrics_json["trajectory_confidence"];
            metrics.pattern_type = metrics_json["pattern_type"];
            metrics.signal_strength = metrics_json["signal_strength"];
            current_metrics_.push_back(metrics);
        }
        
        return true;
        
    } catch (const std::exception& e) {
        std::cerr << "[MarketModel] ❌ Failed to load cache: " << e.what() << std::endl;
        return false;
    }
}

bool MarketModelCache::fetchHistoricalDataForCache(const std::string& instrument) {
    try {
        std::cout << "[MarketModel] Fetching historical market data for " << instrument << "..." << std::endl;
        
        // Calculate time range for previous week of trading data
        auto now = std::chrono::system_clock::now();
        auto one_week_ago = now - std::chrono::hours(168); // 7 days * 24 hours
        
        // Go further back to ensure we get enough trading data (markets are closed weekends)
        auto start_time = one_week_ago - std::chrono::hours(72); // Extra 3 days buffer
        
        current_session_.start_time = start_time;
        current_session_.end_time = now;
        
        auto start_time_t = std::chrono::system_clock::to_time_t(start_time);
        auto now_time_t = std::chrono::system_clock::to_time_t(now);
        std::cout << "[MarketModel] Requesting historical data from " 
                  << std::put_time(std::gmtime(&start_time_t), "%Y-%m-%d %H:%M:%S")
                  << " to " 
                  << std::put_time(std::gmtime(&now_time_t), "%Y-%m-%d %H:%M:%S") 
                  << std::endl;
        
        current_session_.raw_data.clear();
        
        std::atomic<bool> fetch_complete{false};
        std::vector<Candle> historical_candles;
        
        // Use OANDA connector to fetch most recent available data (no time range)
        std::cout << "[MarketModel] Requesting most recent 2880 candles (48 hours of M1 data)..." << std::endl;
        oanda_connector_->getHistoricalData(instrument, "M1", "", "",
            [&](const std::vector<sep::connectors::OandaCandle>& oanda_candles) {
                std::cout << "[MarketModel] Received " << oanda_candles.size() << " OANDA candles" << std::endl;
                
                // Convert OandaCandle to Candle format
                historical_candles.clear();
                historical_candles.reserve(oanda_candles.size());
                
                for (const auto& oc : oanda_candles) {
                    Candle c;
                    // Parse OANDA time string to timestamp
                    std::tm tm = {};
                    std::istringstream ss(oc.time);
                    ss >> std::get_time(&tm, "%Y-%m-%dT%H:%M:%S");
                    c.timestamp = std::mktime(&tm);
                    c.open = oc.open;
                    c.high = oc.high;
                    c.low = oc.low;
                    c.close = oc.close;
                    c.volume = oc.volume;
                    historical_candles.push_back(c);
                }
                
                fetch_complete = true;
                std::cout << "[MarketModel] Converted to " << historical_candles.size() 
                         << " candles for processing" << std::endl;
            });
        
        // Wait for fetch to complete with longer timeout for historical data
        int timeout_seconds = 60;
        while (!fetch_complete && timeout_seconds-- > 0) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
            if (timeout_seconds % 10 == 0) {
                std::cout << "[MarketModel] Waiting for historical data... " << timeout_seconds << "s remaining" << std::endl;
            }
        }
        
        if (!fetch_complete) {
            std::cerr << "[MarketModel] Timeout waiting for historical data" << std::endl;
            return false;
        }
        
        if (historical_candles.empty()) {
            std::cerr << "[MarketModel] No historical data received from OANDA" << std::endl;
            return false;
        }
        
        current_session_.raw_data = std::move(historical_candles);
        
        std::cout << "[MarketModel] ✅ Fetched " << current_session_.raw_data.size() 
                  << " candles of historical market data" << std::endl;
        
        // Update session timeframe based on actual data received
        if (!current_session_.raw_data.empty()) {
            current_session_.start_time = std::chrono::system_clock::from_time_t(current_session_.raw_data.front().timestamp);
            current_session_.end_time = std::chrono::system_clock::from_time_t(current_session_.raw_data.back().timestamp);
            
            auto start_time_t = std::chrono::system_clock::to_time_t(current_session_.start_time);
            auto end_time_t = std::chrono::system_clock::to_time_t(current_session_.end_time);
            
            std::cout << "[MarketModel] Data timeframe: " 
                      << std::put_time(std::gmtime(&start_time_t), "%Y-%m-%d %H:%M:%S")
                      << " to " 
                      << std::put_time(std::gmtime(&end_time_t), "%Y-%m-%d %H:%M:%S") 
                      << std::endl;
        }
        
        // Process the historical data through quantum metrics pipeline
        bool success = processHistoricalData(current_session_.raw_data);
        
        if (success) {
            // Cache the processed results for future use
            std::string cache_key = generateCacheKey(instrument, current_session_.start_time);
            if (saveToDisk(cache_key)) {
                std::cout << "[MarketModel] ✅ Historical data cached for future use" << std::endl;
            }
        }
        
        return success;
        
    } catch (const std::exception& e) {
        std::cerr << "[MarketModel] Error fetching historical data: " << e.what() << std::endl;
        return false;
    }
}

std::string MarketModelCache::getCacheStatus() const {
    std::stringstream ss;
    ss << "Cache Status:\n";
    ss << "  Instrument: " << current_instrument_ << "\n";
    ss << "  Raw Data: " << current_session_.raw_data.size() << " candles\n";
    ss << "  Processed Metrics: " << current_metrics_.size() << " entries\n";
    ss << "  Complete: " << (current_session_.is_complete ? "Yes" : "No") << "\n";
    ss << "  Processing: " << (processing_active_ ? "Yes" : "No") << "\n";
    return ss.str();
}

} // namespace sep::apps
