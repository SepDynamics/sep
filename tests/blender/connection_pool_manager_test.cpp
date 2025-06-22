#include <gtest/gtest.h>

#include <chrono>
#include <nlohmann/json.hpp>
#include <thread>

#include "api/connection_manager.h"

using namespace sep::api;

TEST(ConnectionPoolManagerIntegration, ReuseAndCleanup) {
  ConnectionConfig cfg;
  cfg.maxConnections = 2;
  cfg.maxConnectionsPerHost = 2;
  cfg.idleTimeout = 10;  // ms
  cfg.keepAliveInterval = 5;
  auto manager = create_connection_manager(cfg);
  ASSERT_NE(nullptr, manager);

  auto c1 = manager->getConnection("127.0.0.1", 80);
  ASSERT_NE(nullptr, c1);
  manager->releaseConnection(c1);

  auto c2 = manager->getConnection("127.0.0.1", 80);
  EXPECT_EQ(c1, c2);
  manager->releaseConnection(c2);

  std::this_thread::sleep_for(std::chrono::milliseconds(20));
  auto c3 = manager->getConnection("127.0.0.1", 80);
  EXPECT_NE(c2, c3);
  manager->releaseConnection(c3);

  auto stats = nlohmann::json::parse(manager->getStats());
  EXPECT_EQ(0u, stats["active_connections"].get<size_t>());
  manager->closeAll();
}
