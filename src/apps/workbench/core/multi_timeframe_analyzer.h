#pragma once

#include <map>
#include <memory>
#include <string>
#include <vector>
#include <chrono>
#include <mutex>
#include <deque>

#include "quantum/pattern_metric_engine.h"
#include "quantum/coherence_manager.h"
#include "engine/metrics_collector.h"
#include "connectors/oanda_connector.h"

namespace sep::workbench {

// Forward declaration - will use CandleData from trading_hud.h
struct CandleData;

struct TimeframeData {
    std::string timeframe_id;        // "1m", "5m", "15m", "1h", "4h", "1D"
    int interval_minutes;            // 1, 5, 15, 60, 240, 1440
    std::deque<CandleData> candles;  // Rolling window of candles
    size_t max_candles = 1000;      // Keep last 1000 candles
    
    TimeframeData() : interval_minutes(1) {}  // Default constructor
    TimeframeData(const std::string& id, int minutes) 
        : timeframe_id(id), interval_minutes(minutes) {}
};

struct TimeframeMetrics {
    std::string timeframe;
    std::chrono::system_clock::time_point timestamp;
    
    // Core SEP metrics from PatternMetricEngine
    std::vector<sep::quantum::PatternMetrics> detected_patterns;
    float dominant_coherence = 0.0f;
    float stability_index = 0.0f;
    float entropy_level = 0.0f;
    
    // Derived trading metrics
    float trend_strength = 0.0f;         // Based on pattern stability
    float volatility_prediction = 0.0f;  // Based on entropy patterns
    float breakout_probability = 0.0f;   // Based on coherence changes
    
    // Cross-timeframe signals
    float momentum_alignment = 0.0f;     // How aligned this TF is with others
    float regime_consistency = 0.0f;     // Consistency across timeframes
    
    enum TrendDirection { STRONG_DOWN = -2, DOWN = -1, NEUTRAL = 0, UP = 1, STRONG_UP = 2 };
    TrendDirection trend_direction = NEUTRAL;
    
    TimeframeMetrics() {}  // Default constructor
    TimeframeMetrics(const std::string& tf) : timeframe(tf) {
        timestamp = std::chrono::system_clock::now();
    }
};

struct CorrelationMetrics {
    double coherence_pearson = 0.0;
    double coherence_spearman = 0.0;
    double stability_pearson = 0.0;
    double stability_spearman = 0.0;
    double entropy_pearson = 0.0;
    double entropy_spearman = 0.0;
};

struct MultiTimeframeSignal {
    std::chrono::system_clock::time_point generated_at;
    std::string instrument;
    
    // Composite signals from all timeframes
    float composite_alpha_score = 0.0f;       // Primary trading signal (-1 to +1)
    float signal_confidence = 0.0f;           // How confident we are (0 to 1)
    float risk_assessment = 0.0f;             // Risk level (0 to 1)
    
    // Multi-timeframe context
    std::map<std::string, TimeframeMetrics> timeframe_metrics;
    
    // Trading recommendations
    enum ActionRecommendation { 
        STRONG_SELL, SELL, HOLD, BUY, STRONG_BUY, NO_SIGNAL 
    };
    ActionRecommendation primary_action = NO_SIGNAL;
    
    // Additional context
    std::vector<std::string> supporting_timeframes;  // TFs supporting the signal
    std::vector<std::string> conflicting_timeframes; // TFs opposing the signal
    std::string regime_description;                   // Market regime context
};

class MultiTimeframeAnalyzer {
public:
    struct Config {
        std::vector<std::string> timeframes;
        size_t max_candles_per_tf = 1000;
        bool enable_cuda_acceleration = true;
        
        // SEP Engine specific configs
        float pattern_quality_threshold = 0.5f;
        int max_patterns_per_tf = 100;
        float coherence_threshold = 0.3f;
        
        Config() {
            timeframes = {"1m", "5m", "15m", "1h", "4h"};
        }
    };

private:
    Config config_;
    std::mutex analysis_mutex_;
    
    // SEP Engine components - one per timeframe for parallel processing
    std::map<std::string, std::unique_ptr<sep::quantum::PatternMetricEngine>> pattern_engines_;
    std::unique_ptr<sep::quantum::CoherenceManager> coherence_manager_;
    std::unique_ptr<sep::core::MetricsCollector> metrics_collector_;
    
    // Timeframe data management
    std::map<std::string, TimeframeData> timeframe_data_;
    std::map<std::string, TimeframeMetrics> latest_metrics_;
    
    // Historical analysis for trend detection
    std::deque<MultiTimeframeSignal> signal_history_;
    size_t max_signal_history_ = 500;

    // Rolling history of analyzed metrics per timeframe
    std::map<std::string, std::deque<TimeframeMetrics>> metrics_history_;
    size_t max_metrics_history_ = 1000;

    // Correlation history per timeframe
    std::map<std::string, std::deque<CorrelationMetrics>> correlation_history_;
    size_t max_correlation_history_ = 1000;
    
    // Internal methods
    std::vector<CandleData> resampleCandles(
        const std::vector<CandleData>& source_candles, 
        int target_interval_minutes);
    
    TimeframeMetrics analyzeTimeframe(
        const std::string& timeframe, 
        const std::vector<CandleData>& candles);
    
    float calculateTrendStrength(const std::vector<sep::quantum::PatternMetrics>& patterns);
    float calculateVolatilityPrediction(const std::vector<sep::quantum::PatternMetrics>& patterns);
    float calculateBreakoutProbability(const sep::quantum::PatternMetrics& latest_pattern);
    
    MultiTimeframeSignal::ActionRecommendation determineAction(
        const std::map<std::string, TimeframeMetrics>& tf_metrics);
    
    float calculateCompositeAlpha(const std::map<std::string, TimeframeMetrics>& tf_metrics);
    float calculateSignalConfidence(const std::map<std::string, TimeframeMetrics>& tf_metrics);

public:
    explicit MultiTimeframeAnalyzer(const Config& config = Config{});
    ~MultiTimeframeAnalyzer();
    
    // Initialization
    bool initialize();
    void shutdown();
    
    // Data ingestion
    void ingestMarketData(const std::string& instrument, const CandleData& candle);
    void ingestHistoricalData(const std::string& instrument, 
                            const std::vector<CandleData>& historical_candles);
    
    // Analysis methods
    MultiTimeframeSignal generateSignal(const std::string& instrument);
    std::map<std::string, TimeframeMetrics> getLatestMetrics(const std::string& instrument);
    
    // Real-time updates
    void updateAllTimeframes(const std::string& instrument);
    
    // Configuration and tuning
    void updateConfig(const Config& new_config);
    Config getCurrentConfig() const { return config_; }
    
    // Performance metrics
    struct PerformanceStats {
        double avg_processing_time_ms = 0.0;
        double gpu_utilization_pct = 0.0;
        int patterns_processed_per_second = 0;
        size_t total_patterns_tracked = 0;
        double memory_usage_mb = 0.0;
    };
    
    PerformanceStats getPerformanceStats() const;
    
    // Debug and monitoring
    std::vector<std::string> getActiveTimeframes() const;
    size_t getPatternsCount(const std::string& timeframe) const;
    std::string getStatusReport() const;

    CorrelationMetrics calculateCorrelationMetrics(const std::string& timeframe);
    std::deque<CorrelationMetrics> getCorrelationHistory(const std::string& timeframe) const;
};

// Helper functions for timeframe conversions
namespace TimeframeUtils {
    int getIntervalMinutes(const std::string& timeframe);
    std::string getTimeframeFromMinutes(int minutes);
    bool isValidTimeframe(const std::string& timeframe);
    std::vector<std::string> getStandardTimeframes();
}

} // namespace sep::workbench
