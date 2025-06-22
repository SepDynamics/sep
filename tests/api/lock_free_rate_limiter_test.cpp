#include "api/lock_free_rate_limiter.h"
#include <future>
#include <gtest/gtest.h>
#include <thread>

using namespace sep::api;
using namespace std::chrono_literals;

// Mock request class that implements IRequest
class MockRequest : public IRequest {
public:
  explicit MockRequest(std::string client_id = "", std::string priority = "")
      : client_id_(std::move(client_id)), priority_(std::move(priority)) {}

  std::string get_header_value(const std::string &key) const override {
    if (key == "X-Client-ID")
      return client_id_;
    if (key == "X-Request-Priority")
      return priority_;
    return "";
  }

  const std::string &get_remote_ip() const override {
    return remote_ip_address;
  }

  std::string remote_ip_address = "127.0.0.1";

private:
  std::string client_id_;
  std::string priority_;
};

class LockFreeRateLimiterTest : public ::testing::Test {
protected:
  void SetUp() override {
    limiter = createLockFreeRateLimiter(defaultMaxRequests);
  }

  const unsigned int defaultMaxRequests = 1000;
  std::unique_ptr<IRateLimiter> limiter;
};

TEST_F(LockFreeRateLimiterTest, BasicRateLimiting) {
  MockRequest req("test_client");

  // Should allow up to limit
  for (unsigned int i = 0; i < defaultMaxRequests; ++i) {
    EXPECT_TRUE(limiter->checkRateLimit(req))
        << "Request " << i + 1 << " should be allowed";
  }

  // Should block after limit
  EXPECT_FALSE(limiter->checkRateLimit(req))
      << "Request exceeding limit should be blocked";
}

TEST_F(LockFreeRateLimiterTest, PriorityBehavior) {
  struct TestCase {
    std::string priority;
    float multiplier;
  };

  std::vector<TestCase> testCases = {
      {"LOW", 0.5f}, {"NORMAL", 1.0f}, {"HIGH", 2.0f}, {"CRITICAL", 5.0f}};

  for (const auto &tc : testCases) {
    MockRequest req("test_client", tc.priority);
    auto expectedLimit =
        static_cast<unsigned int>(defaultMaxRequests * tc.multiplier);

    // Should allow up to adjusted limit
    for (unsigned int i = 0; i < expectedLimit; ++i) {
      EXPECT_TRUE(limiter->checkRateLimit(req))
          << "Request " << i + 1 << " should be allowed for priority "
          << tc.priority;
    }

    // Should block after limit
    EXPECT_FALSE(limiter->checkRateLimit(req))
        << "Request exceeding limit should be blocked for priority "
        << tc.priority;
  }
}

TEST_F(LockFreeRateLimiterTest, ConcurrentAccess) {
  const int numThreads = 10;
  const int requestsPerThread = 1000;
  std::atomic<int> totalAllowed{0};
  std::atomic<bool> startFlag{false};
  std::vector<std::future<void>> futures;

  // Launch multiple threads making requests
  for (int i = 0; i < numThreads; ++i) {
    futures.push_back(
        std::async(std::launch::async, [this, &totalAllowed, &startFlag]() {
          MockRequest req("concurrent_client");

          // Wait for all threads to be ready
          while (!startFlag.load(std::memory_order_acquire)) {
            std::this_thread::yield();
          }

          for (int j = 0; j < requestsPerThread; ++j) {
            if (limiter->checkRateLimit(req)) {
              totalAllowed.fetch_add(1, std::memory_order_relaxed);
            }
            std::this_thread::sleep_for(100us);
          }
        }));
  }

  // Start all threads simultaneously
  startFlag.store(true, std::memory_order_release);

  // Wait for all threads to complete
  for (auto &future : futures) {
    future.get();
  }

  // Total allowed should not exceed limit
  EXPECT_LE(totalAllowed, defaultMaxRequests)
      << "Total allowed requests should not exceed the limit";
}

TEST_F(LockFreeRateLimiterTest, WindowExpiration) {
  MockRequest req("window_client");

  // Fill up half the limit
  const unsigned int initialRequests = defaultMaxRequests / 2;
  for (unsigned int i = 0; i < initialRequests; ++i) {
    EXPECT_TRUE(limiter->checkRateLimit(req));
  }

  // Wait for window to expire
  std::this_thread::sleep_for(61s);

  // Should allow full limit again
  for (unsigned int i = 0; i < defaultMaxRequests; ++i) {
    EXPECT_TRUE(limiter->checkRateLimit(req))
        << "Request " << i + 1 << " should be allowed after window expiration";
  }

  EXPECT_FALSE(limiter->checkRateLimit(req))
      << "Request exceeding limit should be blocked after window expiration";
}

TEST_F(LockFreeRateLimiterTest, MultipleClients) {
  std::vector<std::string> clients = {"client1", "client2", "client3"};

  // Each client should have its own quota
  for (const auto &client : clients) {
    MockRequest req(client);

    for (unsigned int i = 0; i < defaultMaxRequests; ++i) {
      EXPECT_TRUE(limiter->checkRateLimit(req))
          << "Request " << i + 1 << " for " << client << " should be allowed";
    }

    EXPECT_FALSE(limiter->checkRateLimit(req))
        << "Request exceeding limit for " << client << " should be blocked";

    // Verify request count
    EXPECT_EQ(limiter->GetRequestCount(client), defaultMaxRequests + 1)
        << "Request count mismatch for " << client;
  }
}

TEST_F(LockFreeRateLimiterTest, DynamicPriorityAdjustment) {
  MockRequest req("test_client", "HIGH");
  const float newMultiplier = 3.0f;

  // Test initial quota (2.0x)
  auto initialLimit = static_cast<unsigned int>(defaultMaxRequests * 2.0f);
  for (unsigned int i = 0; i < initialLimit; ++i) {
    EXPECT_TRUE(limiter->checkRateLimit(req));
  }
  EXPECT_FALSE(limiter->checkRateLimit(req));

  // Adjust quota
  limiter->setPriorityQuota(Priority::HIGH, newMultiplier);

  // Wait for window to expire
  std::this_thread::sleep_for(61s);

  // Test new quota
  auto newLimit = static_cast<unsigned int>(defaultMaxRequests * newMultiplier);
  for (unsigned int i = 0; i < newLimit; ++i) {
    EXPECT_TRUE(limiter->checkRateLimit(req))
        << "Request " << i + 1 << " should be allowed with new quota";
  }
  EXPECT_FALSE(limiter->checkRateLimit(req))
      << "Request exceeding new limit should be blocked";
}

TEST_F(LockFreeRateLimiterTest, EnableDisable) {
  MockRequest req("test_client");

  // Initially enabled
  EXPECT_TRUE(limiter->checkRateLimit(req));

  // Disable rate limiting
  limiter->setEnabled(false);
  for (unsigned int i = 0; i < defaultMaxRequests * 2; ++i) {
    EXPECT_TRUE(limiter->checkRateLimit(req))
        << "Request should be allowed when disabled";
  }

  // Re-enable rate limiting
  limiter->setEnabled(true);
  for (unsigned int i = 0; i < defaultMaxRequests; ++i) {
    EXPECT_TRUE(limiter->checkRateLimit(req));
  }
  EXPECT_FALSE(limiter->checkRateLimit(req))
      << "Request exceeding limit should be blocked when re-enabled";
}

// New tests for adaptive rate limiting

TEST_F(LockFreeRateLimiterTest, SystemMetricsUpdateAndRetrieval) {
  auto *lockFreeRateLimiter =
      static_cast<LockFreeRateLimiter *>(limiter.get());
  ASSERT_NE(lockFreeRateLimiter, nullptr);

  SystemMetrics metrics;
  metrics.gpu_utilization = 0.75f;
  metrics.memory_usage = 0.85f;
  metrics.error_rate = 0.02f;
  metrics.avg_latency = 50.0f;

  lockFreeRateLimiter->updateSystemMetrics(metrics);
  auto retrieved = lockFreeRateLimiter->getSystemMetrics();

  EXPECT_FLOAT_EQ(retrieved.gpu_utilization, 0.75f);
  EXPECT_FLOAT_EQ(retrieved.memory_usage, 0.85f);
  EXPECT_FLOAT_EQ(retrieved.error_rate, 0.02f);
  EXPECT_FLOAT_EQ(retrieved.avg_latency, 50.0f);
}

TEST_F(LockFreeRateLimiterTest, AdaptiveRateLimitingUnderLoad) {
  auto *lockFreeRateLimiter =
      static_cast<LockFreeRateLimiter *>(limiter.get());
  ASSERT_NE(lockFreeRateLimiter, nullptr);

  MockRequest req("test_client");

  // Test under normal conditions
  SystemMetrics normalMetrics;
  normalMetrics.gpu_utilization = 0.5f;
  normalMetrics.memory_usage = 0.5f;
  normalMetrics.error_rate = 0.01f;
  normalMetrics.avg_latency = 50.0f;
  lockFreeRateLimiter->updateSystemMetrics(normalMetrics);

  unsigned int normalAllowed = 0;
  for (unsigned int i = 0; i < defaultMaxRequests * 2; ++i) {
    if (limiter->checkRateLimit(req)) {
      normalAllowed++;
    }
  }

  // Test under high load
  SystemMetrics highLoadMetrics;
  highLoadMetrics.gpu_utilization = 0.9f;
  highLoadMetrics.memory_usage = 0.95f;
  highLoadMetrics.error_rate = 0.1f;
  highLoadMetrics.avg_latency = 200.0f;
  lockFreeRateLimiter->updateSystemMetrics(highLoadMetrics);

  // Wait for window to expire
  std::this_thread::sleep_for(61s);

  unsigned int highLoadAllowed = 0;
  for (unsigned int i = 0; i < defaultMaxRequests * 2; ++i) {
    if (limiter->checkRateLimit(req)) {
      highLoadAllowed++;
    }
  }

  // Under high load, should allow fewer requests
  EXPECT_GT(normalAllowed, highLoadAllowed)
      << "Rate limiter should be more restrictive under high load";
}

TEST_F(LockFreeRateLimiterTest, ConcurrentMetricsUpdate) {
  auto *lockFreeRateLimiter =
      static_cast<LockFreeRateLimiter *>(limiter.get());
  ASSERT_NE(lockFreeRateLimiter, nullptr);

  const int numThreads = 5;
  std::atomic<bool> startFlag{false};
  std::vector<std::future<void>> futures;

  // Launch request threads
  for (int i = 0; i < numThreads; ++i) {
    futures.push_back(std::async(std::launch::async, [this, &startFlag]() {
      MockRequest req("concurrent_client");

      while (!startFlag.load(std::memory_order_acquire)) {
        std::this_thread::yield();
      }

      for (int j = 0; j < 1000; ++j) {
        limiter->checkRateLimit(req);
        std::this_thread::sleep_for(100us);
      }
    }));
  }

  // Launch metrics update thread
  futures.push_back(
      std::async(std::launch::async, [lockFreeRateLimiter, &startFlag]() {
        while (!startFlag.load(std::memory_order_acquire)) {
          std::this_thread::yield();
        }

        for (int i = 0; i < 100; ++i) {
          SystemMetrics metrics;
          metrics.gpu_utilization = static_cast<float>(i % 100) / 100.0f;
          metrics.memory_usage = static_cast<float>((i + 30) % 100) / 100.0f;
          metrics.error_rate = static_cast<float>(i % 10) / 100.0f;
          metrics.avg_latency = static_cast<float>(50 + i);
          lockFreeRateLimiter->updateSystemMetrics(metrics);
          std::this_thread::sleep_for(10ms);
        }
      }));

  // Start all threads
  startFlag.store(true, std::memory_order_release);

  // Wait for completion
  for (auto &future : futures) {
    future.get();
  }

  // Verify final metrics are within valid ranges
  auto finalMetrics = lockFreeRateLimiter->getSystemMetrics();
  EXPECT_GE(finalMetrics.gpu_utilization, 0.0f);
  EXPECT_LE(finalMetrics.gpu_utilization, 1.0f);
  EXPECT_GE(finalMetrics.memory_usage, 0.0f);
  EXPECT_LE(finalMetrics.memory_usage, 1.0f);
  EXPECT_GE(finalMetrics.error_rate, 0.0f);
  EXPECT_LE(finalMetrics.error_rate, 0.1f);
  EXPECT_GT(finalMetrics.avg_latency, 0.0f);
}

TEST_F(LockFreeRateLimiterTest, AdaptiveRateLimitingWithPriorities) {
  auto *lockFreeRateLimiter =
      static_cast<LockFreeRateLimiter *>(limiter.get());
  ASSERT_NE(lockFreeRateLimiter, nullptr);

  // Set high load conditions
  SystemMetrics highLoadMetrics{};
  highLoadMetrics.gpu_utilization = 0.9f;
  highLoadMetrics.memory_usage = 0.9f;
  highLoadMetrics.error_rate = 0.08f;
  highLoadMetrics.avg_latency = 150.0f;
  lockFreeRateLimiter->updateSystemMetrics(highLoadMetrics);

  std::vector<std::string> priorities = {"LOW", "NORMAL", "HIGH", "CRITICAL"};
  std::map<std::string, unsigned int> allowedRequests;

  // Test each priority under high load
  for (const auto &priority : priorities) {
    MockRequest req("test_client", priority);
    unsigned int allowed = 0;

    for (unsigned int i = 0; i < defaultMaxRequests * 5; ++i) {
      if (limiter->checkRateLimit(req)) {
        allowed++;
      }
    }
    allowedRequests[priority] = allowed;
  }

  // Verify priority ordering is maintained under load
  EXPECT_GT(allowedRequests["CRITICAL"], allowedRequests["HIGH"])
      << "Critical priority should allow more requests than High priority "
         "under load";
  EXPECT_GT(allowedRequests["HIGH"], allowedRequests["NORMAL"])
      << "High priority should allow more requests than Normal priority under "
         "load";
  EXPECT_GT(allowedRequests["NORMAL"], allowedRequests["LOW"])
      << "Normal priority should allow more requests than Low priority under "
         "load";
}
