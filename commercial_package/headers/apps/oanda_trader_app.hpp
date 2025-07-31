#pragma once

#include <GLFW/glfw3.h>

#include <deque>
#include <map>
#include <memory>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

#include "connectors/oanda_connector.h"
#include "engine/internal/engine.h"
#include "imgui.h"
#include "apps/oanda_trader/cuda_types.cuh"
#include "apps/oanda_trader/forward_window_kernels.cuh"
#include "apps/oanda_trader/tick_cuda_kernels.cuh"
#include "quantum_signal_bridge.hpp"
#include "util/managed_thread.hpp"

namespace sep::apps {

class OandaTraderApp {
public:
    explicit OandaTraderApp(bool headless = false) : headless_mode_(headless) {}
    ~OandaTraderApp() = default;

    // Core lifecycle
    bool initialize();
    void run();
    void shutdown();
    
    // Error handling
    const std::string& getLastError() const { return last_error_; }

private:
    // UI rendering
    void renderMainInterface();
    void renderConnectionStatus();
    void renderAccountInfo();
    void renderMarketData();
    void renderTradePanel();
    void renderPositions();
    void renderOrderHistory();
    
    // OANDA integration
    void connectToOanda();
    void refreshAccountInfo();
    void refreshPositions();
    void refreshOrderHistory();
    
    // OpenGL/GLFW setup
    bool initializeGraphics();
    void setupImGui();
    void cleanupGraphics();
    
    // Members
    GLFWwindow* window_ = nullptr;
    std::unique_ptr<sep::connectors::OandaConnector> oanda_connector_;
    std::unique_ptr<sep::core::Engine> sep_engine_;
    
    // UI state
    bool show_demo_window_ = false;
    bool oanda_connected_ = false;
    std::string account_balance_ = "N/A";
    std::string account_currency_ = "USD";
    
    // Real-time data handling
    sep::util::ManagedThread data_stream_thread_;
    std::mutex market_data_mutex_;
    std::map<std::string, sep::connectors::MarketData> market_data_map_;
    std::deque<sep::connectors::MarketData> market_history_;
    std::mutex market_history_mutex_;
    std::unique_ptr<sep::trading::QuantumSignalBridge> quantum_bridge_;
    sep::trading::QuantumTradingSignal last_signal_;
    std::mutex signal_mutex_;
    std::vector<sep::apps::cuda::ForwardWindowResult> forward_window_results_;
    std::vector<nlohmann::json> open_positions_;
    std::mutex positions_mutex_;

    std::vector<nlohmann::json> order_history_;
    std::mutex history_mutex_;
    
    // Error handling
    std::string last_error_;
    sep::apps::cuda::CudaContext cuda_context_;
    
    // Runtime settings
    bool headless_mode_ = false;
    
    // Window settings
    static constexpr int WINDOW_WIDTH = 1400;
    static constexpr int WINDOW_HEIGHT = 900;
    static constexpr const char* WINDOW_TITLE = "SEP OANDA Trader";
};

} // namespace sep::apps
