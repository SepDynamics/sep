#pragma once

#include <atomic>
#include <chrono>
#include <memory>
#include <mutex>
#include <thread>
#include <vector>

namespace sep::workbench {

/**
 * @brief Real-time memory usage monitor for the SEP Workbench
 * 
 * Tracks process memory usage, heap allocations, and provides
 * time-series data for visualization in the metrics dashboard.
 */
class MemoryMonitor {
public:
    struct MemorySnapshot {
        std::chrono::steady_clock::time_point timestamp;
        size_t rss_bytes;           // Resident Set Size
        size_t vms_bytes;           // Virtual Memory Size
        size_t heap_bytes;          // Heap usage
        size_t pattern_count;       // Number of active patterns
        size_t tier1_bytes;         // L1 tier memory
        size_t tier2_bytes;         // L2 tier memory
        size_t tier3_bytes;         // L3 tier memory
        float cache_hit_rate;       // Cache efficiency
    };

    struct MemoryStats {
        size_t current_rss{0};
        size_t peak_rss{0};
        size_t current_heap{0};
        size_t peak_heap{0};
        float growth_rate_mb_per_sec{0.0f};
        float avg_bytes_per_pattern{0.0f};
        size_t total_allocations{0};
        size_t total_deallocations{0};
    };

public:
    MemoryMonitor();
    ~MemoryMonitor();

    // Lifecycle
    bool initialize();
    void shutdown();
    void reset();

    // Monitoring control
    void startMonitoring(std::chrono::milliseconds interval = std::chrono::milliseconds(100));
    void stopMonitoring();
    bool isMonitoring() const { return monitoring_.load(); }

    // Data access
    MemoryStats getCurrentStats() const;
    std::vector<MemorySnapshot> getHistory(size_t max_samples = 1000) const;
    MemorySnapshot getLatestSnapshot() const;

    // Configuration
    void setHistorySize(size_t max_snapshots) { max_history_size_ = max_snapshots; }
    void setPatternCount(size_t count) { current_pattern_count_ = count; }
    void updateTierUsage(size_t tier1, size_t tier2, size_t tier3);
    void updateCacheHitRate(float rate) { cache_hit_rate_ = rate; }

    // Memory pressure detection
    bool isUnderMemoryPressure() const;
    float getMemoryPressureLevel() const;  // 0.0 = no pressure, 1.0 = critical

private:
    void monitoringThread();
    void captureSnapshot();
    void updateStats();
    void pruneHistory();
    
    // Platform-specific memory queries
    size_t getCurrentRSS() const;
    size_t getCurrentVMS() const;
    size_t getCurrentHeap() const;

private:
    std::atomic<bool> monitoring_{false};
    std::atomic<bool> shutdown_requested_{false};
    std::unique_ptr<std::thread> monitor_thread_;
    
    mutable std::mutex data_mutex_;
    std::vector<MemorySnapshot> history_;
    MemoryStats current_stats_;
    
    // Configuration
    size_t max_history_size_{10000};
    std::chrono::milliseconds monitoring_interval_{100};
    
    // Current state
    std::atomic<size_t> current_pattern_count_{0};
    std::atomic<float> cache_hit_rate_{0.0f};
    size_t tier1_bytes_{0};
    size_t tier2_bytes_{0};
    size_t tier3_bytes_{0};
    
    // Timing
    std::chrono::steady_clock::time_point start_time_;
    std::chrono::steady_clock::time_point last_snapshot_time_;
};

} // namespace sep::workbench