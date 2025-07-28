#pragma once

#include "connectors/oanda_connector.h"
#include <chrono>
#include <memory>
#include <vector>
#include <string>
#include <mutex>
#include <fstream>
#include <nlohmann/json.hpp>

namespace sep::apps {

/**
 * Manages 48H historical data caching for quantum analysis
 * Separates data retrieval from computation as requested
 */
class DataCacheManager {
public:
    DataCacheManager() = default;
    ~DataCacheManager() = default;

    struct CacheInfo {
        std::chrono::system_clock::time_point last_update;
        size_t data_points;
        std::string cache_file;
        bool valid;
    };

    /**
     * Initialize the cache manager with OANDA connector
     */
    bool initialize(sep::connectors::OandaConnector* connector);

    /**
     * Ensure 48H cache is available and fresh
     * Returns true if cache is ready, false if update failed
     */
    bool ensureCacheReady(const std::string& instrument = "EUR_USD");

    /**
     * Get cached historical data
     * Returns empty vector if cache not ready
     */
    std::vector<sep::connectors::MarketData> getCachedData(const std::string& instrument = "EUR_USD");

    /**
     * Force refresh of cache (retrieves new 48H data from OANDA)
     */
    bool refreshCache(const std::string& instrument = "EUR_USD");

    /**
     * Get cache status information
     */
    CacheInfo getCacheInfo(const std::string& instrument = "EUR_USD") const;

    /**
     * Check if cache needs refresh (older than 1 hour or missing data)
     */
    bool needsRefresh(const std::string& instrument = "EUR_USD") const;

private:
    sep::connectors::OandaConnector* oanda_connector_;
    mutable std::mutex cache_mutex_;
    
    // Cache storage
    std::string cache_directory_ = "cache/oanda/";
    
    // Helper methods
    std::string getCacheFilePath(const std::string& instrument) const;
    bool loadCacheFromFile(const std::string& instrument, std::vector<sep::connectors::MarketData>& data);
    bool saveCacheToFile(const std::string& instrument, const std::vector<sep::connectors::MarketData>& data);
    std::vector<sep::connectors::MarketData> convertOandaCandlesToMarketData(
        const std::vector<sep::connectors::OandaCandle>& candles, 
        const std::string& instrument);
    
    // Constants
    static constexpr size_t EXPECTED_48H_POINTS = 2880; // 48 hours * 60 minutes
    static constexpr std::chrono::hours CACHE_REFRESH_INTERVAL{1}; // Refresh every hour
};

} // namespace sep::apps
