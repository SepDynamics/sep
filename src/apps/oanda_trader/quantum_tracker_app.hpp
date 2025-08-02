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
#include "candle_types.h"
#include "market_model_cache.hpp"

namespace sep::apps {

class QuantumTrackerApp {
public:
    QuantumTrackerApp() = default;
    QuantumTrackerApp(const std::string& simulate_start_time, int simulate_duration_hours);
    QuantumTrackerApp(bool historical_sim); // New constructor for historical simulation
    QuantumTrackerApp(bool historical_sim, bool file_sim); // New constructor for file simulation
    ~QuantumTrackerApp() = default;

    // Core lifecycle
    bool initialize();
    void run();
    void runHeadlessService();
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
    void executeQuantumTrade(const sep::trading::QuantumTradingSignal& signal);
    
    // Weekend optimization
    void runWeekendOptimization();
    
    // Time Machine simulation
    void runSimulation();
    void runTestDataSimulation();
    void logSimulatedTrade(const sep::trading::QuantumTradingSignal& signal, const Candle& candle);
    
    // Historical simulation with current market data
    void runHistoricalSimulation();
    void logHistoricalTrade(const sep::trading::QuantumTradingSignal& signal, const Candle& candle, size_t candle_index);
    
    // File simulation with local test data
    void runFileSimulation();
    void logFileSimulatedTrade(const sep::trading::QuantumTradingSignal& signal, const Candle& candle);
    
    // Core components
    GLFWwindow* window_ = nullptr;
    std::unique_ptr<sep::connectors::OandaConnector> oanda_connector_;
    std::unique_ptr<QuantumTrackerWindow> quantum_tracker_;
    std::unique_ptr<DataCacheManager> cache_manager_;
    std::unique_ptr<TickDataManager> tick_manager_;
    std::unique_ptr<RollingWindowChart> window_chart_;
    std::unique_ptr<MarketModelCache> market_model_cache_;
    
    // Threading for market data
    std::thread data_stream_thread_;
    std::atomic<bool> streaming_active_{false};
    std::mutex connection_mutex_;
    
    // State
    bool oanda_connected_ = false;
    std::string last_error_;
    
    // Time Machine parameters
    std::string simulation_start_time_;
    int simulation_duration_hours_ = 0;
    
    // Historical simulation mode
    bool historical_sim_mode_ = false;
    
    // File simulation mode
    bool file_sim_mode_ = false;
    
    // Window settings
    static constexpr int WINDOW_WIDTH = 800;
    static constexpr int WINDOW_HEIGHT = 900;
    static constexpr const char* WINDOW_TITLE = "SEP Quantum Signal Tracker - Live";
};

} // namespace sep::apps
