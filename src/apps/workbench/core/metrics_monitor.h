#pragma once

#include <vector>
#include <string>
#include <memory>
#include <atomic>
#include <mutex>
#include <chrono>

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
    
    // Forward declaration to avoid circular dependency
    // The actual engine will be created in the implementation
    void* engine_; // Using void* to avoid including the header here
    std::vector<PatternStats> pattern_stats_;
    SystemMetrics system_metrics_;
    
    std::atomic<bool> processing_{false};
    std::atomic<bool> shutdown_requested_{false};
    std::mutex metrics_mutex_;
    
    // Configuration
    size_t min_pattern_length_{4};
    size_t max_patterns_{1000};
    float active_window_seconds_{30.0f};
    
    // Timing
    std::chrono::steady_clock::time_point start_time_;
    std::chrono::steady_clock::time_point last_metrics_update_;
};

} // namespace sep::workbench
