#include "quantum_tracker_app.hpp"
#include <GL/gl.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <imgui.h>

#include <chrono>
#include <cstdlib>
#include <iostream>
#include <thread>

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
    
    // Initialize OANDA connector
    const char* api_key = std::getenv("OANDA_API_KEY");
    const char* account_id = std::getenv("OANDA_ACCOUNT_ID");
    if (!api_key || !account_id) {
        last_error_ = "OANDA_API_KEY and OANDA_ACCOUNT_ID environment variables must be set.";
        return false;
    }
    
    oanda_connector_ = std::make_unique<sep::connectors::OandaConnector>(api_key, account_id);
    
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
    
    while (!glfwWindowShouldClose(window_)) {
        glfwPollEvents();
        
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
        
        glfwSwapBuffers(window_);
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

void QuantumTrackerApp::startMarketDataStream() {
    // First, load 48 hours of historical data
    std::cout << "[QuantumTracker] Loading 48 hours of historical EUR_USD data..." << std::endl;
    if (!oanda_connector_->fetchHistoricalData("EUR_USD", "")) {
        std::cerr << "[QuantumTracker] Failed to load historical data: " 
                  << oanda_connector_->getLastError() << std::endl;
        std::cerr << "[QuantumTracker] Continuing with live data only..." << std::endl;
    } else {
        std::cout << "[QuantumTracker] Historical data loaded successfully!" << std::endl;
    }
    
    // Set the price callback to feed quantum tracker
    oanda_connector_->setPriceCallback([this](const sep::connectors::MarketData& data) {
        // Feed data to quantum tracker
        quantum_tracker_->processNewMarketData(data);
        
        // Log occasional data for debugging
        static int count = 0;
        if (++count % 100 == 0) {
            std::cout << "[QuantumTracker] Processed " << count 
                     << " data points. Stats: " 
                     << quantum_tracker_->getStats().total_predictions << " predictions, "
                     << quantum_tracker_->getStats().accuracy_percentage << "% accuracy" << std::endl;
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
        ImGui::DestroyContext();
        
        // Cleanup GLFW
        glfwDestroyWindow(window_);
        glfwTerminate();
        window_ = nullptr;
    }
}

} // namespace sep::apps
