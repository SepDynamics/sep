#include "enhanced_market_model_cache.hpp"
#include <iostream>
#include <fstream>
#include <filesystem>
#include <algorithm>
#include <numeric>
#include <cmath>
#include <thread>
#include <nlohmann/json.hpp>

namespace sep::cache {

EnhancedMarketModelCache::EnhancedMarketModelCache(
    std::shared_ptr<sep::connectors::OandaConnector> connector,
    std::shared_ptr<sep::apps::MarketModelCache> base_cache)
    : oanda_connector_(connector), base_cache_(base_cache) {
    std::filesystem::create_directories(cache_directory_);
}

bool EnhancedMarketModelCache::ensureEnhancedCacheForInstrument(const std::string& instrument, TimeFrame timeframe) {
    std::string cache_key = getCacheKey(instrument, timeframe);
    std::string cache_path = getCacheFilepath(cache_key);
    
    // Check if we have a valid cache entry
    auto cache_it = cache_entries_.find(cache_key);
    if (cache_it != cache_entries_.end()) {
        auto time_since_update = std::chrono::system_clock::now() - cache_it->second.last_updated;
        if (time_since_update < CACHE_VALIDITY_PERIOD) {
            std::cout << "[ENHANCED_CACHE] ✅ Using valid cache for " << instrument << std::endl;
            cache_it->second.access_count++;
            return true;
        }
    }
    
    // Load from disk if exists
    if (std::filesystem::exists(cache_path)) {
        std::cout << "[ENHANCED_CACHE] 📂 Loading cache from disk for " << instrument << std::endl;
        if (loadEnhancedCache(cache_path)) {
            return true;
        }
    }
    
    std::cout << "[ENHANCED_CACHE] 🔄 Building enhanced cache for " << instrument << std::endl;
    
    // Fetch primary asset data
    std::vector<Candle> primary_candles;
    if (!fetchAssetData(instrument, primary_candles)) {
        std::cerr << "[ENHANCED_CACHE] ❌ Failed to fetch data for " << instrument << std::endl;
        return false;
    }
    
    // Calculate cross-asset correlations
    CrossAssetCorrelation correlation_data = calculateCrossAssetCorrelation(instrument, EUR_USD_CORRELATED_PAIRS);
    
    // Generate enhanced signals using correlation data
    CacheEntry new_entry;
    new_entry.instrument = instrument;
    new_entry.timeframe = timeframe;
    new_entry.correlation_data = correlation_data;
    new_entry.last_updated = std::chrono::system_clock::now();
    
    // Process signals with correlation enhancement
    for (size_t i = 1; i < primary_candles.size(); ++i) {
        const auto& candle = primary_candles[i];
        
        // Generate base quantum signal (simplified for now)
        sep::trading::QuantumTradingSignal base_signal;
        double price_change = (candle.close - primary_candles[i-1].close) / primary_candles[i-1].close;
        
        if (std::abs(price_change) > 0.0001) {
            base_signal.action = price_change > 0 ? 
                sep::trading::QuantumTradingSignal::BUY : sep::trading::QuantumTradingSignal::SELL;
            base_signal.identifiers.confidence = static_cast<float>(std::min(std::abs(price_change) * 5000, 0.95));
            base_signal.identifiers.coherence = 0.5f;
            base_signal.identifiers.stability = 0.6f;
            
            // Create enhanced signal with correlation boost
            ProcessedSignal enhanced_signal;
            enhanced_signal.quantum_signal = base_signal;
            enhanced_signal.correlation_boost = correlation_data.correlation_strength * 0.2; // 20% max boost
            enhanced_signal.contributing_assets = correlation_data.correlated_pairs;
            enhanced_signal.timestamp = std::chrono::system_clock::now();
            
            // Apply correlation boost to confidence
            enhanced_signal.quantum_signal.identifiers.confidence = std::min(
                enhanced_signal.quantum_signal.identifiers.confidence * (1.0f + static_cast<float>(enhanced_signal.correlation_boost)),
                0.99f
            );
            
            new_entry.signals.push_back(enhanced_signal);
        }
    }
    
    cache_entries_[cache_key] = new_entry;
    saveEnhancedCache(cache_path);
    
    std::cout << "[ENHANCED_CACHE] ✅ Generated " << new_entry.signals.size() 
              << " correlation-enhanced signals for " << instrument << std::endl;
    
    return true;
}

EnhancedMarketModelCache::CrossAssetCorrelation EnhancedMarketModelCache::calculateCrossAssetCorrelation(
    const std::string& primary_asset, 
    const std::vector<std::string>& correlated_assets) {
    
    EnhancedMarketModelCache::CrossAssetCorrelation correlation;
    correlation.primary_pair = primary_asset;
    correlation.last_updated = std::chrono::system_clock::now();
    
    // Fetch primary asset data
    std::vector<Candle> primary_candles;
    if (!fetchAssetData(primary_asset, primary_candles)) {
        correlation.correlation_strength = 0.0;
        return correlation;
    }
    
    std::vector<double> primary_prices;
    for (const auto& candle : primary_candles) {
        primary_prices.push_back(candle.close);
    }
    
    double total_correlation = 0.0;
    size_t valid_correlations = 0;
    
    for (const auto& asset : correlated_assets) {
        std::vector<Candle> asset_candles;
        if (fetchAssetData(asset, asset_candles)) {
            std::vector<double> asset_prices;
            for (const auto& candle : asset_candles) {
                asset_prices.push_back(candle.close);
            }
            
            std::chrono::milliseconds optimal_lag;
            double pair_correlation = calculatePairwiseCorrelation(primary_prices, asset_prices, optimal_lag);
            
            if (std::abs(pair_correlation) >= MIN_CORRELATION_THRESHOLD) {
                correlation.correlated_pairs.push_back(asset);
                total_correlation += std::abs(pair_correlation);
                valid_correlations++;
                
                std::cout << "[ENHANCED_CACHE] 📊 " << primary_asset << " vs " << asset 
                          << ": correlation=" << pair_correlation << " lag=" << optimal_lag.count() << "ms" << std::endl;
            }
        }
    }
    
    correlation.correlation_strength = valid_correlations > 0 ? total_correlation / valid_correlations : 0.0;
    correlation.optimal_lag = std::chrono::milliseconds(0); // Simplified for now
    
    std::cout << "[ENHANCED_CACHE] 🎯 Average correlation strength for " << primary_asset 
              << ": " << correlation.correlation_strength << " with " << valid_correlations << " assets" << std::endl;
    
    return correlation;
}

double EnhancedMarketModelCache::calculatePairwiseCorrelation(
    const std::vector<double>& asset1_prices,
    const std::vector<double>& asset2_prices,
    std::chrono::milliseconds& optimal_lag) {
    
    // Simplified correlation calculation without lag optimization
    // In production, this would test multiple lag values
    optimal_lag = std::chrono::milliseconds(0);
    
    size_t min_size = std::min(asset1_prices.size(), asset2_prices.size());
    if (min_size < 10) return 0.0; // Need minimum data points
    
    // Use last CORRELATION_WINDOW_SIZE points or available data
    size_t window_size = std::min(CORRELATION_WINDOW_SIZE, min_size);
    size_t start_idx = min_size - window_size;
    
    // Calculate returns instead of raw prices for better correlation
    std::vector<double> returns1, returns2;
    for (size_t i = start_idx + 1; i < min_size; ++i) {
        returns1.push_back((asset1_prices[i] - asset1_prices[i-1]) / asset1_prices[i-1]);
        returns2.push_back((asset2_prices[i] - asset2_prices[i-1]) / asset2_prices[i-1]);
    }
    
    if (returns1.size() < 5) return 0.0;
    
    // Calculate Pearson correlation coefficient
    double mean1 = std::accumulate(returns1.begin(), returns1.end(), 0.0) / returns1.size();
    double mean2 = std::accumulate(returns2.begin(), returns2.end(), 0.0) / returns2.size();
    
    double numerator = 0.0, sum_sq1 = 0.0, sum_sq2 = 0.0;
    for (size_t i = 0; i < returns1.size(); ++i) {
        double diff1 = returns1[i] - mean1;
        double diff2 = returns2[i] - mean2;
        numerator += diff1 * diff2;
        sum_sq1 += diff1 * diff1;
        sum_sq2 += diff2 * diff2;
    }
    
    double denominator = std::sqrt(sum_sq1 * sum_sq2);
    return denominator > 0.0 ? numerator / denominator : 0.0;
}

bool EnhancedMarketModelCache::fetchAssetData(const std::string& instrument, std::vector<Candle>& out_candles) {
    out_candles.clear();
    
    // Try to use base cache first if available
    if (base_cache_) {
        if (base_cache_->ensureCacheForLastWeek(instrument)) {
            const auto& signal_map = base_cache_->getSignalMap();
            // Convert signals back to candle data (simplified)
            // In production, we'd store raw candle data in base cache
        }
    }
    
    // Fallback: fetch directly from OANDA
    bool data_fetched = false;
    auto oanda_candles = oanda_connector_->getHistoricalData(instrument, "M1", "", "");
    for (const auto& o_candle : oanda_candles) {
        Candle c;
        c.time = o_candle.time;
        c.open = o_candle.open;
        c.high = o_candle.high;
        c.low = o_candle.low;
        c.close = o_candle.close;
        c.volume = static_cast<double>(o_candle.volume);
        out_candles.push_back(c);
    }
    data_fetched = true;
    
    // Wait for data with timeout
    int timeout_seconds = 10;
    while (!data_fetched && timeout_seconds-- > 0) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    
    // Generate demo data if OANDA not available
    if (!data_fetched || out_candles.empty()) {
        std::cout << "[ENHANCED_CACHE] 🎲 Generating demo data for " << instrument << std::endl;
        
        // Generate realistic demo data based on instrument
        double base_price = 1.1585; // EUR_USD base
        if (instrument == "GBP_USD") base_price = 1.2750;
        else if (instrument == "AUD_USD") base_price = 0.6850;
        else if (instrument == "USD_CHF") base_price = 0.9150;
        else if (instrument == "USD_JPY") base_price = 149.50;
        
        for (int i = 0; i < 500; ++i) {
            Candle demo_candle;
            demo_candle.time = "2025-08-01T" + std::to_string(10 + (i / 60)) + ":" + 
                              std::to_string((i % 60)) + ":00.000000000Z";
            
            double movement = (std::rand() % 20 - 10) * 0.00001;
            base_price += movement;
            
            demo_candle.open = base_price;
            demo_candle.high = base_price + (std::rand() % 5) * 0.00001;
            demo_candle.low = base_price - (std::rand() % 5) * 0.00001;
            demo_candle.close = base_price + (std::rand() % 10 - 5) * 0.00001;
            demo_candle.volume = 100 + (std::rand() % 200);
            
            out_candles.push_back(demo_candle);
        }
        data_fetched = true;
    }
    
    return data_fetched && !out_candles.empty();
}

bool EnhancedMarketModelCache::saveEnhancedCache(const std::string& filepath) const {
    nlohmann::json j;
    j["metadata"]["version"] = "1.0";
    j["metadata"]["created_at"] = std::chrono::duration_cast<std::chrono::seconds>(
        std::chrono::system_clock::now().time_since_epoch()).count();
    j["metadata"]["cache_entries"] = cache_entries_.size();
    
    for (const auto& [key, entry] : cache_entries_) {
        nlohmann::json entry_json;
        entry_json["instrument"] = entry.instrument;
        entry_json["timeframe"] = timeFrameToString(entry.timeframe);
        entry_json["signal_count"] = entry.signals.size();
        entry_json["access_count"] = entry.access_count;
        
        // Save correlation data
        entry_json["correlation"]["primary_pair"] = entry.correlation_data.primary_pair;
        entry_json["correlation"]["strength"] = entry.correlation_data.correlation_strength;
        entry_json["correlation"]["optimal_lag_ms"] = entry.correlation_data.optimal_lag.count();
        entry_json["correlation"]["correlated_pairs"] = entry.correlation_data.correlated_pairs;
        
        // Save signals (first 100 for space efficiency)
        size_t signal_limit = std::min(entry.signals.size(), size_t(100));
        for (size_t i = 0; i < signal_limit; ++i) {
            const auto& signal = entry.signals[i];
            nlohmann::json signal_json;
            signal_json["action"] = (signal.quantum_signal.action == sep::trading::QuantumTradingSignal::BUY) ? "BUY" : "SELL";
            signal_json["confidence"] = signal.quantum_signal.identifiers.confidence;
            signal_json["coherence"] = signal.quantum_signal.identifiers.coherence;
            signal_json["stability"] = signal.quantum_signal.identifiers.stability;
            signal_json["correlation_boost"] = signal.correlation_boost;
            signal_json["contributing_assets"] = signal.contributing_assets;
            
            entry_json["signals"].push_back(signal_json);
        }
        
        j["cache_entries"][key] = entry_json;
    }
    
    std::ofstream file(filepath);
    if (!file.is_open()) {
        std::cerr << "[ENHANCED_CACHE] ❌ Failed to open cache file for writing: " << filepath << std::endl;
        return false;
    }
    
    file << j.dump(2);
    std::cout << "[ENHANCED_CACHE] 💾 Saved enhanced cache to " << filepath << std::endl;
    return true;
}

bool EnhancedMarketModelCache::loadEnhancedCache(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) return false;
    
    nlohmann::json j;
    try {
        file >> j;
    } catch (const std::exception& e) {
        std::cerr << "[ENHANCED_CACHE] ❌ Failed to parse cache file: " << e.what() << std::endl;
        return false;
    }
    
    cache_entries_.clear();
    
    if (j.contains("cache_entries")) {
        for (auto const& [key, entry_json] : j["cache_entries"].items()) {
            CacheEntry entry;
            entry.instrument = entry_json.value("instrument", "");
            entry.timeframe = stringToTimeFrame(entry_json.value("timeframe", "M1"));
            entry.access_count = entry_json.value("access_count", 0);
            entry.last_updated = std::chrono::system_clock::now();
            
            // Load correlation data
            if (entry_json.contains("correlation")) {
                const auto& corr_json = entry_json["correlation"];
                entry.correlation_data.primary_pair = corr_json.value("primary_pair", "");
                entry.correlation_data.correlation_strength = corr_json.value("strength", 0.0);
                entry.correlation_data.optimal_lag = std::chrono::milliseconds(corr_json.value("optimal_lag_ms", 0));
                entry.correlation_data.correlated_pairs = corr_json.value("correlated_pairs", std::vector<std::string>{});
            }
            
            // Load signals
            if (entry_json.contains("signals")) {
                for (const auto& signal_json : entry_json["signals"]) {
                    ProcessedSignal signal;
                    
                    std::string action = signal_json.value("action", "HOLD");
                    signal.quantum_signal.action = (action == "BUY") ? 
                        sep::trading::QuantumTradingSignal::BUY : sep::trading::QuantumTradingSignal::SELL;
                    
                    signal.quantum_signal.identifiers.confidence = signal_json.value("confidence", 0.5f);
                    signal.quantum_signal.identifiers.coherence = signal_json.value("coherence", 0.5f);
                    signal.quantum_signal.identifiers.stability = signal_json.value("stability", 0.5f);
                    signal.correlation_boost = signal_json.value("correlation_boost", 0.0);
                    signal.contributing_assets = signal_json.value("contributing_assets", std::vector<std::string>{});
                    signal.timestamp = std::chrono::system_clock::now();
                    
                    entry.signals.push_back(signal);
                }
            }
            
            cache_entries_[key] = entry;
        }
    }
    
    std::cout << "[ENHANCED_CACHE] 📊 Loaded " << cache_entries_.size() << " enhanced cache entries" << std::endl;
    return true;
}

EnhancedMarketModelCache::CachePerformanceMetrics EnhancedMarketModelCache::getPerformanceMetrics() const {
    CachePerformanceMetrics metrics;
    
    if (cache_entries_.empty()) return metrics;
    
    size_t total_accesses = 0;
    size_t total_signals = 0;
    double total_boost = 0.0;
    
    for (const auto& [key, entry] : cache_entries_) {
        total_accesses += entry.access_count;
        total_signals += entry.signals.size();
        
        for (const auto& signal : entry.signals) {
            total_boost += signal.correlation_boost;
        }
    }
    
    metrics.total_cache_entries = cache_entries_.size();
    metrics.correlation_enhanced_signals = total_signals;
    metrics.average_correlation_boost = total_signals > 0 ? total_boost / total_signals : 0.0;
    metrics.overall_hit_rate = total_accesses > 0 ? static_cast<double>(cache_entries_.size()) / total_accesses : 0.0;
    
    return metrics;
}

std::string EnhancedMarketModelCache::getCacheKey(const std::string& instrument, TimeFrame timeframe) const {
    return instrument + "_" + timeFrameToString(timeframe);
}

std::string EnhancedMarketModelCache::getCacheFilepath(const std::string& cache_key) const {
    return cache_directory_ + cache_key + "_enhanced.json";
}

std::string EnhancedMarketModelCache::timeFrameToString(TimeFrame tf) const {
    switch (tf) {
        case TimeFrame::M1: return "M1";
        case TimeFrame::M5: return "M5";
        case TimeFrame::M15: return "M15";
        case TimeFrame::H1: return "H1";
        case TimeFrame::H4: return "H4";
        case TimeFrame::D1: return "D1";
        default: return "M1";
    }
}

EnhancedMarketModelCache::TimeFrame EnhancedMarketModelCache::stringToTimeFrame(const std::string& tf_str) const {
    if (tf_str == "M5") return TimeFrame::M5;
    if (tf_str == "M15") return TimeFrame::M15;
    if (tf_str == "H1") return TimeFrame::H1;
    if (tf_str == "H4") return TimeFrame::H4;
    if (tf_str == "D1") return TimeFrame::D1;
    return TimeFrame::M1; // default
}

std::vector<EnhancedMarketModelCache::ProcessedSignal> EnhancedMarketModelCache::getCorrelationEnhancedSignals(const std::string& instrument) const {
    std::vector<ProcessedSignal> result;
    
    for (const auto& [key, entry] : cache_entries_) {
        if (entry.instrument == instrument) {
            result.insert(result.end(), entry.signals.begin(), entry.signals.end());
        }
    }
    
    return result;
}

} // namespace sep::cache
