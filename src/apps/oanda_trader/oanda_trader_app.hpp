#pragma once

// #include "connectors/oanda_connector.h"
#include <GLFW/glfw3.h>
#include <string>
#include <memory>

namespace sep::apps {

class OandaTraderApp {
public:
    OandaTraderApp() = default;
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
    void updateMarketData();
    void refreshAccountInfo();
    
    // OpenGL/GLFW setup
    bool initializeGraphics();
    void setupImGui();
    void cleanupGraphics();
    
    // Members
    GLFWwindow* window_ = nullptr;
    // std::unique_ptr<sep::connectors::OandaConnector> oanda_connector_;
    // std::unique_ptr<sep::core::SepEngine> sep_engine_;
    
    // UI state
    bool show_demo_window_ = false;
    bool oanda_connected_ = false;
    std::string account_balance_ = "N/A";
    std::string account_currency_ = "USD";
    
    // Error handling
    std::string last_error_;
    
    // Window settings
    static constexpr int WINDOW_WIDTH = 1400;
    static constexpr int WINDOW_HEIGHT = 900;
    static constexpr const char* WINDOW_TITLE = "SEP OANDA Trader";
};

} // namespace sep::apps
