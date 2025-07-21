#include "oanda_trader_app.hpp"
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <iostream>
#include <cstdlib>
#include <GL/gl.h>

namespace sep::apps {

bool OandaTraderApp::initialize() {
    if (!initializeGraphics()) {
        last_error_ = "Failed to initialize graphics";
        return false;
    }
    
    setupImGui();
    
    // Initialize OANDA connector
    // oanda_connector_ = std::make_unique<sep::connectors::OandaConnector>();
    
    // Initialize SEP engine
    // sep_engine_ = std::make_unique<sep::core::SepEngine>();
    
    // Attempt to connect to OANDA
    connectToOanda();
    
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
    // io.ConfigFlags |= ImGuiConfigFlags_DockingEnable; // Not available in this ImGui version
    
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
    while (!glfwWindowShouldClose(window_)) {
        glfwPollEvents();
        
        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        
        // Enable docking (not available in this ImGui version)
        // ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
        
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
    
    // Show demo window if requested (not available in this ImGui version)
    // if (show_demo_window_) {
    //     ImGui::ShowDemoWindow(&show_demo_window_);
    // }
    
    // Main content panels
    renderConnectionStatus();
    renderAccountInfo();
    renderMarketData();
    renderTradePanel();
    renderPositions();
    renderOrderHistory();
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
    
    ImGui::Text("Real-time market data will appear here");
    ImGui::Separator();
    
    // Placeholder for currency pairs
    const char* pairs[] = { "EUR/USD", "GBP/USD", "USD/JPY", "AUD/USD" };
    for (const char* pair : pairs) {
        ImGui::Text("%s: Loading...", pair);
    }
    
    if (ImGui::Button("Update Market Data##market")) {
        updateMarketData();
    }
    
    ImGui::End();
}

void OandaTraderApp::renderTradePanel() {
    ImGui::Begin("Trade Panel");
    
    static char instrument[64] = "EUR_USD";
    static float units = 1000.0f;
    static bool is_buy = true;
    
    ImGui::InputText("Instrument", instrument, sizeof(instrument));
    ImGui::InputFloat("Units", &units);
    ImGui::Checkbox("Buy", &is_buy);
    ImGui::SameLine();
    if (!is_buy) ImGui::Text("(Sell)");
    
    ImGui::Separator();
    
    if (ImGui::Button("Place Order##trade", ImVec2(120, 30))) {
        // TODO: Implement order placement
        std::cout << "[Trade] Order placement not yet implemented" << std::endl;
    }
    
    ImGui::End();
}

void OandaTraderApp::renderPositions() {
    ImGui::Begin("Open Positions");
    
    ImGui::Text("No open positions");
    // TODO: Display actual positions from OANDA
    
    ImGui::End();
}

void OandaTraderApp::renderOrderHistory() {
    ImGui::Begin("Order History");
    
    ImGui::Text("Order history will appear here");
    // TODO: Display order history from OANDA
    
    ImGui::End();
}

void OandaTraderApp::connectToOanda() {
    // if (!oanda_connector_) {
    //     std::cerr << "[OANDA] Connector not initialized" << std::endl;
    //     return;
    // }
    
    // Check for environment variables
    const char* api_key = std::getenv("OANDA_API_KEY");
    const char* account_id = std::getenv("OANDA_ACCOUNT_ID");
    
    if (!api_key || !account_id) {
        std::cerr << "[OANDA] Missing environment variables. Set OANDA_API_KEY and OANDA_ACCOUNT_ID" << std::endl;
        oanda_connected_ = false;
        return;
    }
    
    std::cout << "[OANDA] Attempting to connect..." << std::endl;
    
    // Test connection by fetching account info
    // try {
    //     auto account_info = oanda_connector_->getAccountInfo();
    //     if (!account_info.empty()) {
    //         oanda_connected_ = true;
    //         std::cout << "[OANDA] Successfully connected!" << std::endl;
    //         refreshAccountInfo();
    //     } else {
    //         oanda_connected_ = false;
    //         std::cerr << "[OANDA] Failed to get account info" << std::endl;
    //     }
    // } catch (const std::exception& e) {
    //     oanda_connected_ = false;
    //     std::cerr << "[OANDA] Connection failed: " << e.what() << std::endl;
    // }
    
    // Placeholder - mark as connected for UI testing
    oanda_connected_ = true;
    std::cout << "[OANDA] Mock connection successful" << std::endl;
}

void OandaTraderApp::updateMarketData() {
    if (!oanda_connected_) {
        std::cout << "[Market] Not connected to OANDA" << std::endl;
        return;
    }
    
    std::cout << "[Market] Updating market data..." << std::endl;
    // TODO: Implement market data updates
}

void OandaTraderApp::refreshAccountInfo() {
    if (!oanda_connected_) {
        account_balance_ = "N/A";
        return;
    }
    
    // try {
    //     auto account_info = oanda_connector_->getAccountInfo();
    //     // TODO: Parse account info JSON and extract balance
    //     account_balance_ = "Loading...";
    //     std::cout << "[Account] Account info refreshed" << std::endl;
    // } catch (const std::exception& e) {
    //     std::cerr << "[Account] Failed to refresh: " << e.what() << std::endl;
    //     account_balance_ = "Error";
    // }
    
    // Mock account info for UI testing
    account_balance_ = "10000.00";
    std::cout << "[Account] Mock account info refreshed" << std::endl;
}

void OandaTraderApp::shutdown() {
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
