#pragma once

#ifdef SEP_USE_TBB
#  include <tbb/concurrent_hash_map.h>
#else
#  include "api/rate_limiter.h"
#endif

#include <mutex>
#include <thread>
#include <array>
#include <atomic>
#include <chrono>
#include <memory>
#include <unordered_map>

namespace sep::api {

// Forward declarations
class BackgroundCleanup;

// Internal metrics with atomic values
struct AtomicSystemMetrics {
  std::atomic<float> gpu_utilization{0.0f};
  std::atomic<float> memory_usage{0.0f};
  std::atomic<float> error_rate{0.0f};
  std::atomic<float> avg_latency{0.0f};
};

// External metrics for API use
struct SystemMetrics {
  float gpu_utilization{0.0f};
  float memory_usage{0.0f};
  float error_rate{0.0f};
  float avg_latency{0.0f};
};

struct WindowEntry {
  std::atomic<uint64_t> timestamp{0};
  std::atomic<uint32_t> count{0};
  std::atomic<uint8_t> priority{0};
};

class LockFreeRateLimiter : public IRateLimiter {
public:
  explicit LockFreeRateLimiter(unsigned int requests_per_minute);
  ~LockFreeRateLimiter() override;

  // Core rate limiting interface
  bool checkRateLimit(const IRequest &req) override;
  std::string getErrorResponse(const std::string &message, int status) override;

  // Configuration methods
  void setEnabled(bool enabled) override;
  void setPriorityQuota(Priority priority, float multiplier) override;

  // Monitoring methods
  unsigned int GetRequestCount(const std::string &client_id) const override;
  unsigned int GetWindowSize(const std::string &client_id,
                             Priority priority) const override;

  // System metrics methods
  void updateSystemMetrics(const SystemMetrics &metrics);
  SystemMetrics getSystemMetrics() const;

private:
  // Window management
  static constexpr size_t WINDOW_SIZE = 1024; // Power of 2 for efficient modulo
  static constexpr auto CLEANUP_INTERVAL = std::chrono::seconds(1);
  static constexpr auto METRICS_UPDATE_INTERVAL = std::chrono::seconds(5);

  // Adaptive rate limiting thresholds
  static constexpr float HIGH_LOAD_THRESHOLD = 0.8f;   // 80% GPU/memory usage
  static constexpr float ERROR_RATE_THRESHOLD = 0.05f; // 5% error rate
  static constexpr float LATENCY_THRESHOLD = 100.0f;   // 100ms
  static constexpr float MIN_RATE_MULTIPLIER =
      0.5f; // Minimum rate limit multiplier
  static constexpr float MAX_RATE_MULTIPLIER =
      2.0f; // Maximum rate limit multiplier

  struct ClientWindow {
    std::array<WindowEntry, WINDOW_SIZE> entries;
    std::atomic<size_t> head{0};
    std::atomic<size_t> tail{0};
  };

  // Efficient client tracking
  struct alignas(64) ClientData { // Prevent false sharing
    ClientWindow window;
    std::atomic<uint32_t> request_count{0};
  };

  // Helper methods
  void cleanup(std::chrono::steady_clock::time_point now);
  unsigned int getAdjustedLimit(Priority priority) const;
  Priority getPriorityFromRequest(const IRequest &req) const;
  std::string getClientId(const IRequest &req) const;
  float calculateAdaptiveMultiplier() const;

  // Atomic operations
  bool tryInsertRequest(ClientData &client, Priority priority,
                        std::chrono::steady_clock::time_point now);

  void removeExpiredEntries(ClientData &client,
                            std::chrono::steady_clock::time_point now);

  // Member variables
  const unsigned int max_requests_;
  std::atomic<bool> enabled_{true};
  std::atomic<std::chrono::steady_clock::time_point> last_cleanup_{};
  std::atomic<float> adaptive_multiplier_{1.0f};

  // System metrics
  AtomicSystemMetrics metrics_;
  std::unique_ptr<BackgroundCleanup> metrics_collector_;

  // Priority configuration
  struct PriorityConfig {
    std::atomic<float> multiplier;
    explicit PriorityConfig(float m) : multiplier(m) {}
  };

  // Initialize priority configs with their respective multipliers
  std::array<PriorityConfig, 4> priority_configs_{
      PriorityConfig{0.5f}, // LOW
      PriorityConfig{1.0f}, // NORMAL
      PriorityConfig{2.0f}, // HIGH
      PriorityConfig{5.0f}  // CRITICAL
  };

  // Client management container
#ifdef SEP_USE_TBB
  using ClientMap =
      tbb::concurrent_hash_map<std::string, std::unique_ptr<ClientData>>;
  ClientMap clients_;
#else
  using ClientMap = std::unordered_map<std::string, std::unique_ptr<ClientData>>;
  ClientMap clients_;
  mutable std::mutex clients_mutex_;
#endif

  // Background cleanup
  std::unique_ptr<BackgroundCleanup> background_cleanup_;
};

// Factory function
std::unique_ptr<IRateLimiter>
createLockFreeRateLimiter(unsigned int requests_per_minute);

} // namespace sep::api
