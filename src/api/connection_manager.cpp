#include "api/connection_manager.h"

#include <algorithm>
#include <chrono>
#include <cstdint>
#include <memory>
#include <mutex>
#include <nlohmann/json.hpp>
#include <string>
#include <unordered_map>
#include <vector>

#include "api/background_cleanup.h"

namespace sep::api {

namespace {

struct BasicSocket : public ISocket {
  std::string host;
  uint16_t port{0};
  bool open{true};

  BasicSocket(std::string h, uint16_t p) : host(std::move(h)), port(p) {}
};

}  // namespace

class ConnectionPoolManager : public IConnectionManager {
 public:
  Status initialize(const ConnectionConfig &config) override {
    std::lock_guard<std::mutex> lock(mutex_);
    config_ = config;
    int interval = 0;
    if (config_.keepAliveInterval > 0 && config_.idleTimeout > 0) {
      interval = std::min(config_.keepAliveInterval, config_.idleTimeout);
    } else if (config_.keepAliveInterval > 0) {
      interval = config_.keepAliveInterval;
    } else if (config_.idleTimeout > 0) {
      interval = config_.idleTimeout;
    }

    if (interval > 0) {
      cleanup_task_ = std::make_unique<BackgroundCleanup>(
          std::chrono::milliseconds(interval), [this](auto) {
            if (config_.keepAliveInterval > 0) {
              sendKeepAlive();
            }
            if (config_.idleTimeout > 0) {
              cleanupIdleConnections();
            }
          });
    }
    return Status::OK;
  }

  std::shared_ptr<ISocket> getConnection(const std::string &host, uint16_t port) override {
    std::lock_guard<std::mutex> lock(mutex_);
    cleanupIdleConnections();
    const std::string key = host + ":" + std::to_string(port);

    if (config_.reuseConnections) {
      auto it = idle_pool_.find(key);
      if (it != idle_pool_.end() && !it->second.empty()) {
        auto entry = std::move(it->second.back());
        it->second.pop_back();
        active_per_host_[key]++;
        active_connections_++;
        reused_connections_++;
        return entry.socket;
      }
    }

    if (active_connections_ >= static_cast<size_t>(config_.maxConnections) ||
        active_per_host_[key] >= static_cast<size_t>(config_.maxConnectionsPerHost)) {
      return nullptr;
    }

    auto socket = createConnection(host, port);
    if (!socket) return nullptr;
    active_per_host_[key]++;
    active_connections_++;
    total_connections_++;
    return socket;
  }

  Status releaseConnection(std::shared_ptr<ISocket> socket) override {
    if (!socket) return Status::OK;

    auto *basic = static_cast<BasicSocket *>(socket.get());

    std::lock_guard<std::mutex> lock(mutex_);
    const std::string key = basic->host + ":" + std::to_string(basic->port);
    if (config_.reuseConnections) {
      idle_pool_[key].push_back({socket, std::chrono::steady_clock::now(), key});
    }
    active_per_host_[key]--;
    active_connections_--;
    return Status::OK;
  }

  Status closeAll() override {
    std::lock_guard<std::mutex> lock(mutex_);
    idle_pool_.clear();
    active_per_host_.clear();
    active_connections_ = 0;
    return Status::OK;
  }

  std::string getStats() const override {
    std::lock_guard<std::mutex> lock(mutex_);
    nlohmann::json j;
    j["active_connections"] = active_connections_;
    j["total_connections"] = total_connections_;
    j["reused_connections"] = reused_connections_;
    size_t idle = 0;
    for (const auto &kv : idle_pool_) {
      idle += kv.second.size();
    }
    j["idle_connections"] = idle;
    nlohmann::json per_host;
    for (const auto &kv : active_per_host_) {
      size_t idle_count = 0;
      auto it = idle_pool_.find(kv.first);
      if (it != idle_pool_.end()) {
        idle_count = it->second.size();
      }
      per_host[kv.first] = {{"active", kv.second}, {"idle", idle_count}};
    }
    j["per_host"] = per_host;
    return j.dump();
  }

 protected:
  std::shared_ptr<ISocket> createConnection(const std::string &host, uint16_t port) override {
    return std::make_shared<BasicSocket>(host, port);
  }

  bool validateConnection(std::shared_ptr<ISocket> /*socket*/) override { return true; }

  void cleanupIdleConnections() override {
    if (config_.idleTimeout <= 0 || idle_pool_.empty())
      return;
    const auto now = std::chrono::steady_clock::now();
    for (auto it = idle_pool_.begin(); it != idle_pool_.end();) {
      auto &vec = it->second;
      vec.erase(std::remove_if(vec.begin(), vec.end(),
                               [&](const PooledConn &c) {
                                 return (now - c.lastUsed) >=
                                        std::chrono::milliseconds(config_.idleTimeout);
                               }),
                vec.end());
      if (vec.empty()) {
        it = idle_pool_.erase(it);
      } else {
        ++it;
      }
    }
  }

  void sendKeepAlive() override {
    if (config_.keepAliveInterval <= 0 || idle_pool_.empty())
      return;
    const auto now = std::chrono::steady_clock::now();
    for (auto &kv : idle_pool_) {
      for (auto &conn : kv.second) {
        if ((now - conn.lastUsed) >=
            std::chrono::milliseconds(config_.keepAliveInterval)) {
          conn.lastUsed = now;
        }
      }
    }
  }

 private:
  struct PooledConn {
    std::shared_ptr<ISocket> socket;
    std::chrono::steady_clock::time_point lastUsed;
    std::string key;
  };

  ConnectionConfig config_{};
  std::unique_ptr<BackgroundCleanup> cleanup_task_;
  std::unordered_map<std::string, std::vector<PooledConn>> idle_pool_;
  std::unordered_map<std::string, size_t> active_per_host_;
  size_t active_connections_{0};
  size_t total_connections_{0};
  size_t reused_connections_{0};
  mutable std::mutex mutex_;
};

std::unique_ptr<IConnectionManager> create_connection_manager(const ConnectionConfig &config) {
  auto manager = std::make_unique<ConnectionPoolManager>();
  if (manager->initialize(config) != Status::OK) {
    return nullptr;
  }
  return manager;
}

}  // namespace sep::api
