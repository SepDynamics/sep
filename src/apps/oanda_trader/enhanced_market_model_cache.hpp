#pragma once
#include <vector>
#include <string>
#include <memory>
#include <map>
#include <chrono>
#include <unordered_map>
#include "connectors/oanda_connector.h"
#include "candle_types.h"
#include "quantum_signal_bridge.hpp"
#include "market_model_cache.hpp"

namespace sep::cache {

// Enhanced Market Model Cache with Multi-Asset Intelligence
class EnhancedMarketModelCache {
public:
    struct CrossAssetCorrelation {
        std::string primary_pair;      // EUR_USD
        std::vector<std::string> correlated_pairs; // GBP_USD, AUD_USD, etc.
        double correlation_strength;
        std::chrono::milliseconds optimal_lag;
        std::chrono::system_clock::time_point last_updated;
    };
    
    struct ProcessedSignal {
        sep::trading::QuantumTradingSignal quantum_signal;
        double correlation_boost;      // Signal boost from correlated assets
        std::vector<std::string> contributing_assets;
        std::chrono::system_clock::time_point timestamp;
    };
    
    enum class TimeFrame {
        M1, M5, M15, H1, H4, D1
    };
    
    struct CacheEntry {
        std::string instrument;
        TimeFrame timeframe;
        std::vector<ProcessedSignal> signals;
        CrossAssetCorrelation correlation_data;
        std::chrono::system_clock::time_point last_updated;
        double cache_hit_rate = 0.0;
        size_t access_count = 0;
    };

private:
    // Multi-asset correlation pairs for EUR_USD
    const std::vector<std::string> EUR_USD_CORRELATED_PAIRS = {
        "GBP_USD",   // Strong positive correlation
        "AUD_USD",   // Moderate positive correlation  
        "USD_CHF",   // Strong negative correlation
        "USD_JPY",   // Moderate negative correlation
        "EUR_GBP",   // Cross-pair correlation
        "EUR_JPY"    // Cross-pair correlation
    };
    
    std::shared_ptr<sep::connectors::OandaConnector> oanda_connector_;
    std::shared_ptr<sep::apps::MarketModelCache> base_cache_;
    std::unordered_map<std::string, CacheEntry> cache_entries_;
    std::string cache_directory_ = "/sep/cache/enhanced_market_model/";
    
    // Correlation analysis parameters
    static constexpr double MIN_CORRELATION_THRESHOLD = 0.3;
    static constexpr size_t CORRELATION_WINDOW_SIZE = 100;  // 100 candles for correlation calculation
    static constexpr std::chrono::hours CACHE_VALIDITY_PERIOD{24}; // 24 hours
    
public:
    explicit EnhancedMarketModelCache(
        std::shared_ptr<sep::connectors::OandaConnector> connector,
        std::shared_ptr<sep::apps::MarketModelCache> base_cache = nullptr
    );
    
    // Main enhancement functions
    bool ensureEnhancedCacheForInstrument(const std::string& instrument, TimeFrame timeframe = TimeFrame::M1);
    ProcessedSignal generateCorrelationEnhancedSignal(const std::string& target_asset, const std::string& timestamp);
    void updateCorrelatedAssets(const std::string& primary_asset);
    
    // Cache management
    void optimizeCacheHierarchy();  // Smart eviction based on correlation strength
    bool loadEnhancedCache(const std::string& filepath);
    bool saveEnhancedCache(const std::string& filepath) const;
    
    // Correlation analysis
    CrossAssetCorrelation calculateCrossAssetCorrelation(
        const std::string& primary_asset, 
        const std::vector<std::string>& correlated_assets
    );
    double calculatePairwiseCorrelation(
        const std::vector<double>& asset1_prices,
        const std::vector<double>& asset2_prices,
        std::chrono::milliseconds& optimal_lag
    );
    
    // Accessors
    const std::unordered_map<std::string, CacheEntry>& getCacheEntries() const { return cache_entries_; }
    std::vector<ProcessedSignal> getCorrelationEnhancedSignals(const std::string& instrument) const;
    
    // Performance metrics
    struct CachePerformanceMetrics {
        double overall_hit_rate = 0.0;
        size_t total_cache_entries = 0;
        size_t correlation_enhanced_signals = 0;
        double average_correlation_boost = 0.0;
        std::chrono::milliseconds average_signal_generation_time{0};
    };
    CachePerformanceMetrics getPerformanceMetrics() const;
    
private:
    std::string getCacheKey(const std::string& instrument, TimeFrame timeframe) const;
    std::string getCacheFilepath(const std::string& cache_key) const;
    TimeFrame stringToTimeFrame(const std::string& tf_str) const;
    std::string timeFrameToString(TimeFrame tf) const;
    
    // Asset data fetching
    bool fetchAssetData(const std::string& instrument, std::vector<Candle>& out_candles);
    
    // Signal enhancement logic
    double calculateCorrelationBoost(
        const sep::trading::QuantumTradingSignal& base_signal,
        const std::vector<sep::trading::QuantumTradingSignal>& correlated_signals,
        const CrossAssetCorrelation& correlation_data
    );
};

} // namespace sep::cache
