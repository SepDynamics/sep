#pragma once

#include <vector>
#include <string>
#include <memory>
#include <atomic>
#include <mutex>
#include <chrono>
#include <unordered_map>
#include "connectors/oanda_connector.h"

namespace sep::workbench {

/**
 * @brief Real-time metrics monitoring system for SEP engine data
 * 
 * This class provides the core functionality needed for an investor demo:
 * - Real-time pattern detection and metrics calculation
 * - Data stream ingestion with configurable sources
 * - Coherence, Stability, and Entropy tracking
 * - Pattern statistics (length, frequency, persistence)
 * - Clear/reset functionality for fresh analysis
 */
class MetricsMonitor {
public:
    // Default constructor - creates its own engine
    MetricsMonitor();
    
    struct PatternStats {
        std::string pattern_id;
        float coherence{0.0f};
        float stability{0.0f};
        float entropy{0.0f};
        size_t length{0};
        size_t frequency{0};
        float persistence{0.0f};  // How long pattern has persisted
        std::chrono::steady_clock::time_point first_seen;
        std::chrono::steady_clock::time_point last_seen;
    };

    struct SystemMetrics {
        float avg_coherence{0.0f};
        float avg_stability{0.0f};
        float avg_entropy{0.0f};
        size_t total_patterns{0};
        size_t unique_patterns{0};
        size_t active_patterns{0};  // Patterns seen in last N seconds
        float pattern_emergence_rate{0.0f};  // New patterns per second
        std::chrono::steady_clock::time_point last_update;
    };

    struct RollingMetrics {
        float coherence_24h_avg{0.0f};
        float stability_24h_avg{0.0f};
        float entropy_24h_avg{0.0f};
        float coherence_1h_avg{0.0f};
        float stability_1h_avg{0.0f};
        float entropy_1h_avg{0.0f};
        float coherence_trend{0.0f};  // Direction of change over 24h
        float stability_trend{0.0f};
        float entropy_trend{0.0f};
        std::chrono::steady_clock::time_point last_calculation;
    };

    struct ThresholdSignal {
        enum Type { SELL, BUY, HOLD };
        Type signal_type{HOLD};
        float confidence{0.0f};
        std::string reason;
        std::chrono::steady_clock::time_point timestamp;
        
        // Signal conditions that triggered
        // Signal conditions that triggered
        bool low_stability{false};
        bool high_entropy{false};
        bool coherence_drop{false};
        bool rapid_change{false};
        bool entropy_spike{false};

        // Advanced signal metrics
        float cumulative_score{0.0f};
        float trade_confidence{0.0f};
    };

public:
    ~MetricsMonitor();

    // Core functionality
    bool initialize();
    void shutdown();
    void clear();  // Reset all pattern data for fresh analysis

    // Data ingestion
    void ingestData(const uint8_t* data, size_t size);
    void ingestFile(const std::string& filepath);
    void ingestStream(std::istream& stream);

    // Real-time processing
    void startProcessing();
    void stopProcessing();
    bool isProcessing() const;

    // Metrics access
    const std::vector<PatternStats>& getPatternStats() const;
    const SystemMetrics& getSystemMetrics() const;
    const RollingMetrics& getRollingMetrics() const;
    const ThresholdSignal& getLatestSignal() const;
    sep::connectors::MarketData getLatestMarketData() const;
    void setLatestMarketData(const sep::connectors::MarketData& data);
    
    // Added for debugging and testing
    std::unordered_map<std::string, double> getMetrics() {
        std::lock_guard<std::mutex> lock(metrics_mutex_);
        return {};
    }

    void set(const std::string& key, double value) {
        std::lock_guard<std::mutex> lock(metrics_mutex_);
    }
    
    // Simple threshold detection as specified in TODO.md
    struct SimpleSignals {
        bool sellSignal{false};
        bool buySignal{false};
        float stability{0.0f};
        float entropy{0.0f};
        float coherence{0.0f};
    };
    SimpleSignals getSimpleThresholdSignals() const;
    
    // Pattern selection and filtering
    void setMinPatternLength(size_t min_length);
    void setMaxPatterns(size_t max_patterns);
    void setActiveWindowSeconds(float seconds);

    // Export functionality
    std::string exportMetricsAsJSON() const;
    void saveMetricsToFile(const std::string& filepath) const;

private:
    void updateMetrics();
    void processIngestedData();
    void calculateSystemMetrics();
    void calculateRollingMetrics();
    void detectThresholdSignals();
    
    // Forward declaration to avoid circular dependency
    // The actual engine will be created in the implementation
    void* engine_; // Using void* to avoid including the header here
    std::vector<PatternStats> pattern_stats_;
    SystemMetrics system_metrics_;
    RollingMetrics rolling_metrics_;
    ThresholdSignal latest_signal_;
    sep::connectors::MarketData latest_market_data_;
    
    // Historical data for rolling calculations
    struct MetricsSnapshot {
        float coherence, stability, entropy;
        std::chrono::steady_clock::time_point timestamp;
    };
    std::vector<MetricsSnapshot> metrics_history_;
    
    std::atomic<bool> processing_{false};
    std::atomic<bool> shutdown_requested_{false};
    mutable std::mutex metrics_mutex_;
    
    // Configuration
    size_t min_pattern_length_{4};
    size_t max_patterns_{1000};
    float active_window_seconds_{30.0f};
    
    // Timing
    std::chrono::steady_clock::time_point start_time_;
    std::chrono::steady_clock::time_point last_metrics_update_;
};

} // namespace sep::workbench
