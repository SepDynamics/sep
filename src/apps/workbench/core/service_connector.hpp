#pragma once

#include <memory>
#include <string>
#include <atomic>
#include <chrono>
#include <functional>
#include <thread>

#include "connectors/oanda_connector.h"

namespace sep {
namespace core {
class Engine;
}
}

namespace sep::workbench {

// Service connection states
enum class ConnectionState {
    DISCONNECTED,
    CONNECTING,
    CONNECTED,
    ERROR
};

// Service health metrics
struct ServiceHealth {
    bool is_responsive{false};
    std::chrono::milliseconds latency{0};
    uint64_t processed_patterns{0};
    float coherence_average{0.0f};
    std::string version_info;
    std::chrono::steady_clock::time_point last_heartbeat;
};

// Connection configuration
struct ConnectionConfig {
    std::string service_address{"127.0.0.1"};
    uint16_t service_port{3000};  // Changed to match API server default port
    std::chrono::milliseconds connection_timeout{5000};
    std::chrono::milliseconds heartbeat_interval{1000};
    uint32_t max_retry_attempts{3};
    bool auto_reconnect{true};
};

class ServiceConnector {
public:
    ServiceConnector();
    explicit ServiceConnector(const ConnectionConfig& config);
    ~ServiceConnector();

    // Connection management
    bool connect();
    void disconnect();
    bool reconnect();
    
    // State queries
    ConnectionState getConnectionState() const { return connection_state_; }
    bool isConnected() const { return connection_state_ == ConnectionState::CONNECTED; }
    
    // Service interaction
    sep::core::Engine* getEngine() const { return service_engine_; }
    sep::connectors::OandaConnector* getOandaConnector() const { return oanda_connector_.get(); }
    ServiceHealth getServiceHealth() const;
    
    // Health monitoring
    void startHealthMonitoring();
    void stopHealthMonitoring();
    bool sendHeartbeat();
    
    // Callbacks
    using ConnectionCallback = std::function<void(ConnectionState)>;
    void setConnectionCallback(ConnectionCallback callback) { connection_callback_ = callback; }
    
    // Configuration
    void setConfig(const ConnectionConfig& config) { config_ = config; }
    const ConnectionConfig& getConfig() const { return config_; }

private:
    // Internal state
    std::atomic<ConnectionState> connection_state_{ConnectionState::DISCONNECTED};
    ConnectionConfig config_;
    ServiceHealth health_metrics_;
    
    // Service connection
    sep::core::Engine* service_engine_{nullptr};
    std::unique_ptr<sep::connectors::OandaConnector> oanda_connector_;
    void* service_handle_{nullptr}; // Platform-specific handle
    
    // Health monitoring
    std::atomic<bool> monitoring_active_{false};
    std::unique_ptr<std::thread> monitor_thread_;
    
    // Callbacks
    ConnectionCallback connection_callback_;
    
    // Internal methods
    bool attemptConnection();
    void updateHealthMetrics();
    void monitoringLoop();
    bool validateServiceVersion();
    
    // Platform-specific connection methods
    bool connectTCP();
    bool connectIPC();
    bool connectSharedMemory();
    
    // Service engine creation
    sep::core::Engine* createServiceEngineProxy(int socket_fd);
};

} // namespace sep::workbench