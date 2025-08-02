#pragma once

#include <vector>
#include <string>
#include <chrono>
#include <memory>
#include <functional>
#include <map>

#include "connectors/oanda_connector.h"
#include "candle_types.h"
#include "quantum/bitspace/qfh.h"

namespace sep::apps {

/**
 * MarketModelCache - Dynamic Historical Market Data Cache and Processing System
 * 
 * This system:
 * 1. Fetches current historical data from last market close (Friday)
 * 2. Processes it through the full quantum metrics pipeline
 * 3. Builds toward a complete market model with path history
 * 4. Caches processed metrics for rapid access
 * 5. Scales toward decade+ historical datasets
 */
class MarketModelCache {
public:
    struct ProcessedMetrics {
        std::chrono::system_clock::time_point timestamp;
        double entropy;
        double coherence;
        double stability;
        double trajectory_confidence;
        std::string pattern_type;
        double signal_strength;
    };
    
    struct MarketSession {
        std::chrono::system_clock::time_point start_time;
        std::chrono::system_clock::time_point end_time;
        std::vector<Candle> raw_data;
        std::vector<ProcessedMetrics> processed_metrics;
        bool is_complete = false;
    };

public:
    MarketModelCache(std::shared_ptr<sep::connectors::OandaConnector> oanda_connector);
    ~MarketModelCache() = default;

    // Core API
    bool initializeWithCurrentWeek(const std::string& instrument = "EUR_USD");
    bool loadProcessedSession(const std::string& session_id);
    bool processHistoricalData(const std::vector<Candle>& candles);
    
    // Data Access
    const std::vector<ProcessedMetrics>& getCurrentMetrics() const { return current_metrics_; }
    const MarketSession& getCurrentSession() const { return current_session_; }
    
    // Model Building
    bool saveModelSnapshot(const std::string& snapshot_name);
    bool loadModelSnapshot(const std::string& snapshot_name);
    
    // Cache Management
    bool refreshFromMarketClose(const std::string& instrument);
    std::string getCacheStatus() const;
    
    // Configuration
    void setTimeWindow(int hours) { time_window_hours_ = hours; }
    void enablePersistence(bool enable) { persistence_enabled_ = enable; }

private:
    // Market Time Calculations
    std::chrono::system_clock::time_point getLastMarketClose();
    std::chrono::system_clock::time_point getNextMarketOpen();
    bool isMarketOpen() const;
    
    // Data Fetching
    void fetchHistoricalDataAsync(
        const std::string& instrument,
        const std::chrono::system_clock::time_point& start_time,
        const std::chrono::system_clock::time_point& end_time,
        std::function<void(const std::vector<Candle>&)> callback
    );
    
    // Metrics Processing
    ProcessedMetrics computeMetricsForCandle(const Candle& candle, const std::vector<Candle>& window);
    void processWithQuantumPipeline(const std::vector<Candle>& candles);
    
    // Persistence
    bool saveToDisk(const std::string& cache_key) const;
    bool loadFromDisk(const std::string& cache_key);
    bool fetchHistoricalDataForCache(const std::string& instrument);
    std::string generateCacheKey(const std::string& instrument, 
                                 const std::chrono::system_clock::time_point& start_time) const;
    
    // Path to cache directory
    std::string getCacheDirectory() const { return "/sep/cache/market_model/"; }

private:
    std::shared_ptr<sep::connectors::OandaConnector> oanda_connector_;
    std::unique_ptr<sep::quantum::QFHProcessor> qfh_processor_;
    
    // Current state
    MarketSession current_session_;
    std::vector<ProcessedMetrics> current_metrics_;
    std::map<std::string, MarketSession> cached_sessions_;
    
    // Configuration
    int time_window_hours_ = 120; // Default: 5 days (120 hours)
    bool persistence_enabled_ = true;
    std::string current_instrument_ = "EUR_USD";
    
    // Processing state
    std::atomic<bool> processing_active_{false};
    std::string last_error_;
};

} // namespace sep::apps
