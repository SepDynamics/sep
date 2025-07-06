#include "api/client.h"
#include "api/server.h"
#include "api/types.h"
#include "compat/types.h"
#ifdef BOOST_ASIO_DISABLE_RTTI
#include "crow/asio_isolation.h"
#else
#include <asio.hpp>
#endif
#include <atomic>
#include <chrono>
#include <fcntl.h>
#include <gtest/gtest.h>
#include <netinet/in.h>
#include <set>
#include <signal.h>
#include <spdlog/spdlog.h>
#include "logging/manager.h"
#include "blender/cycles_renderer.h"
#include <sys/socket.h>
#include <sys/types.h>
#include <thread>
#include <unistd.h>

using namespace sep::api;
using namespace std::chrono_literals;

// g_running uses default seq_cst semantics for signal handling
static std::atomic<bool> g_running{true};

void signal_handler(int) { g_running = false; }

// Helper to create a default server configuration
static sep::config::APIConfig createConfig(uint16_t port) {
  sep::config::APIConfig config;
  config.port = port;
  config.threads = 1;
  config.log_level = "info";
  config.keep_alive_timeout_ms = 15000;
  config.enable_metrics = true;
  config.max_batch_size = 100;
  config.cors.enabled = false;
  config.cors.allowed_origins = "*";
  config.cors.allowed_headers = {};
  config.rate_limit.enabled = false;
  config.rate_limit.rpm = 0;
  config.logging.log_requests = false;
  config.logging.log_responses = false;
  config.ollama.host = "";
  config.ollama.model = "";
  return config;
}

// Helper to create a rate-limited server configuration
static sep::config::APIConfig
createRateLimitedConfig(uint16_t port, uint32_t rpm, bool adaptive = false) {
  auto config = createConfig(port);
  config.rate_limit.enabled = true;
  config.rate_limit.rpm = rpm;
  config.rate_limit.adaptive = adaptive;
  config.rate_limit.burst_multiplier = 2.0f;
  config.rate_limit.window_size_ms = 60000;
  return config;
}

static uint16_t requestEphemeralPort() {
  asio::io_context io;
  asio::ip::tcp::acceptor acceptor(io);
  asio::ip::tcp::endpoint endpoint(asio::ip::tcp::v4(), 0);
  acceptor.open(endpoint.protocol());
  acceptor.bind(endpoint);
  uint16_t port = acceptor.local_endpoint().port();
  acceptor.close();
  return port;
}

class ServerTest : public ::testing::Test {
protected:
  std::set<uint16_t> used_ports_;
  std::mutex ports_mutex_;
  void SetUp() override {
    sep::logging::initializeLogging();
    // Set up signal handler
    struct sigaction sa;
    sa.sa_handler = signal_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGINT, &sa, nullptr);
    sigaction(SIGTERM, &sa, nullptr);

    g_running = true;
    std::this_thread::sleep_for(std::chrono::seconds(1));
  }

  void TearDown() override {
    g_running = false;

    sep::logging::Manager::shutdown();

    // Wait for all used ports to be released
    std::lock_guard<std::mutex> lock(ports_mutex_);
    for (uint16_t port : used_ports_) {
      waitForPortRelease(port);
    }
    used_ports_.clear();

    std::this_thread::sleep_for(std::chrono::seconds(1));
  }

  uint16_t findAvailablePort() {
    for (int attempts = 0; attempts < 20; ++attempts) {
      uint16_t port = requestEphemeralPort();
      {
        std::lock_guard<std::mutex> lock(ports_mutex);
        if (used_ports.find(port) != used_ports.end()) {
          continue;
        }
        used_ports.insert(port);
      }
      return port;
    }
    throw std::runtime_error("Could not find available port");
  }

  bool isPortAvailable(uint16_t port) {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
      return false;
    }

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = INADDR_ANY;

    int yes = 1;
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) < 0 ||
        setsockopt(sock, SOL_SOCKET, SO_REUSEPORT, &yes, sizeof(yes)) < 0) {
      close(sock);
      return false;
    }

    bool available = bind(sock, (struct sockaddr *)&addr, sizeof(addr)) == 0;
    close(sock);
    return available;
  }

  void waitForPortRelease(uint16_t port) {
    auto start = std::chrono::steady_clock::now();
    auto timeout = std::chrono::seconds(5);

    while (std::chrono::steady_clock::now() - start < timeout) {
      if (isPortAvailable(port)) {
        spdlog::info("Port {} is now available", port);
        return;
      }
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    throw std::runtime_error("Port " + std::to_string(port) +
                             " not released after timeout");
  }

  class ServerGuard {
  public:
    ServerGuard(uint16_t port) : port_(port) {
      config_ = createConfig(port);
      renderer_ = std::make_unique<sep::blender::ccl::CyclesRenderer>();
      server_ = std::make_unique<SEPApiServer>(config_, renderer_.get());
      running_ = std::make_shared<std::atomic<bool>>(true);

      // Start server in a separate thread
      server_thread_ = std::thread([this]() {
        while (running_->load()) {
          try {
            server_->run();
          } catch (const std::exception &e) {
            spdlog::error("Server error: {}", e.what());
            break;
          }
        }
      });

      // Wait for server to start
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    ServerGuard(const sep::config::APIConfig &config)
        : port_(config.port), config_(config) {
      renderer_ = std::make_unique<sep::blender::ccl::CyclesRenderer>();
      server_ = std::make_unique<SEPApiServer>(config_, renderer_.get());
      running_ = std::make_shared<std::atomic<bool>>(true);

      // Start server in a separate thread
      server_thread_ = std::thread([this]() {
        while (running_->load()) {
          try {
            server_->run();
          } catch (const std::exception &e) {
            spdlog::error("Server error: {}", e.what());
            break;
          }
        }
      });

      // Wait for server to start
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    ~ServerGuard() {
      shutdown();

      // Wait for port to be released
      auto start = std::chrono::steady_clock::now();
      while (!isPortAvailable(port_) &&
             std::chrono::steady_clock::now() - start <
                 std::chrono::seconds(5)) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
      }
    }

    void shutdown() {
      if (running_->load()) {
        spdlog::info("Shutting down server on port {}", port_);
        running_->store(false);
        server_->stop();
        server_->waitForShutdown();
        if (server_thread_.joinable()) {
          server_thread_.join();
        }
        spdlog::info("Server shutdown complete");
      }
    }

    bool isPortAvailable(uint16_t port) {
      int sock = socket(AF_INET, SOCK_STREAM, 0);
      if (sock < 0) {
        return false;
      }

      struct sockaddr_in addr;
      addr.sin_family = AF_INET;
      addr.sin_port = htons(port);
      addr.sin_addr.s_addr = INADDR_ANY;

      int yes = 1;
      if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) < 0 ||
          setsockopt(sock, SOL_SOCKET, SO_REUSEPORT, &yes, sizeof(yes)) < 0) {
        close(sock);
        return false;
      }

      bool available = bind(sock, (struct sockaddr *)&addr, sizeof(addr)) == 0;
      close(sock);
      return available;
    }

  private:
    uint16_t port_;
    sep::config::APIConfig config_;
    std::unique_ptr<sep::blender::ccl::CyclesRenderer> renderer_;
    std::unique_ptr<SEPApiServer> server_;
    // running_ flag uses seq_cst semantics
    std::shared_ptr<std::atomic<bool>> running_;
    std::thread server_thread_;
  };
};

// Test that server can start and shut down cleanly
TEST_F(ServerTest, StartupAndShutdown) {
  uint16_t port = findAvailablePort();
  spdlog::info("Starting test with port {}", port);

  try {
    {
      ServerGuard guard(port);
      spdlog::info("Server started on port {}, waiting...", port);
      std::this_thread::sleep_for(std::chrono::milliseconds(500));
      spdlog::info("Initiating server shutdown...");
    } // ServerGuard destructor will handle shutdown

    spdlog::info("Waiting for port {} to be released...", port);
    waitForPortRelease(port);
    spdlog::info("Port {} released successfully", port);

    SUCCEED();
  } catch (const std::exception &e) {
    spdlog::error("StartupAndShutdown test failed: {}", e.what());
    FAIL() << "StartupAndShutdown test failed: " << e.what();
  }
}

// Test that a custom endpoint returns the expected response
TEST_F(ServerTest, PingEndpointReturnsPong) {
  uint16_t port = findAvailablePort();
  spdlog::info("Starting test with port {}", port);

  try {
    ServerGuard guard(port);
    spdlog::info("Server started on port {}, waiting...", port);
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    // Create HTTP client
    ClientConfig client_config;
    client_config.baseUrl = "http://127.0.0.1:" + std::to_string(port);
    client_config.enableMetrics = false;
    auto client = createClient(client_config);

    auto response = client->get("/ping");
    EXPECT_EQ(response.statusCode, 200);
    EXPECT_NE(response.body.find("\"response\":\"pong\""), std::string::npos);

    spdlog::info("Initiating server shutdown...");
  } catch (const std::exception &e) {
    spdlog::error("PingEndpointReturnsPong test failed: {}", e.what());
    FAIL() << "PingEndpointReturnsPong test failed: " << e.what();
  }
}

// Test that requesting an undefined endpoint returns 404
TEST_F(ServerTest, UnknownEndpointReturns404) {
  uint16_t port = findAvailablePort();
  spdlog::info("Starting test with port {}", port);

  try {
    ServerGuard guard(port);
    spdlog::info("Server started on port {}, waiting...", port);
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    // Create HTTP client
    ClientConfig client_config;
    client_config.baseUrl = "http://127.0.0.1:" + std::to_string(port);
    client_config.enableMetrics = false;
    auto client = createClient(client_config);

    auto response = client->get("/notfound");
    EXPECT_EQ(response.statusCode, 404);

    spdlog::info("Initiating server shutdown...");
  } catch (const std::exception &e) {
    spdlog::error("UnknownEndpointReturns404 test failed: {}", e.what());
    FAIL() << "UnknownEndpointReturns404 test failed: " << e.what();
  }
}

// Test basic rate limiting functionality
TEST_F(ServerTest, BasicRateLimiting) {
  uint16_t port = findAvailablePort();
  const uint32_t rpm = 60; // 1 request per second
  auto config = createRateLimitedConfig(port, rpm);

  try {
    ServerGuard guard(config);

    ClientConfig client_config;
    client_config.baseUrl = "http://127.0.0.1:" + std::to_string(port);
    client_config.enableMetrics = true;
    auto client = createClient(client_config);

    // Should succeed - within limit
    for (int i = 0; i < 5; i++) {
      auto response = client->get("/ping");
      EXPECT_EQ(response.statusCode, 200);
      std::this_thread::sleep_for(1s);
    }

    // Should fail - exceeds rate limit
    std::vector<std::future<APIResponse>> futures;
    for (int i = 0; i < 10; i++) {
      futures.push_back(std::async(
          std::launch::async, [&client]() { return client->get("/ping"); }));
    }

    int rejected = 0;
    for (auto &future : futures) {
      auto response = future.get();
      if (response.statusCode == 429) {
        rejected++;
      }
    }

    EXPECT_GT(rejected, 0) << "Expected some requests to be rate limited";

  } catch (const std::exception &e) {
    FAIL() << "BasicRateLimiting test failed: " << e.what();
  }
}

// Test priority-based rate limiting
TEST_F(ServerTest, PriorityBasedRateLimiting) {
  uint16_t port = findAvailablePort();
  const uint32_t rpm = 60;
  auto config = createRateLimitedConfig(port, rpm);

  try {
    ServerGuard guard(config);

    ClientConfig client_config;
    client_config.baseUrl = "http://127.0.0.1:" + std::to_string(port);
    client_config.enableMetrics = true;
    auto client = createClient(client_config);

    // Test different priority levels
    std::vector<Priority> priorities = {Priority::LOW, Priority::NORMAL,
                                        Priority::HIGH, Priority::CRITICAL};

    std::map<Priority, int> success_counts;

    // Send requests with different priorities
    for (const auto &priority : priorities) {
      std::vector<std::future<APIResponse>> futures;
      for (int i = 0; i < 10; i++) {
        futures.push_back(std::async(std::launch::async, [&client, priority]() {
          return client->get("/ping", {}, priority);
        }));
      }

      for (auto &future : futures) {
        auto response = future.get();
        if (response.statusCode == 200) {
          success_counts[priority]++;
        }
      }
    }

    // Higher priority should have more successful requests
    EXPECT_GT(success_counts[Priority::CRITICAL],
              success_counts[Priority::HIGH]);
    EXPECT_GT(success_counts[Priority::HIGH], success_counts[Priority::NORMAL]);
    EXPECT_GT(success_counts[Priority::NORMAL], success_counts[Priority::LOW]);

  } catch (const std::exception &e) {
    FAIL() << "PriorityBasedRateLimiting test failed: " << e.what();
  }
}

// Test adaptive rate limiting
TEST_F(ServerTest, AdaptiveRateLimiting) {
  uint16_t port = findAvailablePort();
  const uint32_t rpm = 120;
  auto config = createRateLimitedConfig(port, rpm, true);

  try {
    ServerGuard guard(config);

    ClientConfig client_config;
    client_config.baseUrl = "http://127.0.0.1:" + std::to_string(port);
    client_config.enableMetrics = true;
    auto client = createClient(client_config);

    // First phase - normal load
    std::vector<APIResponse> responses;
    for (int i = 0; i < 10; i++) {
      responses.push_back(client->get("/ping"));
      EXPECT_EQ(responses.back().statusCode, 200);
      std::this_thread::sleep_for(100ms);
    }

    // Second phase - high load to trigger adaptation
    std::vector<std::future<APIResponse>> futures;
    for (int i = 0; i < 50; i++) {
      futures.push_back(std::async(
          std::launch::async, [&client]() { return client->get("/ping"); }));
    }

    int rejected = 0;
    for (auto &future : futures) {
      auto response = future.get();
      if (response.statusCode == 429) {
        rejected++;
      }
    }

    // Should see some rejections due to adaptive rate limiting
    EXPECT_GT(rejected, 0);

    // Third phase - verify system recovers
    std::this_thread::sleep_for(5s);
    auto response = client->get("/ping");
    EXPECT_EQ(response.statusCode, 200);

  } catch (const std::exception &e) {
    FAIL() << "AdaptiveRateLimiting test failed: " << e.what();
  }
}

// Test rate limit bursting behavior
TEST_F(ServerTest, BurstingBehavior) {
  uint16_t port = findAvailablePort();
  const uint32_t rpm = 60;
  auto config = createRateLimitedConfig(port, rpm);
  config.rate_limit.burst_multiplier = 3.0f; // Allow 3x burst

  try {
    ServerGuard guard(config);

    ClientConfig client_config;
    client_config.baseUrl = "http://127.0.0.1:" + std::to_string(port);
    client_config.enableMetrics = true;
    auto client = createClient(client_config);

    // Should allow burst of requests
    std::vector<std::future<APIResponse>> futures;
    for (int i = 0; i < 3; i++) {
      futures.push_back(std::async(
          std::launch::async, [&client]() { return client->get("/ping"); }));
    }

    // All burst requests should succeed
    for (auto &future : futures) {
      auto response = future.get();
      EXPECT_EQ(response.statusCode, 200);
    }

    // Additional requests should be rate limited
    auto response = client->get("/ping");
    EXPECT_EQ(response.statusCode, 429);

    // After waiting, should accept requests again
    std::this_thread::sleep_for(3s);
    response = client->get("/ping");
    EXPECT_EQ(response.statusCode, 200);

  } catch (const std::exception &e) {
    FAIL() << "BurstingBehavior test failed: " << e.what();
  }
}
