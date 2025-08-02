#include "quantum_tracker_app.hpp"
#include "data_cache_manager.hpp"
#include "tick_data_manager.hpp"
#include "candle_types.h"
#include <GL/gl.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <imgui.h>
#include <implot.h>

#include <chrono>
#include <cstdlib>
#include <iostream>
#include <mutex>
#include <thread>
#include <condition_variable>
#include <nlohmann/json.hpp>

namespace sep::apps {

bool QuantumTrackerApp::initialize() {
    if (!initializeGraphics()) {
        last_error_ = "Failed to initialize graphics";
        return false;
    }
    
    setupImGui();
    
    // Initialize quantum tracker
    quantum_tracker_ = std::make_unique<QuantumTrackerWindow>();
    if (!quantum_tracker_->initialize()) {
        last_error_ = "Failed to initialize quantum tracker";
        return false;
    }
    
    // Initialize cache manager
    cache_manager_ = std::make_unique<DataCacheManager>();
    
    // Initialize tick data manager
    tick_manager_ = std::make_unique<TickDataManager>();
    
    // Initialize OANDA connector
    const char* api_key = std::getenv("OANDA_API_KEY");
    const char* account_id = std::getenv("OANDA_ACCOUNT_ID");
    if (!api_key || !account_id) {
        last_error_ = "OANDA_API_KEY and OANDA_ACCOUNT_ID environment variables must be set.";
        return false;
    }
    
    oanda_connector_ = std::make_unique<sep::connectors::OandaConnector>(api_key, account_id);
    
    // Initialize cache manager with OANDA connector
    if (!cache_manager_->initialize(oanda_connector_.get())) {
        last_error_ = "Failed to initialize data cache manager";
        return false;
    }
    
    // Initialize tick data manager with OANDA connector
    if (!tick_manager_->initialize(oanda_connector_.get())) {
        last_error_ = "Failed to initialize tick data manager";
        return false;
    }
    
    // --- DYNAMIC BOOTSTRAP SEQUENCE ---
    std::cout << "[Bootstrap] Fetching 48 hours of historical M1 data to build M5/M15 signals..." << std::endl;

    // 1. Fetch historical M1 data from OANDA
    std::vector<Candle> historical_m1_candles;
    std::mutex mtx;
    std::condition_variable cv;
    bool data_fetched = false;

    // OANDA requires specific time formats - fetch from last 5 trading days to handle weekends
    auto now = std::chrono::system_clock::now();
    auto start_time = now - std::chrono::hours(120); // 5 days to ensure we get trading data
    char from_str[32], to_str[32];
    auto from_time_t = std::chrono::system_clock::to_time_t(start_time);
    auto to_time_t = std::chrono::system_clock::to_time_t(now);
    std::strftime(from_str, sizeof(from_str), "%Y-%m-%dT%H:%M:%SZ", std::gmtime(&from_time_t));
    std::strftime(to_str, sizeof(to_str), "%Y-%m-%dT%H:%M:%SZ", std::gmtime(&to_time_t));
    
    std::cout << "[Bootstrap] Requesting M1 data from " << from_str << " to " << to_str << std::endl;

    oanda_connector_->getHistoricalData(
        "EUR_USD", "M1", from_str, to_str,
        [&](const std::vector<sep::connectors::OandaCandle>& oanda_candles) {
            std::lock_guard<std::mutex> lock(mtx);
            // Convert OandaCandle to the local Candle struct
            for (const auto& o_candle : oanda_candles) {
                Candle c;
                c.time = o_candle.time;
                c.timestamp = parseTimestamp(o_candle.time);
                c.open = o_candle.open;
                c.high = o_candle.high;
                c.low = o_candle.low;
                c.close = o_candle.close;
                c.volume = static_cast<double>(o_candle.volume);
                historical_m1_candles.push_back(c);
            }
            data_fetched = true;
            cv.notify_one();
        });

    // Wait for the asynchronous fetch to complete
    std::unique_lock<std::mutex> lock(mtx);
    if (!cv.wait_for(lock, std::chrono::seconds(30), [&]{ return data_fetched; })) {
        std::cout << "[Bootstrap] API fetch timeout. Falling back to static test data for development..." << std::endl;
        
        // Fallback to static file initialization for development/testing
        if (!quantum_tracker_->getQuantumBridge()->initializeMultiTimeframe(
            "/sep/Testing/OANDA/O-test-M5.json",
            "/sep/Testing/OANDA/O-test-M15.json")) {
            last_error_ = "Failed to initialize with both dynamic and static data";
            return false;
        }
        std::cout << "[Bootstrap] Static fallback completed successfully! System ready for live trading." << std::endl;
    } else if (historical_m1_candles.empty()) {
        std::cout << "[Bootstrap] API returned 0 candles (likely weekend/market closed). Using static test data..." << std::endl;
        
        // Fallback to static file initialization when no data available
        if (!quantum_tracker_->getQuantumBridge()->initializeMultiTimeframe(
            "/sep/Testing/OANDA/O-test-M5.json",
            "/sep/Testing/OANDA/O-test-M15.json")) {
            last_error_ = "Failed to initialize with static fallback data";
            return false;
        }
        std::cout << "[Bootstrap] Static fallback completed successfully! System ready for live trading." << std::endl;
    } else {
        std::cout << "[Bootstrap] Fetched " << historical_m1_candles.size() << " M1 candles. Initializing multi-timeframe system..." << std::endl;

        // 2. Bootstrap the QuantumSignalBridge with the historical data
        quantum_tracker_->getQuantumBridge()->bootstrap(historical_m1_candles);

        std::cout << "[Bootstrap] Dynamic bootstrap completed successfully! System ready for live trading." << std::endl;
    }
    // --- END OF DYNAMIC BOOTSTRAP ---
    
    // Auto-connect to OANDA
    connectToOanda();
    
    return true;
}

bool QuantumTrackerApp::initializeGraphics() {
    // Initialize GLFW
    if (!glfwInit()) {
        last_error_ = "Failed to initialize GLFW";
        return false;
    }
    
    // Setup GLFW window
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    window_ = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, nullptr, nullptr);
    if (!window_) {
        last_error_ = "Failed to create GLFW window";
        glfwTerminate();
        return false;
    }
    
    glfwMakeContextCurrent(window_);
    glfwSwapInterval(1); // Enable vsync
    
    return true;
}

void QuantumTrackerApp::setupImGui() {
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImPlot::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    
    // Setup Dear ImGui style with quantum theme
    ImGui::StyleColorsDark();
    
    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowRounding = 8.0f;
    style.FrameRounding = 4.0f;
    style.GrabRounding = 4.0f;
    style.ScrollbarRounding = 4.0f;
    style.WindowPadding = ImVec2(12, 12);
    style.FramePadding = ImVec2(8, 4);
    style.ItemSpacing = ImVec2(8, 6);
    
    // Quantum-inspired colors (purple/blue/cyan theme)
    ImVec4* colors = style.Colors;
    colors[ImGuiCol_WindowBg] = ImVec4(0.08f, 0.08f, 0.15f, 0.95f);
    colors[ImGuiCol_Header] = ImVec4(0.25f, 0.15f, 0.55f, 0.80f);
    colors[ImGuiCol_HeaderHovered] = ImVec4(0.35f, 0.25f, 0.65f, 0.90f);
    colors[ImGuiCol_HeaderActive] = ImVec4(0.45f, 0.35f, 0.75f, 1.00f);
    colors[ImGuiCol_Button] = ImVec4(0.20f, 0.20f, 0.45f, 0.70f);
    colors[ImGuiCol_ButtonHovered] = ImVec4(0.30f, 0.30f, 0.55f, 0.90f);
    colors[ImGuiCol_ButtonActive] = ImVec4(0.40f, 0.40f, 0.65f, 1.00f);
    colors[ImGuiCol_FrameBg] = ImVec4(0.15f, 0.15f, 0.25f, 0.60f);
    colors[ImGuiCol_FrameBgHovered] = ImVec4(0.20f, 0.20f, 0.35f, 0.70f);
    colors[ImGuiCol_FrameBgActive] = ImVec4(0.25f, 0.25f, 0.45f, 0.80f);
    colors[ImGuiCol_TitleBg] = ImVec4(0.15f, 0.10f, 0.35f, 1.00f);
    colors[ImGuiCol_TitleBgActive] = ImVec4(0.25f, 0.20f, 0.55f, 1.00f);
    colors[ImGuiCol_TableHeaderBg] = ImVec4(0.20f, 0.15f, 0.40f, 1.00f);
    colors[ImGuiCol_TableBorderStrong] = ImVec4(0.35f, 0.25f, 0.55f, 1.00f);
    colors[ImGuiCol_TableBorderLight] = ImVec4(0.25f, 0.15f, 0.45f, 1.00f);
    colors[ImGuiCol_TableRowBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
    colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.0f, 1.0f, 1.0f, 0.04f);
    
    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window_, true);
    ImGui_ImplOpenGL3_Init("#version 330");
}

void QuantumTrackerApp::run() {
    std::cout << "[QuantumTracker] Starting quantum signal tracking..." << std::endl;
    
    // Set up GLFW error callback
    glfwSetErrorCallback([](int error, const char* description) {
        std::cerr << "[GLFW Error] " << error << ": " << description << std::endl;
    });
    
    while (!glfwWindowShouldClose(window_)) {
        // Poll events with timeout to prevent infinite blocking
        glfwWaitEventsTimeout(0.016); // ~60 FPS equivalent
        
        // Check if window should close due to external signals
        if (glfwGetKey(window_, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window_, GLFW_TRUE);
            break;
        }
        
        // Start ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        
        // Set window to fill entire viewport
        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
        ImGui::SetNextWindowBgAlpha(1.0f);
        
        // Render quantum tracker window
        quantum_tracker_->render();
        
        // Connection status overlay
        if (!oanda_connected_) {
            ImGui::SetNextWindowPos(ImVec2(10, 10));
            ImGui::Begin("Connection", nullptr, 
                        ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | 
                        ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove);
            ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "⚠ DISCONNECTED");
            ImGui::Text("Waiting for OANDA connection...");
            ImGui::End();
        } else {
            ImGui::SetNextWindowPos(ImVec2(10, 10));
            ImGui::Begin("Connection", nullptr, 
                        ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | 
                        ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove);
            ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "● CONNECTED");
            ImGui::Text("Live quantum analysis active");
            ImGui::End();
        }
        
        // Render
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window_, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.05f, 0.05f, 0.12f, 1.00f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        
        // Check for OpenGL errors
        GLenum gl_error = glGetError();
        if (gl_error != GL_NO_ERROR) {
            std::cerr << "[OpenGL Error] " << gl_error << std::endl;
        }
        
        glfwSwapBuffers(window_);
        
        // Small sleep to prevent excessive CPU usage
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}

void QuantumTrackerApp::connectToOanda() {
    std::lock_guard<std::mutex> lock(connection_mutex_);
    
    if (!oanda_connector_) {
        std::cerr << "[QuantumTracker] Connector not initialized" << std::endl;
        return;
    }
    
    std::cout << "[QuantumTracker] Attempting to connect to OANDA..." << std::endl;
    
    // Initialize the connector
    if (!oanda_connector_->initialize()) {
        std::cerr << "[QuantumTracker] Failed to initialize connector: " 
                  << oanda_connector_->getLastError() << std::endl;
        oanda_connected_ = false;
        return;
    }
    
    oanda_connected_ = true;
    std::cout << "[QuantumTracker] Successfully connected to OANDA!" << std::endl;
    
    // Start market data stream
    startMarketDataStream();
}

void QuantumTrackerApp::loadHistoricalData() {
    std::cout << "[QuantumTracker] Loading 2H TICK-LEVEL data for EUR_USD..." << std::endl;
    std::cout << "[QuantumTracker] This will collect ALL price updates, not just M1 candles!" << std::endl;
    
    if (!tick_manager_) {
        std::cerr << "[QuantumTracker] Tick manager not initialized" << std::endl;
        return;
    }
    
    // Load tick-level historical data (this gets ALL price updates)
    if (!tick_manager_->loadHistoricalTicks("EUR_USD")) {
        std::cerr << "[QuantumTracker] Failed to load historical tick data. Continuing with live data only..." << std::endl;
        return;
    }
    
    std::cout << "[QuantumTracker] Tick data collection completed!" << std::endl;
    std::cout << "  - Total ticks: " << tick_manager_->getTickCount() << std::endl;
    std::cout << "  - Average ticks/min: " << tick_manager_->getAverageTicksPerMinute() << std::endl;
    std::cout << "  - Hourly calculations: " << tick_manager_->getHourlyCalculations().size() << std::endl;
    std::cout << "  - Daily calculations: " << tick_manager_->getDailyCalculations().size() << std::endl;
    
    // Process the rolling window calculations through quantum tracker
    auto hourly_prices = tick_manager_->getHourlyPrices();
    auto daily_prices = tick_manager_->getDailyPrices();
    auto timestamps = tick_manager_->getTimestamps();
    
    std::cout << "[QuantumTracker] Feeding " << hourly_prices.size() 
              << " hourly calculations to quantum analysis..." << std::endl;
    
    // Feed rolling window calculations to quantum tracker for pattern analysis
    for (size_t i = 0; i < hourly_prices.size() && i < timestamps.size(); ++i) {
        sep::connectors::MarketData synthetic_data;
        synthetic_data.instrument = "EUR_USD";
        synthetic_data.mid = hourly_prices[i];
        synthetic_data.bid = hourly_prices[i] - 0.00001;
        synthetic_data.ask = hourly_prices[i] + 0.00001;
        synthetic_data.timestamp = timestamps[i];
        synthetic_data.volume = 100; // Synthetic volume
        synthetic_data.atr = 0.0001;
        
        quantum_tracker_->processNewMarketData(synthetic_data, std::to_string(timestamps[i]));
        
        // Rate limit for visual feedback
        if (i % 100 == 0) {
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            std::cout << "[QuantumTracker] Processed " << (i + 1) 
                      << "/" << hourly_prices.size() << " rolling calculations" << std::endl;
        }
    }
    
    std::cout << "[QuantumTracker] TICK-LEVEL historical analysis complete!" << std::endl;
    std::cout << "  - Now ready for real-time tick processing with rolling windows" << std::endl;
}

void QuantumTrackerApp::startMarketDataStream() {
    // Load historical data safely
    loadHistoricalData();
    
    // Set the price callback to feed BOTH tick manager and quantum tracker
    oanda_connector_->setPriceCallback([this](const sep::connectors::MarketData& data) {
        // Feed tick to tick manager for rolling window calculations
        if (tick_manager_) {
            tick_manager_->processNewTick(data);
        }
        
        // Feed data to quantum tracker for pattern analysis
        quantum_tracker_->processNewMarketData(data);
        
        // Check for triple-confirmed signals and execute trades
        if (quantum_tracker_ && quantum_tracker_->hasLatestSignal()) {
            const auto& latest_signal = quantum_tracker_->getLatestSignal();
            if (latest_signal.should_execute && 
                latest_signal.mtf_confirmation.triple_confirmed &&
                latest_signal.action != sep::trading::QuantumTradingSignal::HOLD) {
                executeQuantumTrade(latest_signal);
            }
        }
        
        // Log occasional data for debugging with tick info
        static int count = 0;
        if (++count % 100 == 0) {
            std::cout << "[QuantumTracker] Processed " << count << " TICKS. ";
            if (tick_manager_) {
                std::cout << "Total ticks: " << tick_manager_->getTickCount() 
                         << ", Hourly calcs: " << tick_manager_->getHourlyCalculations().size() << ". ";
            }
            std::cout << "Predictions: " << quantum_tracker_->getStats().total_predictions 
                     << ", Accuracy: " << quantum_tracker_->getStats().accuracy_percentage << "%" << std::endl;
        }
    });

    // Start the price stream once
    std::cout << "[QuantumTracker] Starting EUR_USD price stream..." << std::endl;
    if (!oanda_connector_->startPriceStream({"EUR_USD"})) {
        std::cerr << "[QuantumTracker] Failed to start price stream: " 
                  << oanda_connector_->getLastError() << std::endl;
    } else {
        std::cout << "[QuantumTracker] Price stream started successfully!" << std::endl;
    }
}

void QuantumTrackerApp::executeQuantumTrade(const sep::trading::QuantumTradingSignal& signal) {
    if (!oanda_connector_) {
        std::cerr << "[QuantumTracker] Cannot execute trade - OANDA connector not initialized" << std::endl;
        return;
    }
    
    std::cout << "[QuantumTracker] 🚀 EXECUTING QUANTUM TRADE - "
              << (signal.action == sep::trading::QuantumTradingSignal::BUY ? "BUY" : "SELL")
              << " " << signal.suggested_position_size << " units of EUR_USD" << std::endl;
              
    // Create order JSON
    nlohmann::json order_json = {
        {"order", {
            {"instrument", "EUR_USD"},
            {"units", signal.action == sep::trading::QuantumTradingSignal::BUY ? 
                     static_cast<int>(signal.suggested_position_size) : 
                     -static_cast<int>(signal.suggested_position_size)},
            {"type", "MARKET"},
            {"timeInForce", "FOK"}, // Fill or Kill
            {"stopLossOnFill", {
                {"distance", std::to_string(signal.stop_loss_distance)}
            }},
            {"takeProfitOnFill", {
                {"distance", std::to_string(signal.take_profit_distance)}
            }}
        }}
    };
    
    // Execute the trade
    if (oanda_connector_->placeOrder(order_json)) {
        std::cout << "[QuantumTracker] ✅ Trade executed successfully!" << std::endl;
        std::cout << "[QuantumTracker] Stop Loss: " << signal.stop_loss_distance 
                  << " Take Profit: " << signal.take_profit_distance << std::endl;
    } else {
        std::cerr << "[QuantumTracker] ❌ Trade execution failed: " 
                  << oanda_connector_->getLastError() << std::endl;
    }
}

void QuantumTrackerApp::shutdown() {
    std::cout << "[QuantumTracker] Shutting down..." << std::endl;
    
    // Stop OANDA stream
    if (oanda_connector_) {
        oanda_connector_->stopPriceStream();
    }
    
    // Shutdown quantum tracker
    if (quantum_tracker_) {
        quantum_tracker_->shutdown();
    }
    
    cleanupGraphics();
    
    // Print final stats
    if (quantum_tracker_) {
        const auto& stats = quantum_tracker_->getStats();
        std::cout << "[QuantumTracker] Final Results:" << std::endl;
        std::cout << "  Total Predictions: " << stats.total_predictions << std::endl;
        std::cout << "  Correct: " << stats.correct_predictions << std::endl;
        std::cout << "  Accuracy: " << stats.accuracy_percentage << "%" << std::endl;
        std::cout << "  Average Confidence: " << stats.average_confidence << std::endl;
    }
}

void QuantumTrackerApp::cleanupGraphics() {
    if (window_) {
        // Cleanup ImGui
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImPlot::DestroyContext();
        ImGui::DestroyContext();
        
        // Cleanup GLFW
        glfwDestroyWindow(window_);
        glfwTerminate();
        window_ = nullptr;
    }
}

} // namespace sep::apps
