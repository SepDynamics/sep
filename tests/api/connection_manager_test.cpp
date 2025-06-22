#include "api/connection_manager.h"
#include "mock_connection_manager.h"
#include <atomic>
#include <future>
#include <gtest/gtest.h>
#include <mutex>
#include <thread>

using namespace sep::api;
using namespace sep::api::test;
using namespace std::chrono_literals;

class ConnectionManagerTest : public ::testing::Test {
protected:
  void SetUp() override {
    manager = std::make_unique<MockConnectionManager>();
    config.maxConnections = 100;
    config.maxConnectionsPerHost = 10;
    config.connectionTimeout = 5000;
    config.keepAliveInterval = 30000;
    config.idleTimeout = 300000;
    config.reuseConnections = true;
    config.validateConnections = true;
    config.validateSSL = true;
    config.sslCertPath = "/path/to/cert.pem";
  }

  ConnectionConfig config;
  std::unique_ptr<MockConnectionManager> manager;
};

TEST_F(ConnectionManagerTest, InitializeReturnsOk) {
  EXPECT_EQ(Status::OK, manager->initialize(config));
}

TEST_F(ConnectionManagerTest, GetConnectionReturnsNullptr) {
  auto socket = manager->getConnection("127.0.0.1", 80);
  EXPECT_EQ(nullptr, socket);
}

TEST_F(ConnectionManagerTest, ReleaseNullConnection) {
  EXPECT_EQ(Status::OK, manager->releaseConnection(nullptr));
}

TEST_F(ConnectionManagerTest, CloseAllReturnsOk) {
  EXPECT_EQ(Status::OK, manager->closeAll());
}

TEST_F(ConnectionManagerTest, GetStatsReturnsEmptyJson) {
  EXPECT_EQ("{}", manager->getStats());
}

TEST_F(ConnectionManagerTest, CustomConnectionHandling) {
  auto mockSocket = std::make_shared<MockSocket>();

  manager->SetGetConnectionHandler(
      [mockSocket](const std::string &host,
                   uint16_t port) -> std::shared_ptr<ISocket> {
        if (host == "127.0.0.1" && port == 80) {
          return mockSocket;
        }
        return nullptr;
      });

  auto socket1 = manager->getConnection("127.0.0.1", 80);
  EXPECT_EQ(mockSocket, socket1);

  auto socket2 = manager->getConnection("otherhost", 8080);
  EXPECT_EQ(nullptr, socket2);
}

TEST_F(ConnectionManagerTest, ConnectionConfiguration) {
  bool initializeCalled = false;

  manager->SetInitializeHandler(
      [&initializeCalled, this](const ConnectionConfig &cfg) -> Status {
        initializeCalled = true;
        EXPECT_EQ(cfg.maxConnections, config.maxConnections);
        EXPECT_EQ(cfg.maxConnectionsPerHost, config.maxConnectionsPerHost);
        EXPECT_EQ(cfg.connectionTimeout, config.connectionTimeout);
        EXPECT_EQ(cfg.keepAliveInterval, config.keepAliveInterval);
        EXPECT_EQ(cfg.idleTimeout, config.idleTimeout);
        EXPECT_EQ(cfg.reuseConnections, config.reuseConnections);
        EXPECT_EQ(cfg.validateConnections, config.validateConnections);
        EXPECT_EQ(cfg.validateSSL, config.validateSSL);
        EXPECT_EQ(cfg.sslCertPath, config.sslCertPath);
        return Status::OK;
      });

  EXPECT_EQ(Status::OK, manager->initialize(config));
  EXPECT_TRUE(initializeCalled);
}

TEST_F(ConnectionManagerTest, ConnectionReleaseHandling) {
  auto mockSocket = std::make_shared<MockSocket>();
  bool releaseCalled = false;

  manager->SetReleaseConnectionHandler(
      [&releaseCalled, mockSocket](std::shared_ptr<ISocket> socket) -> Status {
        releaseCalled = true;
        EXPECT_EQ(mockSocket, socket);
        return Status::OK;
      });

  EXPECT_EQ(Status::OK, manager->releaseConnection(mockSocket));
  EXPECT_TRUE(releaseCalled);
}

TEST_F(ConnectionManagerTest, CustomStatsHandling) {
  std::string expectedStats =
      R"({"active_connections": 0, "total_connections": 0})";

  manager->SetGetStatsHandler(
      [expectedStats]() -> std::string { return expectedStats; });

  EXPECT_EQ(expectedStats, manager->getStats());
}

TEST_F(ConnectionManagerTest, ConnectionPoolStressTest) {
  const int maxConnections = 10;
  std::atomic<int> activeConnections{0};  // seq_cst
  std::atomic<int> totalConnections{0};  // seq_cst

  config.maxConnections = maxConnections;
  manager->initialize(config);

  manager->SetGetConnectionHandler(
      [&activeConnections, &totalConnections, maxConnections](
          const std::string &, uint16_t) -> std::shared_ptr<ISocket> {
        if (activeConnections >= maxConnections) {
          return nullptr;
        }
        activeConnections++;
        totalConnections++;
        return std::make_shared<MockSocket>();
      });

  manager->SetReleaseConnectionHandler(
      [&activeConnections](std::shared_ptr<ISocket>) -> Status {
        activeConnections--;
        return Status::OK;
      });

  const int numThreads = 20;
  std::vector<std::future<void>> futures;

  for (int i = 0; i < numThreads; ++i) {
    futures.push_back(std::async(std::launch::async, [this]() {
      for (int j = 0; j < 5; ++j) {
        auto conn = manager->getConnection("test", 80);
        if (conn) {
          std::this_thread::sleep_for(10ms);
          manager->releaseConnection(conn);
        }
      }
    }));
  }

  for (auto &future : futures) {
    future.get();
  }

  EXPECT_EQ(0, activeConnections);
  EXPECT_LE(totalConnections, numThreads * 5);
}

TEST_F(ConnectionManagerTest, ConnectionPriorityHandling) {
  std::atomic<int> nextPriority{1};  // seq_cst

  manager->SetGetConnectionHandler(
      [&nextPriority](const std::string &,
                      uint16_t) -> std::shared_ptr<ISocket> {
        auto socket = std::make_shared<MockSocket>();
        std::static_pointer_cast<MockSocket>(socket)->priority = nextPriority++;
        return socket;
      });

  // Get connections in sequence
  auto highPriorityConn =
      std::static_pointer_cast<MockSocket>(manager->getConnection("high", 80));
  auto mediumPriorityConn = std::static_pointer_cast<MockSocket>(
      manager->getConnection("medium", 80));
  auto lowPriorityConn =
      std::static_pointer_cast<MockSocket>(manager->getConnection("low", 80));

  ASSERT_NE(nullptr, highPriorityConn);
  ASSERT_NE(nullptr, mediumPriorityConn);
  ASSERT_NE(nullptr, lowPriorityConn);

  // Verify priorities are assigned in order
  EXPECT_EQ(1, highPriorityConn->priority);
  EXPECT_EQ(2, mediumPriorityConn->priority);
  EXPECT_EQ(3, lowPriorityConn->priority);
}

TEST_F(ConnectionManagerTest, SSLConfiguration) {
  bool initializeCalled = false;

  manager->SetInitializeHandler(
      [&initializeCalled](const ConnectionConfig &cfg) -> Status {
        initializeCalled = true;
        if (cfg.validateSSL && cfg.sslCertPath.empty()) {
          return Status::ERROR;
        }
        return Status::OK;
      });

  EXPECT_EQ(Status::OK, manager->initialize(config));
  EXPECT_TRUE(initializeCalled);

  // Test with invalid cert path
  config.sslCertPath = "";
  EXPECT_EQ(Status::ERROR, manager->initialize(config));
}
