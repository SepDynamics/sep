#include "data_cache_manager.hpp"
#include "common/financial_data_types.h"
#include <filesystem>
#include <iostream>
#include <condition_variable>

namespace sep::apps {

bool DataCacheManager::initialize(sep::connectors::OandaConnector* connector) {
    if (!connector) {
        std::cerr << "[DataCacheManager] Error: Null OANDA connector provided" << std::endl;
        return false;
    }
    
    oanda_connector_ = connector;
    
    // Create cache directory if it doesn't exist
    try {
        std::filesystem::create_directories(cache_directory_);
        std::cout << "[DataCacheManager] Initialized with cache directory: " << cache_directory_ << std::endl;
        return true;
    } catch (const std::exception& e) {
        std::cerr << "[DataCacheManager] Failed to create cache directory: " << e.what() << std::endl;
        return false;
    }
}

bool DataCacheManager::ensureCacheReady(const std::string& instrument) {
    std::lock_guard<std::mutex> lock(cache_mutex_);
    
    if (needsRefresh(instrument)) {
        std::cout << "[DataCacheManager] Cache needs refresh for " << instrument << std::endl;
        return refreshCache(instrument);
    }
    
    // Check if cache file exists and has data
    auto cache_info = getCacheInfo(instrument);
    if (!cache_info.valid || cache_info.data_points < EXPECTED_48H_POINTS * 0.95) {
        std::cout << "[DataCacheManager] Cache invalid or insufficient data for " << instrument 
                  << " (points: " << cache_info.data_points << "/" << EXPECTED_48H_POINTS << ")" << std::endl;
        return refreshCache(instrument);
    }
    
    std::cout << "[DataCacheManager] Cache ready for " << instrument 
              << " (points: " << cache_info.data_points << ", age: " 
              << std::chrono::duration_cast<std::chrono::minutes>(
                  std::chrono::system_clock::now() - cache_info.last_update).count() 
              << " minutes)" << std::endl;
    return true;
}

std::vector<sep::connectors::MarketData> DataCacheManager::getCachedData(const std::string& instrument) {
    std::lock_guard<std::mutex> lock(cache_mutex_);
    
    std::vector<sep::connectors::MarketData> data;
    if (!loadCacheFromFile(instrument, data)) {
        std::cerr << "[DataCacheManager] Failed to load cached data for " << instrument << std::endl;
        return {};
    }
    
    std::cout << "[DataCacheManager] Loaded " << data.size() << " cached data points for " << instrument << std::endl;
    return data;
}

bool DataCacheManager::refreshCache(const std::string& instrument) {
    if (!oanda_connector_) {
        std::cerr << "[DataCacheManager] No OANDA connector available" << std::endl;
        return false;
    }
    
    std::cout << "[DataCacheManager] Refreshing 48H cache for " << instrument << "..." << std::endl;
    
    // Use count parameter to get most recent 2880 candles
    std::vector<sep::connectors::OandaCandle> candles;
    std::mutex data_mutex;
    std::condition_variable data_ready;
    bool data_received = false;
    bool request_success = false;
    
    try {
        candles = oanda_connector_->getHistoricalData(instrument, "M1", "", "");
        data_received = true;
        data_ready.notify_one();
        
        request_success = true;
        
        // Wait for data with timeout
        std::unique_lock<std::mutex> lock(data_mutex);
        if (!data_ready.wait_for(lock, std::chrono::seconds(60), [&]{ return data_received; })) {
            std::cerr << "[DataCacheManager] Timeout waiting for historical data" << std::endl;
            return false;
        }
        
    } catch (const std::exception& e) {
        std::cerr << "[DataCacheManager] Exception during data fetch: " << e.what() << std::endl;
        return false;
    }
    
    if (candles.empty()) {
        std::cerr << "[DataCacheManager] Received empty historical data" << std::endl;
        return false;
    }
    
    std::cout << "[DataCacheManager] Received " << candles.size() << " candles, converting to MarketData..." << std::endl;
    
    // Convert to MarketData format
    auto market_data = convertOandaCandlesToMarketData(candles, instrument);
    
    // Save to cache
    if (!saveCacheToFile(instrument, market_data)) {
        std::cerr << "[DataCacheManager] Failed to save cache for " << instrument << std::endl;
        return false;
    }
    
    std::cout << "[DataCacheManager] Successfully cached " << market_data.size() 
              << " data points for " << instrument << std::endl;
    return true;
}

DataCacheManager::CacheInfo DataCacheManager::getCacheInfo(const std::string& instrument) const {
    CacheInfo info{};
    info.cache_file = getCacheFilePath(instrument);
    info.valid = false;
    info.data_points = 0;
    
    try {
        if (!std::filesystem::exists(info.cache_file)) {
            return info;
        }
        
        // Get file modification time
        auto file_time = std::filesystem::last_write_time(info.cache_file);
        auto sctp = std::chrono::time_point_cast<std::chrono::system_clock::duration>(
            file_time - std::filesystem::file_time_type::clock::now() + std::chrono::system_clock::now());
        info.last_update = sctp;
        
        // Count data points by loading file
        std::ifstream file(info.cache_file);
        if (file.is_open()) {
            nlohmann::json cache_json;
            file >> cache_json;
            
            if (cache_json.contains("data") && cache_json["data"].is_array()) {
                info.data_points = cache_json["data"].size();
                info.valid = true;
            }
        }
        
    } catch (const std::exception& e) {
        std::cerr << "[DataCacheManager] Error getting cache info: " << e.what() << std::endl;
    }
    
    return info;
}

bool DataCacheManager::needsRefresh(const std::string& instrument) const {
    auto cache_info = getCacheInfo(instrument);
    
    if (!cache_info.valid) {
        return true; // No valid cache
    }
    
    // Check age
    auto age = std::chrono::system_clock::now() - cache_info.last_update;
    if (age > CACHE_REFRESH_INTERVAL) {
        return true; // Cache too old
    }
    
    // Check data completeness (allow 5% tolerance)
    if (cache_info.data_points < EXPECTED_48H_POINTS * 0.95) {
        return true; // Insufficient data
    }
    
    return false;
}

std::string DataCacheManager::getCacheFilePath(const std::string& instrument) const {
    return cache_directory_ + instrument + "_48h_cache.json";
}

bool DataCacheManager::loadCacheFromFile(const std::string& instrument, 
                                        std::vector<sep::connectors::MarketData>& data) {
    data.clear();
    
    try {
        std::string cache_file = getCacheFilePath(instrument);
        
        if (!std::filesystem::exists(cache_file)) {
            return false;
        }
        
        std::ifstream file(cache_file);
        if (!file.is_open()) {
            return false;
        }
        
        nlohmann::json cache_json;
        file >> cache_json;
        
        if (!cache_json.contains("data") || !cache_json["data"].is_array()) {
            return false;
        }
        
        for (const auto& item : cache_json["data"]) {
            sep::connectors::MarketData md;
            md.instrument = item.value("instrument", instrument);
            md.mid = item.value("mid", 0.0);
            md.bid = item.value("bid", 0.0);
            md.ask = item.value("ask", 0.0);
            md.volume = item.value("volume", 0);
            md.atr = item.value("atr", 0.0001);
            md.timestamp = item.value("timestamp", 0ULL);
            
            data.push_back(md);
        }
        
        return true;
        
    } catch (const std::exception& e) {
        std::cerr << "[DataCacheManager] Error loading cache: " << e.what() << std::endl;
        return false;
    }
}

bool DataCacheManager::saveCacheToFile(const std::string& instrument, 
                                      const std::vector<sep::connectors::MarketData>& data) {
    try {
        std::string cache_file = getCacheFilePath(instrument);
        
        nlohmann::json cache_json;
        cache_json["instrument"] = instrument;
        cache_json["timestamp"] = std::chrono::duration_cast<std::chrono::seconds>(
            std::chrono::system_clock::now().time_since_epoch()).count();
        cache_json["data_points"] = data.size();
        
        nlohmann::json data_array = nlohmann::json::array();
        for (const auto& md : data) {
            nlohmann::json item;
            item["instrument"] = md.instrument;
            item["mid"] = md.mid;
            item["bid"] = md.bid;
            item["ask"] = md.ask;
            item["volume"] = md.volume;
            item["atr"] = md.atr;
            item["timestamp"] = md.timestamp;
            
            data_array.push_back(item);
        }
        cache_json["data"] = data_array;
        
        std::ofstream file(cache_file);
        if (!file.is_open()) {
            return false;
        }
        
        file << cache_json.dump(2); // Pretty print with 2-space indent
        return true;
        
    } catch (const std::exception& e) {
        std::cerr << "[DataCacheManager] Error saving cache: " << e.what() << std::endl;
        return false;
    }
}

std::vector<sep::connectors::MarketData> DataCacheManager::convertOandaCandlesToMarketData(
    const std::vector<sep::connectors::OandaCandle>& candles, 
    const std::string& instrument) {
    
    std::vector<sep::connectors::MarketData> market_data;
    market_data.reserve(candles.size());
    
    for (const auto& candle : candles) {
        sep::connectors::MarketData md;
        md.instrument = instrument;
        md.mid = candle.close; // Use close price as mid
        md.bid = candle.close - 0.00001; // Approximate spread
        md.ask = candle.close + 0.00001;
        md.volume = candle.volume;
        md.atr = 0.0001; // Default ATR
        
        // Convert timestamp to microseconds since epoch
        try {
            auto time_point = sep::common::parseTimestamp(candle.time);
            md.timestamp = std::chrono::duration_cast<std::chrono::microseconds>(
                time_point.time_since_epoch()).count();
        } catch (const std::exception& e) {
            std::cerr << "[DataCacheManager] Error parsing timestamp: " << e.what() << std::endl;
            md.timestamp = 0;
        }
        
        market_data.push_back(md);
    }
    
    return market_data;
}

} // namespace sep::apps
