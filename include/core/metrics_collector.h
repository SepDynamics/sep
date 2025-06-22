#pragma once

#include <atomic>
#include <cstdint>
#include <memory>
#include <unordered_map>
#include <mutex>
#include "compat/shim.h"
#include <string>

namespace sep {
namespace core {

// Forward declaration for PIMPL pattern
struct DetailedMetrics {
    float gpu_memory_usage{0.0f};
    float gpu_utilization{0.0f};
    std::uint64_t requests_per_second{0};
    std::uint64_t successful_requests{0};
    std::uint64_t p50_latency{0};
    std::uint64_t p95_latency{0};
    std::uint64_t p99_latency{0};
    float kernel_execution_time{0.0f};
    std::uint64_t memory_usage{0};
    float cpu_usage{0.0f};
};

// Forward declaration for PIMPL pattern
class MetricsCollector;

/**
 * @brief Metrics collection system for performance monitoring
 */
class MetricsCollector {
public:
    struct PerformanceMetrics {
        ::sep::shim::chrono::steady_clock::time_point start_time;
        ::sep::shim::chrono::steady_clock::time_point last_update;
        std::uint64_t total_operations = 0;
        std::uint64_t successful_operations = 0;
        std::uint64_t failed_operations = 0;
        double average_processing_time_ms = 0.0;
        double peak_processing_time_ms = 0.0;
    };

    struct MemoryMetrics {
        std::uint64_t allocated_bytes = 0;
        std::uint64_t peak_allocated_bytes = 0;
        std::uint64_t total_allocations = 0;
        std::uint64_t total_deallocations = 0;
    };

    struct SystemMetrics {
        double cpu_usage_percent = 0.0;
        std::uint64_t memory_usage_bytes = 0;
        std::uint64_t gpu_memory_usage_bytes = 0;
        double gpu_utilization_percent = 0.0;
    };

    static MetricsCollector& instance();

    // Performance tracking
    void recordOperation(const std::string& operation_name, bool success, double duration_ms);
    void incrementCounter(const std::string& counter_name, std::uint64_t value = 1);
    void setGauge(const std::string& gauge_name, double value);

    // Memory tracking
    void recordAllocation(std::uint64_t bytes);
    void recordDeallocation(std::uint64_t bytes);

    // System metrics
    void updateSystemMetrics();

    // Getters
    PerformanceMetrics getPerformanceMetrics(const std::string& operation_name) const;
    MemoryMetrics getMemoryMetrics() const;
    SystemMetrics getSystemMetrics() const;
    std::uint64_t getCounter(const std::string& counter_name) const;
    double getGauge(const std::string& gauge_name) const;
    
    // Detailed metrics access
    DetailedMetrics getDetailedMetrics() const;

    // Reset and cleanup
    void reset();
    void resetOperation(const std::string& operation_name);

private:
    MetricsCollector() = default;
    ~MetricsCollector() = default;
    MetricsCollector(const MetricsCollector&) = delete;
    MetricsCollector& operator=(const MetricsCollector&) = delete;

    // Forward declaration for PIMPL pattern
    class Impl;
    std::unique_ptr<Impl> pImpl;

    mutable std::mutex metrics_mutex_;
    std::unordered_map<::sep::shim::string, PerformanceMetrics, ::sep::shim::string_hash> performance_metrics_;
    std::unordered_map<::sep::shim::string, std::uint64_t, ::sep::shim::string_hash> counters_;
    std::unordered_map<::sep::shim::string, double, ::sep::shim::string_hash> gauges_;

    MemoryMetrics memory_metrics_;
    SystemMetrics system_metrics_;
};

} // namespace core
} // namespace sep
