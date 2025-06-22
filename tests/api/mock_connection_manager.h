#ifndef SEP_TEST_MOCK_CONNECTION_MANAGER_H
#define SEP_TEST_MOCK_CONNECTION_MANAGER_H

#include "api/connection_manager.h"
#include "api/types.h"
#include <functional>
#include <gtest/gtest.h>
#include <memory>
#include <string>

namespace sep::api::test {

// Mock socket implementation
class MockSocket : public ISocket {
public:
  MockSocket() = default;
  ~MockSocket() override = default;
  int priority{0}; // Add priority member for testing
};

// Mock connection manager implementation
class MockConnectionManager : public IConnectionManager {
public:
  MockConnectionManager() = default;
  ~MockConnectionManager() override = default;

  // Public methods
  Status initialize(const ConnectionConfig &config) override {
    return initializeHandler ? initializeHandler(config) : Status::OK;
  }

  std::shared_ptr<ISocket> getConnection(const std::string &host,
                                         uint16_t port) override {
    return getConnectionHandler ? getConnectionHandler(host, port) : nullptr;
  }

  Status releaseConnection(std::shared_ptr<ISocket> socket) override {
    return releaseConnectionHandler ? releaseConnectionHandler(socket)
                                    : Status::OK;
  }

  Status closeAll() override {
    return closeAllHandler ? closeAllHandler() : Status::OK;
  }

  std::string getStats() const override {
    return getStatsHandler ? getStatsHandler() : "{}";
  }

  // Set handlers for testing
  void SetInitializeHandler(
      std::function<Status(const ConnectionConfig &)> handler) {
    initializeHandler = std::move(handler);
  }

  void SetGetConnectionHandler(
      std::function<std::shared_ptr<ISocket>(const std::string &, uint16_t)>
          handler) {
    getConnectionHandler = std::move(handler);
  }

  void SetReleaseConnectionHandler(
      std::function<Status(std::shared_ptr<ISocket>)> handler) {
    releaseConnectionHandler = std::move(handler);
  }

  void SetCloseAllHandler(std::function<Status()> handler) {
    closeAllHandler = std::move(handler);
  }

  void SetGetStatsHandler(std::function<std::string()> handler) {
    getStatsHandler = std::move(handler);
  }

protected:
  std::shared_ptr<ISocket> createConnection(const std::string &host,
                                            uint16_t port) override {
    return nullptr;
  }

  bool validateConnection(std::shared_ptr<ISocket> socket) override {
    return true;
  }

  void cleanupIdleConnections() override {}
  void sendKeepAlive() override {}

private:
  std::function<Status(const ConnectionConfig &)> initializeHandler;
  std::function<std::shared_ptr<ISocket>(const std::string &, uint16_t)>
      getConnectionHandler;
  std::function<Status(std::shared_ptr<ISocket>)> releaseConnectionHandler;
  std::function<Status()> closeAllHandler;
  std::function<std::string()> getStatsHandler;
};

} // namespace sep::api::test

#endif // SEP_TEST_MOCK_CONNECTION_MANAGER_H
