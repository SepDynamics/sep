#pragma once

#include <memory>
#include <string>

#include "api/types.h"              // API-specific types
#include "engine/internal/types.h"  // For Status enum

namespace sep {
namespace api {

class ISocket {
 public:
  virtual ~ISocket() = default;
  int priority{0};  // Add priority member
};

struct ConnectionConfig {
  int maxConnections{100};
  int maxConnectionsPerHost{10};
  int connectionTimeout{5000};
  int keepAliveInterval{30000};
  int idleTimeout{300000};
  bool reuseConnections{true};
  bool validateConnections{true};
  bool validateSSL{false};
  std::string sslCertPath;
};

struct ConnectionPoolStats {
  size_t activeConnections{0};
  size_t totalConnections{0};
  size_t reusedConnections{0};
  size_t idleConnections{0};
};

class IConnectionManager {
 public:
  virtual ~IConnectionManager() = default;

  virtual Status initialize(const ConnectionConfig &config) = 0;
  virtual std::shared_ptr<ISocket> getConnection(const std::string &host, uint16_t port) = 0;
  virtual Status releaseConnection(std::shared_ptr<ISocket> socket) = 0;
  virtual Status closeAll() = 0;
  virtual std::string getStats() const = 0;

 protected:
  virtual std::shared_ptr<ISocket> createConnection(const std::string &host, uint16_t port) = 0;
  virtual bool validateConnection(std::shared_ptr<ISocket> socket) = 0;
  virtual void cleanupIdleConnections() = 0;
  virtual void sendKeepAlive() = 0;
};



}  // namespace api
}  // namespace sep
