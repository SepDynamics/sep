#include "api/lock_free_rate_limiter.h"
#include "api/background_cleanup.h"

#include <nlohmann/json.hpp>

namespace sep::api {

LockFreeRateLimiter::LockFreeRateLimiter(unsigned int requests_per_minute)
    : max_requests_(requests_per_minute),
      last_cleanup_(std::chrono::steady_clock::now()) {

  // Initialize background cleanup service
  background_cleanup_ = std::make_unique<BackgroundCleanup>(
      CLEANUP_INTERVAL, [this](auto now) { cleanup(now); });

  // Initialize metrics collector
  metrics_collector_ = std::make_unique<BackgroundCleanup>(
      METRICS_UPDATE_INTERVAL, [this](const auto& now) {
        (void)now;
        metrics_mutex_.lock();
        adaptive_multiplier_.store(calculateAdaptiveMultiplier(),
                                   std::memory_order_release);
      });
}

LockFreeRateLimiter::~LockFreeRateLimiter() = default;

bool LockFreeRateLimiter::checkRateLimit(const IRequest &req) {
  if (!enabled_) {
    return true;
  }

  const auto now = std::chrono::steady_clock::now();
  const auto client_id = getClientId(req);
  const auto priority = getPriorityFromRequest(req);

  // Get or create client data
#ifdef SEP_USE_TBB
  ClientMap::accessor accessor;
  if (clients_.insert(accessor, client_id)) {
    accessor->second = std::make_unique<ClientData>();
  }
  return tryInsertRequest(*(accessor->second), priority, now);
#else
  std::unique_lock<std::mutex> lock(clients_mutex_);
  auto &ptr = clients_[client_id];
  if (!ptr) {
    ptr = std::make_unique<ClientData>();
  }
  ClientData &ref = *ptr;
  lock.unlock();
  return tryInsertRequest(ref, priority, now);
#endif
}

bool LockFreeRateLimiter::tryInsertRequest(
    ClientData &client, Priority priority,
    std::chrono::steady_clock::time_point now) {
  auto &window = client.window;
  const auto effective_limit = getAdjustedLimit(priority);

  // Try to insert the request
  size_t current_size = window.tail.load(std::memory_order_acquire) -
                        window.head.load(std::memory_order_acquire);

  if (current_size >= effective_limit) {
    // Remove expired entries before rejecting
    removeExpiredEntries(client, now);

    // Recheck after cleanup
    current_size = window.tail.load(std::memory_order_acquire) -
                   window.head.load(std::memory_order_acquire);
    if (current_size >= effective_limit) {
      return false;
    }
  }

  // Insert new request
  size_t tail = window.tail.load(std::memory_order_relaxed);
  size_t new_tail = (tail + 1) % WINDOW_SIZE;

  // Check for buffer wrap-around
  if (new_tail == window.head.load(std::memory_order_acquire)) {
    removeExpiredEntries(client, now);
    tail = window.tail.load(std::memory_order_relaxed);
    new_tail = (tail + 1) % WINDOW_SIZE;
  }

  // Store request
  auto &entry = window.entries[tail];
  entry.timestamp.store(std::chrono::duration_cast<std::chrono::milliseconds>(
                            now.time_since_epoch())
                            .count(),
                        std::memory_order_release);
  entry.priority.store(static_cast<uint8_t>(priority),
                       std::memory_order_release);
  entry.count.store(1, std::memory_order_release);

  window.tail.store(new_tail, std::memory_order_release);
  client.request_count.fetch_add(1, std::memory_order_relaxed);

  return true;
}

void LockFreeRateLimiter::removeExpiredEntries(
    ClientData &client, std::chrono::steady_clock::time_point now) {
  auto &window = client.window;
  const auto window_duration = std::chrono::minutes(1);
  const auto cutoff = static_cast<uint64_t>(
      std::chrono::duration_cast<std::chrono::milliseconds>(
          (now - window_duration).time_since_epoch())
          .count());

  size_t head = window.head.load(std::memory_order_acquire);
  const size_t tail = window.tail.load(std::memory_order_acquire);

  while (head != tail) {
    const auto &entry = window.entries[head];
    if (entry.timestamp.load(std::memory_order_acquire) <= cutoff) {
      head = (head + 1) % WINDOW_SIZE;
    } else {
      break;
    }
  }

  window.head.store(head, std::memory_order_release);
}

void LockFreeRateLimiter::cleanup(std::chrono::steady_clock::time_point now) {
  // Cleanup is now handled by background thread
#ifdef SEP_USE_TBB
  for (auto it = clients_.begin(); it != clients_.end(); ++it) {
    removeExpiredEntries(*it->second, now);
  }
#else
  std::lock_guard<std::mutex> lock(clients_mutex_);
  for (auto &pair : clients_) {
    removeExpiredEntries(*pair.second, now);
  }
#endif
}

std::string LockFreeRateLimiter::getErrorResponse(const std::string &message,
                                                  int status) {
  nlohmann::json error;
  error["error"] = message;
  error["status"] = status;
  return error.dump();
}

void LockFreeRateLimiter::setEnabled(bool enabled) {
  enabled_.store(enabled, std::memory_order_release);
}

void LockFreeRateLimiter::setPriorityQuota(Priority priority,
                                           float multiplier) {
  if (priority >= Priority::LOW && priority <= Priority::CRITICAL) {
    priority_configs_[static_cast<size_t>(priority)].multiplier.store(
        multiplier, std::memory_order_release);
  }
}

unsigned int
LockFreeRateLimiter::GetRequestCount(const std::string &client_id) const {
#ifdef SEP_USE_TBB
  ClientMap::const_accessor acc;
  if (clients_.find(acc, client_id)) {
    return acc->second->request_count.load(std::memory_order_acquire);
  }
  return 0;
#else
  std::lock_guard<std::mutex> lock(clients_mutex_);
  auto it = clients_.find(client_id);
  if (it != clients_.end()) {
    return it->second->request_count.load(std::memory_order_acquire);
  }
  return 0;
#endif
}

unsigned int LockFreeRateLimiter::GetWindowSize(const std::string &client_id,
                                                Priority priority) const {
#ifdef SEP_USE_TBB
  ClientMap::const_accessor acc;
  if (clients_.find(acc, client_id)) {
    const auto &window = acc->second->window;
    size_t count = 0;

    const size_t head = window.head.load(std::memory_order_acquire);
    const size_t tail = window.tail.load(std::memory_order_acquire);

    size_t current = head;
    while (current != tail) {
      const auto &entry = window.entries[current];
      if (entry.priority.load(std::memory_order_acquire) ==
          static_cast<uint8_t>(priority)) {
        count += entry.count.load(std::memory_order_acquire);
      }
      current = (current + 1) % WINDOW_SIZE;
    }

    return count;
  }
  return 0;
#else
  std::lock_guard<std::mutex> lock(clients_mutex_);
  auto it = clients_.find(client_id);
  if (it != clients_.end()) {
    const auto &window = it->second->window;
    size_t count = 0;
    const size_t head = window.head.load(std::memory_order_acquire);
    const size_t tail = window.tail.load(std::memory_order_acquire);
    size_t current = head;
    while (current != tail) {
      const auto &entry = window.entries[current];
      if (entry.priority.load(std::memory_order_acquire) ==
          static_cast<uint8_t>(priority)) {
        count += entry.count.load(std::memory_order_acquire);
      }
      current = (current + 1) % WINDOW_SIZE;
    }
    return count;
  }
  return 0;
#endif
}

unsigned int LockFreeRateLimiter::getAdjustedLimit(Priority priority) const {
  if (priority >= Priority::LOW && priority <= Priority::CRITICAL) {
    const float base_multiplier =
        priority_configs_[static_cast<size_t>(priority)].multiplier.load(
            std::memory_order_acquire);
    const float adaptive_mult =
        adaptive_multiplier_.load(std::memory_order_acquire);
    return static_cast<unsigned int>(max_requests_ * base_multiplier *
                                     adaptive_mult);
  }
  return max_requests_;
}

float LockFreeRateLimiter::calculateAdaptiveMultiplier() const {
  float multiplier = 1.0f;

  // Adjust based on GPU utilization
  const float gpu_util =
      metrics_.gpu_utilization.load(std::memory_order_acquire);
  if (gpu_util > HIGH_LOAD_THRESHOLD) {
    multiplier *= (1.0f - ((gpu_util - HIGH_LOAD_THRESHOLD) /
                           (1.0f - HIGH_LOAD_THRESHOLD)));
  }

  // Adjust based on memory usage
  const float mem_usage = metrics_.memory_usage.load(std::memory_order_acquire);
  if (mem_usage > HIGH_LOAD_THRESHOLD) {
    multiplier *= (1.0f - ((mem_usage - HIGH_LOAD_THRESHOLD) /
                           (1.0f - HIGH_LOAD_THRESHOLD)));
  }

  // Adjust based on error rate
  const float error_rate = metrics_.error_rate.load(std::memory_order_acquire);
  if (error_rate > ERROR_RATE_THRESHOLD) {
    multiplier *= (1.0f - (error_rate / ERROR_RATE_THRESHOLD));
  }

  // Adjust based on latency
  const float avg_latency =
      metrics_.avg_latency.load(std::memory_order_acquire);
  if (avg_latency > LATENCY_THRESHOLD) {
    multiplier *= (LATENCY_THRESHOLD / avg_latency);
  }

  // Clamp the final multiplier
  return std::clamp(multiplier, MIN_RATE_MULTIPLIER, MAX_RATE_MULTIPLIER);
}

void LockFreeRateLimiter::updateSystemMetrics(const SystemMetrics &metrics) {
  metrics_.gpu_utilization.store(metrics.gpu_utilization,
                                 std::memory_order_release);
  metrics_.memory_usage.store(metrics.memory_usage, std::memory_order_release);
  metrics_.error_rate.store(metrics.error_rate, std::memory_order_release);
  metrics_.avg_latency.store(metrics.avg_latency, std::memory_order_release);
}

SystemMetrics LockFreeRateLimiter::getSystemMetrics() const {
  return SystemMetrics{metrics_.gpu_utilization.load(std::memory_order_acquire),
                       metrics_.memory_usage.load(std::memory_order_acquire),
                       metrics_.error_rate.load(std::memory_order_acquire),
                       metrics_.avg_latency.load(std::memory_order_acquire)};
}

Priority
LockFreeRateLimiter::getPriorityFromRequest(const IRequest &req) const {
  auto priorityHeader = req.get_header_value("X-Request-Priority");
  if (priorityHeader == "LOW")
    return Priority::LOW;
  if (priorityHeader == "HIGH")
    return Priority::HIGH;
  if (priorityHeader == "CRITICAL")
    return Priority::CRITICAL;
  return Priority::NORMAL;
}

std::string LockFreeRateLimiter::getClientId(const IRequest &req) const {
  auto clientId = req.get_header_value("X-Client-ID");
  return clientId.empty() ? req.get_remote_ip() : clientId;
}

std::unique_ptr<IRateLimiter>
createLockFreeRateLimiter(unsigned int requests_per_minute) {
  return std::make_unique<LockFreeRateLimiter>(requests_per_minute);
}

std::unique_ptr<IRateLimiter>
createRateLimiter(unsigned int requests_per_minute) { // Fix: Missing definition for createRateLimiter
  return createLockFreeRateLimiter(requests_per_minute);
}

} // namespace sep::api
