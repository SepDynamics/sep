#pragma once

#include <GLFW/glfw3.h>
#include <memory>
#include <string>
#include <thread>
#include <atomic>
#include <mutex>

#include "connectors/oanda_connector.h"
#include "quantum_tracker_window.hpp"
#include "data_cache_manager.hpp"
#include "tick_data_manager.hpp"
#include "rolling_window_chart.hpp"

namespace sep::apps {

class QuantumTrackerApp {
public:
    QuantumTrackerApp() = default;
    ~QuantumTrackerApp() = default;

    // Core lifecycle
    bool initialize();
    void run();
    void shutdown();
    
    // Error handling
    const std::string& getLastError() const { return last_error_; }

private:
    // Graphics setup
    bool initializeGraphics();
    void setupImGui();
    void cleanupGraphics();
    
    // OANDA integration
    void connectToOanda();
    void loadHistoricalData();
    void startMarketDataStream();
    
    // Core components
    GLFWwindow* window_ = nullptr;
    std::unique_ptr<sep::connectors::OandaConnector> oanda_connector_;
    std::unique_ptr<QuantumTrackerWindow> quantum_tracker_;
    std::unique_ptr<DataCacheManager> cache_manager_;
    std::unique_ptr<TickDataManager> tick_manager_;
    std::unique_ptr<RollingWindowChart> window_chart_;
    
    // Threading for market data
    std::thread data_stream_thread_;
    std::atomic<bool> streaming_active_{false};
    std::mutex connection_mutex_;
    
    // State
    bool oanda_connected_ = false;
    std::string last_error_;
    
    // Window settings
    static constexpr int WINDOW_WIDTH = 800;
    static constexpr int WINDOW_HEIGHT = 900;
    static constexpr const char* WINDOW_TITLE = "SEP Quantum Signal Tracker - Live";
};

} // namespace sep::apps
