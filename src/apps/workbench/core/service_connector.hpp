#pragma once

#include <memory>
#include <string>
#include <atomic>
#include <chrono>
#include <functional>
#include <thread>

#include "connectors/oanda_connector.h"
#include "service_proxy_engine.h"
#include "trade_manager.h"
#include "multi_timeframe_analyzer.h"
#include "tabs/signals_tab_controller.h"
#include "common/financial_data_types.h"

namespace sep {
namespace core {
class Engine;
}
}

namespace sep::workbench {

enum class ConnectionState {
    DISCONNECTED,
    CONNECTING,
    CONNECTED,
    CONNECTION_FAILED,
    RECONNECTING
};

// Service connection states


// Service health metrics
struct ServiceHealth {
    bool is_responsive{false};
    std::chrono::milliseconds latency{0};
    uint64_t processed_patterns{0};
    float coherence_average{0.0f};
    std::string version_info;
    std::chrono::steady_clock::time_point last_heartbeat;
    std::string last_error;
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
    workbench::TradeManager* getTradeManager() const { return trade_manager_.get(); }
    sep::core::ServiceProxyEngine* getServiceProxyEngine() const { return service_proxy_engine_; }
    ServiceHealth getServiceHealth() const;
    
    // Health monitoring
    void startHealthMonitoring();
    void stopHealthMonitoring();
    bool sendHeartbeat();

    bool startStreaming(const std::vector<std::string>& instruments);
    void stopStreaming();
    
    // Callbacks
    using ConnectionCallback = std::function<void(ConnectionState)>;
    void setConnectionCallback(ConnectionCallback callback) { connection_callback_ = callback; }
    
    // Configuration
    void setConfig(const ConnectionConfig& config) { config_ = config; }
    const ConnectionConfig& getConfig() const { return config_; }

    void setSignalsTab(sep::workbench::SignalsTabController* tab);
    void setMultiTimeframeAnalyzer(MultiTimeframeAnalyzer* analyzer);
    void setBacktester(sep::workbench::backtester::Backtester* bt) { backtester_ = bt; }
    const std::deque<common::CandleData>& getInitialData() const { return initial_data_; }

private:
    // Internal state
    std::atomic<ConnectionState> connection_state_{ConnectionState::DISCONNECTED};
    ConnectionConfig config_;
    ServiceHealth health_metrics_;
    
    // Service connection
    sep::core::Engine* service_engine_{nullptr};
    std::unique_ptr<sep::connectors::OandaConnector> oanda_connector_;
    std::unique_ptr<workbench::TradeManager> trade_manager_;
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
    sep::core::ServiceProxyEngine* createHttpEngineProxy(int socket_fd);
    
    // Engine instances
    std::unique_ptr<sep::core::ServiceProxyEngine> http_proxy_engine_;
    sep::core::ServiceProxyEngine* service_proxy_engine_{nullptr};

    sep::workbench::SignalsTabController* signals_tab_{nullptr};
    MultiTimeframeAnalyzer* mtf_analyzer_{nullptr};
    sep::workbench::backtester::Backtester* backtester_{nullptr};
    std::deque<common::CandleData> initial_data_;
    std::deque<common::SEPSignalData> initial_signals_;
    std::deque<common::SEPSignalData> streaming_signals_;

public:
    const std::deque<common::SEPSignalData>& getInitialSignals() const { return initial_signals_; }

    void loadInitialData(const std::string& path);
};

} // namespace sep::workbench