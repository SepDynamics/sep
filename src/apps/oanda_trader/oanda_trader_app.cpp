#include "oanda_trader_app.hpp"

#include <GL/gl.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <chrono>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <nlohmann/json.hpp>
#include <thread>

#include "connectors/market_data_converter.h"
#include "engine/internal/engine.h"
#include "imgui.h"

namespace sep::apps {

bool OandaTraderApp::initialize() {
    if (!headless_mode_) {
        if (!initializeGraphics()) {
            last_error_ = "Failed to initialize graphics";
            return false;
        }
        
        setupImGui();
    }
    
    // Initialize OANDA connector
    const char* api_key = std::getenv("OANDA_API_KEY");
    const char* account_id = std::getenv("OANDA_ACCOUNT_ID");
    if (!api_key || !account_id) {
        last_error_ = "OANDA_API_KEY and OANDA_ACCOUNT_ID environment variables must be set.";
        return false;
    }
    oanda_connector_ = std::make_unique<sep::connectors::OandaConnector>(api_key, account_id);
    
    // Initialize SEP engine
    sep_engine_ = std::make_unique<sep::core::Engine>();

    // Initialize quantum bridge for signal processing
    quantum_bridge_ = std::make_unique<sep::trading::QuantumSignalBridge>();
    if (!quantum_bridge_->initialize()) {
        last_error_ = "Failed to initialize quantum signal bridge";
        return false;
    }

    // Set default thresholds based on tracker configuration
    quantum_bridge_->setConfidenceThreshold(0.6f);
    quantum_bridge_->setCoherenceThreshold(0.4f);
    quantum_bridge_->setStabilityThreshold(0.0f);

    return true;
}

bool OandaTraderApp::initializeGraphics() {
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
    
    // OpenGL is already loaded by GLFW context
    
    return true;
}

void OandaTraderApp::setupImGui() {
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    
    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    
    // Custom OANDA-focused theme
    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowRounding = 5.0f;
    style.FrameRounding = 3.0f;
    style.GrabRounding = 3.0f;
    style.ScrollbarRounding = 3.0f;
    
    // OANDA-inspired colors (blue/orange theme)
    ImVec4* colors = style.Colors;
    colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.08f, 0.12f, 0.94f);
    colors[ImGuiCol_Header] = ImVec4(0.15f, 0.35f, 0.65f, 0.80f);
    colors[ImGuiCol_HeaderHovered] = ImVec4(0.20f, 0.40f, 0.70f, 0.80f);
    colors[ImGuiCol_HeaderActive] = ImVec4(0.25f, 0.45f, 0.75f, 0.80f);
    colors[ImGuiCol_Button] = ImVec4(0.15f, 0.35f, 0.65f, 0.70f);
    colors[ImGuiCol_ButtonHovered] = ImVec4(0.20f, 0.40f, 0.70f, 1.00f);
    colors[ImGuiCol_ButtonActive] = ImVec4(0.25f, 0.45f, 0.75f, 1.00f);
    
    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window_, true);
    ImGui_ImplOpenGL3_Init("#version 330");
}

void OandaTraderApp::run() {
    sep::apps::cuda::initializeCudaDevice(cuda_context_);

    while (!glfwWindowShouldClose(window_)) {
        // Perform forward-looking window calculations
        if (oanda_connected_ && !market_history_.empty()) {
            std::vector<sep::apps::cuda::TickData> ticks;
            {
                std::lock_guard<std::mutex> lock(market_history_mutex_);
                ticks.reserve(market_history_.size());
                for (const auto& md : market_history_) {
                    ticks.push_back({md.mid, md.bid, md.ask, md.timestamp, md.volume});
                }
            }
            const uint64_t window_size_ns = 24ULL * 3600ULL * 1000000000ULL; // 24 hours
            sep::apps::cuda::calculateForwardWindowsCuda(cuda_context_, ticks, forward_window_results_, window_size_ns);
        }
        glfwPollEvents();
        
        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        
        // Render main interface
        renderMainInterface();
        
        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window_, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.06f, 0.08f, 0.12f, 1.00f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        
        glfwSwapBuffers(window_);
    }
}

void OandaTraderApp::renderMainInterface() {
    // Main menu bar
    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("Exit")) {
                glfwSetWindowShouldClose(window_, GLFW_TRUE);
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Connection")) {
            if (ImGui::MenuItem("Connect to OANDA")) {
                connectToOanda();
            }
            if (ImGui::MenuItem("Refresh Account")) {
                refreshAccountInfo();
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("View")) {
            ImGui::MenuItem("Show Demo Window", nullptr, &show_demo_window_);
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
    
    // Set default window positions and sizes
    ImGui::SetNextWindowPos(ImVec2(0, 18));
    ImGui::SetNextWindowSize(ImVec2(300, 200));
    renderConnectionStatus();

    ImGui::SetNextWindowPos(ImVec2(0, 218));
    ImGui::SetNextWindowSize(ImVec2(300, 150));
    renderAccountInfo();

    ImGui::SetNextWindowPos(ImVec2(0, 368));
    ImGui::SetNextWindowSize(ImVec2(300, 200));
    renderTradePanel();

    ImGui::SetNextWindowPos(ImVec2(300, 18));
    ImGui::SetNextWindowSize(ImVec2(1100, 450));
    renderMarketData();

    ImGui::SetNextWindowPos(ImVec2(300, 468));
    ImGui::SetNextWindowSize(ImVec2(1100, 200));
    renderPositions();

    ImGui::SetNextWindowPos(ImVec2(300, 668));
    ImGui::SetNextWindowSize(ImVec2(550, 232));
    renderOrderHistory();

    // Quantum analysis is handled by separate quantum_tracker app
}

void OandaTraderApp::renderConnectionStatus() {
    ImGui::Begin("Connection Status");
    
    // Connection indicator
    if (oanda_connected_) {
        ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "● CONNECTED");
        ImGui::SameLine();
        ImGui::Text("OANDA API");
    } else {
        ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "● DISCONNECTED");
        ImGui::SameLine();
        ImGui::Text("OANDA API");
    }
    
    ImGui::Separator();
    
    // Connection controls
    if (ImGui::Button("Connect##oanda")) {
        connectToOanda();
    }
    ImGui::SameLine();
    if (ImGui::Button("Refresh##status")) {
        refreshAccountInfo();
    }
    
    ImGui::End();
}

void OandaTraderApp::renderAccountInfo() {
    ImGui::Begin("Account Information");
    
    ImGui::Text("Account Balance: %s %s", account_balance_.c_str(), account_currency_.c_str());
    ImGui::Text("Account Currency: %s", account_currency_.c_str());
    
    if (ImGui::Button("Refresh Account##account")) {
        refreshAccountInfo();
    }
    
    ImGui::End();
}

void OandaTraderApp::renderMarketData() {
    ImGui::Begin("Market Data");

    ImGui::Text("Real-time market data:");
    ImGui::Separator();
    
    std::lock_guard<std::mutex> lock(market_data_mutex_);
    for (const auto& [instrument, data] : market_data_map_) {
        ImGui::Text("%s - Bid: %.5f, Ask: %.5f, Time: %llu",
                    instrument.c_str(), data.bid, data.ask, (unsigned long long)data.timestamp);
    }

    {
        std::lock_guard<std::mutex> lock(signal_mutex_);
        if (last_signal_.timestamp != 0) {
            const char* action_str = last_signal_.action == sep::trading::QuantumTradingSignal::BUY ? "BUY" :
                                      last_signal_.action == sep::trading::QuantumTradingSignal::SELL ? "SELL" : "HOLD";
            ImGui::Separator();
            ImGui::Text("Quantum Signal: %s", action_str);
            ImGui::Text("Conf: %.2f Coh: %.2f Stab: %.2f", last_signal_.identifiers.confidence,
                        last_signal_.identifiers.coherence, last_signal_.identifiers.stability);
        }
    }

    ImGui::End();
}

void OandaTraderApp::renderTradePanel() {
    ImGui::Begin("Trade Panel");

    static char instrument[64] = "EUR_USD";
    static float risk_per_trade = 2.0f; // 2% risk
    static bool is_buy = true;
    static float manual_atr = 0.0010; // Manual ATR for now

    ImGui::InputText("Instrument", instrument, sizeof(instrument));
    ImGui::InputFloat("Risk %", &risk_per_trade);
    ImGui::InputFloat("Manual ATR", &manual_atr); // Later get this from connector
    ImGui::Checkbox("Buy", &is_buy);
    ImGui::SameLine();
    if (!is_buy) ImGui::Text("(Sell)");

    ImGui::Separator();

    if (ImGui::Button("Place Order##trade", ImVec2(120, 30))) {
        if (oanda_connected_) {
            // --- Logic from oanda_connector.js ---
            double balance = 0.0;
            try {
                balance = std::stod(account_balance_);
            } catch (...) {
                std::cerr << "Invalid account balance" << std::endl;
                return;
            }

            const double risk_amount = balance * (risk_per_trade / 100.0);
            const double stop_loss_pips = manual_atr * 10000;
            const double pip_value = 0.0001; // For EUR_USD
            const double position_units_double = std::floor(risk_amount / (stop_loss_pips * pip_value));
            const int position_units = static_cast<int>(position_units_double);


            float order_units = is_buy ? position_units : -position_units;

            double current_price = 0.0;
            {
                std::lock_guard<std::mutex> lock(market_data_mutex_);
                if (market_data_map_.count(instrument)) {
                    current_price = market_data_map_[instrument].mid;
                }
            }

            if (current_price > 0) {
                double stop_loss_price = is_buy
                                       ? (current_price - (stop_loss_pips * pip_value))
                                       : (current_price + (stop_loss_pips * pip_value));

                nlohmann::json order_details;
                order_details["order"]["instrument"] = instrument;
                order_details["order"]["units"] = std::to_string(order_units);
                order_details["order"]["type"] = "MARKET";
                order_details["order"]["timeInForce"] = "FOK";
                order_details["order"]["positionFill"] = "DEFAULT";
                order_details["order"]["stopLossOnFill"]["price"] = std::to_string(stop_loss_price);
                order_details["order"]["stopLossOnFill"]["timeInForce"] = "GTC";


                // This assumes placeOrder is updated to take a json object
                auto result = oanda_connector_->placeOrder(order_details);

                if (result.find("error") != result.end()) {
                    std::cout << "[Trade] Failed to place order: " << result["error"] << std::endl;
                } else {
                    std::cout << "[Trade] Order placed successfully: " << instrument << " " << order_units << std::endl;
                }
            } else {
                 std::cout << "[Trade] Could not get current price for " << instrument << std::endl;
            }

        } else {
            std::cout << "[Trade] Not connected to OANDA" << std::endl;
        }
    }

    ImGui::End();
}

void OandaTraderApp::renderPositions() {
    ImGui::Begin("Open Positions");
    
    if (ImGui::Button("Refresh Positions")) {
        refreshPositions();
    }
    
    ImGui::Separator();
    
    std::lock_guard<std::mutex> lock(positions_mutex_);
    for (const auto& position : open_positions_) {
        std::string instrument = position["instrument"];
        std::string long_units = position["long"]["units"];
        std::string short_units = position["short"]["units"];
        std::string pnl = position["unrealizedPL"];
        
        if (long_units != "0") {
            ImGui::Text("%s | Units: %s | P/L: %s", instrument.c_str(), long_units.c_str(), pnl.c_str());
        }
        if (short_units != "0") {
            ImGui::Text("%s | Units: %s | P/L: %s", instrument.c_str(), short_units.c_str(), pnl.c_str());
        }
    }
    
    ImGui::End();
}

void OandaTraderApp::refreshPositions() {
    if (!oanda_connected_) return;
    
    auto positions_json = oanda_connector_->getOpenPositions();
    if (positions_json.contains("positions")) {
        std::lock_guard<std::mutex> lock(positions_mutex_);
        open_positions_ = positions_json["positions"].get<std::vector<nlohmann::json>>();
    }
}

void OandaTraderApp::refreshOrderHistory() {
    if (!oanda_connected_) return;
    
    auto orders_json = oanda_connector_->getOrders();
    if (orders_json.contains("orders")) {
        std::lock_guard<std::mutex> lock(history_mutex_);
        order_history_ = orders_json["orders"].get<std::vector<nlohmann::json>>();
    }
}

void OandaTraderApp::renderOrderHistory() {
    ImGui::Begin("Order History");
    
    if (ImGui::Button("Refresh History")) {
        refreshOrderHistory();
    }
    
    ImGui::Separator();
    
    std::lock_guard<std::mutex> lock(history_mutex_);
    for (const auto& order : order_history_) {
        std::string id = order["id"];
        std::string instrument = order["instrument"];
        std::string units = order["units"];
        std::string type = order["type"];
        std::string state = order["state"];
        
        ImGui::Text("ID: %s | %s | Units: %s | Type: %s | Status: %s",
                    id.c_str(), instrument.c_str(), units.c_str(), type.c_str(), state.c_str());
    }
    
    ImGui::End();
}

void OandaTraderApp::connectToOanda() {
    if (!oanda_connector_) {
        std::cerr << "[OANDA] Connector not initialized" << std::endl;
        return;
    }
    
    // Check for environment variables
    const char* api_key = std::getenv("OANDA_API_KEY");
    const char* account_id = std::getenv("OANDA_ACCOUNT_ID");
    
    if (!api_key || !account_id) {
        std::cerr << "[OANDA] Missing environment variables. Set OANDA_API_KEY and OANDA_ACCOUNT_ID" << std::endl;
        oanda_connected_ = false;
        return;
    }
    
    std::cout << "[OANDA] Attempting to connect..." << std::endl;
    
    // Initialize the connector
    if (!oanda_connector_->initialize()) {
        std::cerr << "[OANDA] Failed to initialize connector: " << oanda_connector_->getLastError() << std::endl;
        oanda_connected_ = false;
        return;
    }
    
    oanda_connected_ = true;
    std::cout << "[OANDA] Successfully connected!" << std::endl;
    refreshAccountInfo();
    
    // Set the price callback
    oanda_connector_->setPriceCallback([this](const sep::connectors::MarketData& data) {
        {
            std::lock_guard<std::mutex> lock(market_data_mutex_);
            market_data_map_[data.instrument] = data;
        }

        {
            std::lock_guard<std::mutex> lock(market_history_mutex_);
            market_history_.push_back(data);
            if (market_history_.size() > 256) {
                market_history_.pop_front();
            }
        }

        if (quantum_bridge_) {
            std::vector<sep::connectors::MarketData> history_copy;
            {
                std::lock_guard<std::mutex> lock(market_history_mutex_);
                history_copy.assign(market_history_.begin(), market_history_.end());
            }

            auto signal = quantum_bridge_->analyzeMarketData(data, history_copy, forward_window_results_);
            {
                std::lock_guard<std::mutex> lock(signal_mutex_);
                last_signal_ = signal;
            }
            if (signal.should_execute) {
                std::cout << "[Signal] " << (signal.action == sep::trading::QuantumTradingSignal::BUY ? "BUY" : "SELL")
                          << " confidence:" << signal.identifiers.confidence << " size:" << signal.suggested_position_size << std::endl;
            }
        }
    });

    // Start the price stream using managed thread
    try {
        data_stream_thread_.start([this]() {
            std::cout << "[OANDA] Starting price stream for EUR_USD..." << std::endl;
            if (!oanda_connector_->startPriceStream({"EUR_USD"})) {
                std::cerr << "[OANDA] Failed to start price stream: "
                          << oanda_connector_->getLastError() << std::endl;
            }
        });
    } catch (const std::exception& e) {
        std::cerr << "[OANDA] Exception starting price stream: " << e.what() << std::endl;
    }
}
void OandaTraderApp::refreshAccountInfo() {
    if (!oanda_connected_ || !oanda_connector_) {
        account_balance_ = "N/A";
        account_currency_ = "N/A";
        return;
    }
    
    try {
        auto account_json = oanda_connector_->getAccountInfo();
        if (account_json.contains("account")) {
            account_balance_ = account_json["account"]["balance"];
            account_currency_ = account_json["account"]["currency"];
            
            std::cout << "[Account] Balance: " << account_balance_
                      << " " << account_currency_ << std::endl;
        } else if (account_json.contains("error")) {
            std::cerr << "[Account] Error: " << account_json["error"] << std::endl;
            account_balance_ = "Error";
            account_currency_ = "N/A";
        }
    } catch (const std::exception& e) {
        std::cerr << "[Account] Exception: " << e.what() << std::endl;
        account_balance_ = "Error";
        account_currency_ = "N/A";
    }
}

void OandaTraderApp::shutdown() {
    sep::apps::cuda::cleanupCudaDevice(cuda_context_);
    if (oanda_connector_) {
        oanda_connector_->stopPriceStream();
    }
    data_stream_thread_.join();
    if (quantum_bridge_) {
        quantum_bridge_->shutdown();
    }
    cleanupGraphics();
}

void OandaTraderApp::cleanupGraphics() {
    if (window_) {
        // Cleanup ImGui
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
        
        // Cleanup GLFW
        glfwDestroyWindow(window_);
        glfwTerminate();
        window_ = nullptr;
    }
}

} // namespace sep::apps
