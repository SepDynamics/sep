#include "core/metrics_collector.h"
#include "compat/cuda_common.h"
#include "compat/cuda_helpers.h"
#include <cuda_runtime_api.h>

#include <sys/resource.h>
#include <sys/sysinfo.h>
#include <unistd.h>

#include <algorithm>
#include <atomic>
#include <cstring>
#include <deque>
#include <iostream>
#include <memory>
#include <mutex>
#include <stdexcept>
#include <system_error>
#include <thread>
#include <vector>
#include <chrono>
#include <cmath> // Required for std::sqrt

using namespace sep::shim::chrono_literals;
using namespace sep::cuda;

namespace sep::metrics {

class MetricsCollector::Impl {
 public:

  Impl() : running_(false), latency_window_size_(1000) {
    // Create events for timing
        CUDA_CHECK(cudaEventCreate(&start_event_));
    CUDA_CHECK(cudaEventCreate(&stop_event_));
    
  }

  ~Impl() {
    stopCollection();
    if (start_event_) {
      CUDA_CHECK(cudaEventDestroy(start_event_));
    }
    if (stop_event_) {
      CUDA_CHECK(cudaEventDestroy(stop_event_));
    }
  }

  void startCollection() {
    if (running_) return;
    running_ = true;

    // Start collection thread using lambda
    threads_.emplace_back([this]() {
      while (running_) {
        collectMetrics();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
      }
    });
  }

  void stopCollection() {
    if (!running_) return;
    running_ = false;
    for (auto& thread : threads_) {
      if (thread.joinable()) {
        thread.join();
      }
    }
    threads_.clear();
  }

  DetailedMetrics getCurrentMetrics() const {
    std::lock_guard<std::mutex> lock(metrics_mutex_);
    return current_metrics_;
  }

  void updateMetrics(const DetailedMetrics& metrics) {
    std::lock_guard<std::mutex> lock(metrics_mutex_);
    current_metrics_ = metrics;
  }

  void recordKernelStart() {
    if (start_event_) {
      CUDA_CHECK(cudaEventRecord(start_event_, nullptr));
    }
  }

  void recordKernelStop() {
    if (stop_event_) {
            CUDA_CHECK(cudaEventRecord(stop_event_, nullptr));
      CUDA_CHECK(cudaEventSynchronize(stop_event_));

      float elapsed_time = 0.0f;
      if (start_event_ && stop_event_) {
        CUDA_CHECK(cudaEventElapsedTime(&elapsed_time, start_event_, stop_event_));
      }
      

      std::lock_guard<std::mutex> lock(metrics_mutex_);
      current_metrics_.kernel_execution_time = elapsed_time;
    }
  }

  void addLatencySample(std::uint64_t latency_us) {
    std::lock_guard<std::mutex> lock(metrics_mutex_);
    latency_samples_.push_back(latency_us);
    while (latency_samples_.size() > latency_window_size_) {
      latency_samples_.pop_front();
    }
    updateLatencyMetrics();
  }

  void updateLatencyMetrics() {
    if (latency_samples_.empty()) return;

    std::vector<std::uint64_t> sorted_samples(latency_samples_.begin(), latency_samples_.end());
    std::sort(sorted_samples.begin(), sorted_samples.end());

    size_t count = sorted_samples.size();
    if (count > 0) {
      current_metrics_.p50_latency = sorted_samples[count * 50 / 100];
      current_metrics_.p95_latency = sorted_samples[count * 95 / 100];
      current_metrics_.p99_latency = sorted_samples[count * 99 / 100];
    }
  }

  void collectCPUMetrics() {
    struct rusage usage;
    if (getrusage(RUSAGE_SELF, &usage) == 0) {
      // Convert to milliseconds and calculate CPU usage percentage
      std::uint64_t cpu_time = (usage.ru_utime.tv_sec + usage.ru_stime.tv_sec) * 1000 +
                               (usage.ru_utime.tv_usec + usage.ru_stime.tv_usec) / 1000;

      static std::uint64_t last_cpu_time = 0;
      static auto last_check = sep::shim::chrono::steady_clock::now();
      auto now = sep::shim::chrono::steady_clock::now();

      auto elapsed =
          sep::shim::chrono::duration_cast<sep::shim::chrono::milliseconds>(now - last_check);
      if (elapsed > 0) {
        float cpu_usage = static_cast<float>(cpu_time - last_cpu_time) / elapsed * 100.0f;
        current_metrics_.cpu_usage = cpu_usage;
      }

      last_cpu_time = cpu_time;
      last_check = now;
    }
  }

  void collectMemoryMetrics() {
    struct sysinfo si;
    if (sysinfo(&si) == 0) {
      float total_ram = si.totalram * si.mem_unit;
      float used_ram = (si.totalram - si.freeram) * si.mem_unit;
      float memory_usage = (used_ram / total_ram) * 100.0f;
      current_metrics_.memory_usage = static_cast<std::uint64_t>(memory_usage);
    }
  }

  void collectMetrics() {
    DetailedMetrics new_metrics;

    // Get CUDA memory info
    size_t free_mem, total_mem;
    CUDA_CHECK(cudaMemGetInfo(&free_mem, &total_mem));
    float gpu_memory_usage = static_cast<float>(total_mem - free_mem) / total_mem * 100.0f;
    new_metrics.gpu_memory_usage = gpu_memory_usage;
    // GPU utilization requires more complex querying, set to 0 for now
    new_metrics.gpu_utilization = 0.0f;

    // Collect CPU and memory metrics
    collectCPUMetrics();
    collectMemoryMetrics();

    // Copy current metrics for non-GPU values
    DetailedMetrics current = getCurrentMetrics();
    new_metrics.requests_per_second = current.requests_per_second;
    new_metrics.successful_requests = current.successful_requests;

    // Calculate requests per second
    static std::uint64_t last_request_count = 0;
    static auto last_rps_check = sep::shim::chrono::steady_clock::now();
    auto now = sep::shim::chrono::steady_clock::now();

    auto elapsed = sep::shim::chrono::duration_cast<sep::shim::chrono::seconds>(now - last_rps_check);
    if (elapsed > 0) {
      std::uint64_t current_requests = current.successful_requests;
      std::uint64_t rps = (current_requests - last_request_count) / elapsed;
      new_metrics.requests_per_second = rps;

      last_request_count = current_requests;
      last_rps_check = now;
    }

    // Keep latency metrics from current state since they're updated separately
    new_metrics.p50_latency = current.p50_latency;
    new_metrics.p95_latency = current.p95_latency;
    new_metrics.p99_latency = current.p99_latency;

    // Update the metrics
    updateMetrics(new_metrics);
  }

  void resetMetrics() {
    std::lock_guard<std::mutex> lock(metrics_mutex_);
    current_metrics_ = DetailedMetrics();
  }

 private:
  std::atomic<bool> running_;
  std::vector<std::thread> threads_;
  cudaEvent_t start_event_{nullptr};
  cudaEvent_t stop_event_{nullptr};
  std::deque<std::uint64_t> latency_samples_;
  const size_t latency_window_size_;

  mutable std::mutex metrics_mutex_;
  DetailedMetrics current_metrics_;
};

// MetricsCollector implementation

MetricsCollector::MetricsCollector() : pImpl(std::make_unique<Impl>()) {}

MetricsCollector& MetricsCollector::instance() {
    // Use a function-local static variable for thread-safe singleton initialization
    static MetricsCollector instance;
    return instance;
}

void MetricsCollector::recordOperation(const std::string& operation_name, bool success, double duration_ms) {
    std::lock_guard<std::mutex> lock(metrics_mutex_);
    sep::shim::string key(operation_name.c_str());
    auto& perf = performance_metrics_[key];
    perf.total_operations++;
    if (success) {
        perf.successful_operations++;
    } else {
        perf.failed_operations++;
    }
    
    // Update timing metrics
    perf.average_processing_time_ms =
        (perf.average_processing_time_ms * (perf.total_operations - 1) + duration_ms) / perf.total_operations;
    perf.peak_processing_time_ms = std::max(perf.peak_processing_time_ms, duration_ms);
    perf.last_update = sep::shim::chrono::steady_clock::now();
}

void MetricsCollector::incrementCounter(const std::string& counter_name, std::uint64_t value) {
    std::lock_guard<std::mutex> lock(metrics_mutex_);
    sep::shim::string key(counter_name.c_str());
    counters_[key] += value;
    // Ensure updates are visible across threads.
}

void MetricsCollector::setGauge(const std::string& gauge_name, double value) {
    std::lock_guard<std::mutex> lock(metrics_mutex_);
    sep::shim::string key(gauge_name.c_str());
    gauges_[key] = value;
}

void MetricsCollector::recordAllocation(std::uint64_t bytes) {
    std::lock_guard<std::mutex> lock(metrics_mutex_);
    memory_metrics_.allocated_bytes += bytes;
    memory_metrics_.total_allocations++;
    memory_metrics_.peak_allocated_bytes =
        std::max(memory_metrics_.peak_allocated_bytes, memory_metrics_.allocated_bytes);
}

void MetricsCollector::recordDeallocation(std::uint64_t bytes) {
    std::lock_guard<std::mutex> lock(metrics_mutex_);
    memory_metrics_.allocated_bytes -= bytes;
    memory_metrics_.total_deallocations++;
}

void MetricsCollector::updateSystemMetrics() {
    if (pImpl) {
        pImpl->collectMetrics();
        auto detailed = pImpl->getCurrentMetrics();
        
        std::lock_guard<std::mutex> lock(metrics_mutex_);
        // cast to double to avoid implicit float-to-double conversion warning
        system_metrics_.cpu_usage_percent = static_cast<double>(detailed.cpu_usage); 
        system_metrics_.memory_usage_bytes = detailed.memory_usage; 
        system_metrics_.gpu_memory_usage_bytes = static_cast<std::uint64_t>(detailed.gpu_memory_usage);
        system_metrics_.gpu_utilization_percent = static_cast<double>(detailed.gpu_utilization);
    }
}

MetricsCollector::PerformanceMetrics MetricsCollector::getPerformanceMetrics(const std::string& operation_name) const {
    std::lock_guard<std::mutex> lock(metrics_mutex_);
    sep::shim::string key(operation_name.c_str());
    auto it = performance_metrics_.find(key);
    if (it != performance_metrics_.end()) {
        return it->second;
    }
    return PerformanceMetrics{};
}

MetricsCollector::MemoryMetrics MetricsCollector::getMemoryMetrics() const {
    std::lock_guard<std::mutex> lock(metrics_mutex_);
    return memory_metrics_;
}

MetricsCollector::SystemMetrics MetricsCollector::getSystemMetrics() const {
    std::lock_guard<std::mutex> lock(metrics_mutex_);
    return system_metrics_;
}

std::uint64_t MetricsCollector::getCounter(const std::string& counter_name) const {
    std::lock_guard<std::mutex> lock(metrics_mutex_);
    sep::shim::string key(counter_name.c_str());
    auto it = counters_.find(key);
    return (it != counters_.end()) ? it->second : 0;
}

double MetricsCollector::getGauge(const std::string& gauge_name) const {
    std::lock_guard<std::mutex> lock(metrics_mutex_);
    sep::shim::string key(gauge_name.c_str());
    auto it = gauges_.find(key);
    return (it != gauges_.end()) ? it->second : 0.0;
}

DetailedMetrics MetricsCollector::getDetailedMetrics() const {
    if (pImpl) {
        return pImpl->getCurrentMetrics();
    }
    return DetailedMetrics{};
}

void MetricsCollector::reset() {
    std::lock_guard<std::mutex> lock(metrics_mutex_);
    performance_metrics_.clear();
    counters_.clear();  
    gauges_.clear(); 
    memory_metrics_ = MemoryMetrics{};
    system_metrics_ = SystemMetrics{};
    if (pImpl) {
        pImpl->resetMetrics();
    }
}

void MetricsCollector::resetOperation(const std::string& operation_name) {
    std::lock_guard<std::mutex> lock(metrics_mutex_);
    sep::shim::string key(operation_name.c_str());
    performance_metrics_.erase(key);
}

}  // namespace sep::metrics

