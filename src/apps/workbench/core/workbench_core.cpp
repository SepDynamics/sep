#include "workbench_core.hpp"

// Include engine first to get proper includes
#include "engine/config.h"
#include "engine/engine.h"

// Include workbench components
// #include "demo_orchestrator.hpp"
// #include "demos/register_demos.hpp"
#include "landing_page.hpp"
#include "renderer.h"
#include "service_connector.hpp"
#include "apps/workbench/tabs/signals_tab_controller.h"
#include "apps/workbench/tabs/engine_tab_controller.h"
#include "apps/workbench/tabs/backend_tab_controller.h"
#include "apps/workbench/backtester/data/data_loader.h"
#include "apps/workbench/backtester/ui/backtester_tab_controller.h"

#include "apps/workbench/signal_generator/quantum_signal_generator.h"
#include "apps/workbench/core/metrics_monitor.h"
#include "apps/workbench/core/multi_timeframe_analyzer.h"
#include "memory/memory_tier_manager.hpp"
#include "engine/data_parser.h"

// Include ImGui headers
#include "imgui.h"
#include "imgui_internal.h"
#include "imgui_helpers.hpp"
#include <implot.h>

// Use correct paths for ImGui implementation files
#include <chrono>
#include <iostream>
#include <thread>

#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

namespace sep::workbench {

// Static instance for GLFW callbacks
WorkbenchEngine* WorkbenchEngine::instance_ = nullptr;

WorkbenchEngine::WorkbenchEngine()
{
    instance_ = this;
    metrics_.startup_time = ::std::chrono::steady_clock::now();
}

WorkbenchEngine::~WorkbenchEngine()
{
    // Only shutdown if not already shut down
    if (current_state_ != ApplicationState::SHUTTING_DOWN)
    {
        shutdown();
    }
    instance_ = nullptr;
}

bool WorkbenchEngine::initialize()
{
    ::std::cout << "[WorkbenchEngine] Starting initialization sequence..." << ::std::endl;

    try {
        // Initialize GLFW
        if (!initializeGLFW()) {
            reportError("Failed to initialize GLFW");
            return false;
        }
        
        // Create window
        if (!createWindow()) {
            reportError("Failed to create window");
            return false;
        }
        
        // Initialize OpenGL
        if (!initializeOpenGL()) {
            reportError("Failed to initialize OpenGL");
            return false;
        }
        
        // Initialize ImGui
        if (!initializeImGui()) {
            reportError("Failed to initialize ImGui");
            return false;
        }
        
        // Initialize core components
        ::std::cout << "[WorkbenchEngine] Initializing core components..." << ::std::endl;

        // Removed demo components for real trading
        // sep::workbench::registerDemos();
        // demo_orchestrator_ = std::make_unique<DemoOrchestrator>();
        // landing_page_ = std::make_unique<LandingPage>(this);
        renderer_ = ::std::make_unique<Renderer>();
        layout_manager_ = ::std::make_unique<UILayoutManager>();
        layout_manager_->setRefreshInterval(0.05f);

        // Subscribe to basic events
        globalEventBus().subscribe<PanelVisibilityEvent>([](const PanelVisibilityEvent& e) {
            std::cout << "[EventBus] Panel " << e.panel_id
                      << (e.visible ? " shown" : " hidden") << std::endl;
        });
        globalEventBus().subscribe<ConnectionStateEvent>([this](const ConnectionStateEvent& e) {
            metrics_.service_connected = (e.state == ConnectionState::CONNECTED);
        });
        signal_generator_ = ::std::make_unique<QuantumSignalGenerator>();
        signal_generator_->setMemoryManager(&::sep::memory::MemoryTierManager::getInstance());
        metrics_monitor_ = ::std::make_shared<MetricsMonitor>();
        multi_timeframe_analyzer_ = ::std::make_unique<MultiTimeframeAnalyzer>();
        multi_timeframe_analyzer_->setMetricsMonitor(metrics_monitor_.get());
        multi_timeframe_analyzer_->setMetricsCallback([this](const std::map<std::string, workbench::TimeframeMetrics>& m) {
            if (metrics_monitor_) {
                auto rolling = metrics_monitor_->getRollingMetrics();
                auto it1 = m.find("1h");
                if (it1 != m.end()) {
                    rolling.coherence_1h_avg = it1->second.dominant_coherence;
                    rolling.stability_1h_avg = it1->second.stability_index;
                    rolling.entropy_1h_avg = it1->second.entropy_level;
                }
                auto it4 = m.find("4h");
                if (it4 != m.end()) {
                    rolling.coherence_4h_avg = it4->second.dominant_coherence;
                    rolling.stability_4h_avg = it4->second.stability_index;
                    rolling.entropy_4h_avg = it4->second.entropy_level;
                }
                metrics_monitor_->setRollingMetrics(rolling);
            }

            {
                std::lock_guard<std::mutex> lock(pending_metrics_mutex_);
                pending_metrics_ = m;
                metrics_ready_ = true;
            }
        });

        service_connector_ = ::std::make_unique<ServiceConnector>();
        service_connector_->setMultiTimeframeAnalyzer(multi_timeframe_analyzer_.get());
        
        // Initialize renderer and metrics dashboard
        int width, height;
        glfwGetFramebufferSize(window_, &width, &height);
        renderer_->init(width, height);

        signals_tab_ = std::make_unique<workbench::SignalsTabController>();
        engine_tab_ = std::make_unique<workbench::EngineTabController>();
        backend_tab_ = std::make_unique<workbench::BackendTabController>(metrics_monitor_,
                                                                         multi_timeframe_analyzer_.get());
        backtester_tab_ = std::make_unique<BacktesterTabController>();

        signals_tab_->initialize();
        engine_tab_->initialize();
        backend_tab_->initialize();
        backend_tab_->setMetricsMonitor(metrics_monitor_);

        // Set up data flow
        signals_tab_->setOandaConnector(service_connector_->getOandaConnector());
        signals_tab_->setQuantumSignalGenerator(signal_generator_.get());
        signals_tab_->setMetricsMonitor(metrics_monitor_);
        signals_tab_->setWorkbenchEngine(this);
        signals_tab_->setMultiTimeframeAnalyzer(multi_timeframe_analyzer_.get());
        service_connector_->setSignalsTab(signals_tab_.get());
        engine_tab_->setSEPEngine(active_engine_);
        engine_tab_->setMetricsMonitor(metrics_monitor_);
        engine_tab_->setMultiTimeframeAnalyzer(multi_timeframe_analyzer_.get());
        engine_tab_->setServiceProxyEngine(service_connector_->getServiceProxyEngine());
        backend_tab_->setServiceConnector(service_connector_.get());
        backend_tab_->setTradeManager(service_connector_->getTradeManager());

        // Create offline engine as fallback
        ::std::cout << "[WorkbenchEngine] Creating offline engine..." << ::std::endl;
        offline_engine_ = ::std::make_unique<core::Engine>();
        // Initialize with default config
        config::CudaConfig config;
        offline_engine_->init(config);
        active_engine_ = offline_engine_.get();

        if (backtester_tab_) {
            backtester_tab_->setPatternMetricEngine(getPatternMetricEngine());
            if (service_connector_) {
                backtester_tab_->setOandaConnector(service_connector_->getOandaConnector());
            }
        }
        
        // Skip service check - use offline engine as primary engine
        ::std::cout << "[WorkbenchEngine] Using offline engine as primary engine - no service needed" << ::std::endl;
        
        // Connect components together
        std::cout << "[WorkbenchEngine] Connecting trading components..." << std::endl;
        if (service_connector_ && service_connector_->getOandaConnector()) {
            auto oanda_ptr = service_connector_->getOandaConnector();
            std::cout << "[WorkbenchEngine] OANDA connector available: " << (oanda_ptr ? "Yes" : "No") << std::endl;
            setupOandaCallbacks(oanda_ptr);
            service_connector_->startStreaming({"EUR_USD"});
        }

        const char* skip_env = std::getenv("SEP_SKIP_FETCH");
        if (!skip_env && signals_tab_) {
            backtester::DataLoader loader;
            auto candles = loader.load_data("eur_usd_m1_48h.json");
            if (!candles.empty()) {
                std::deque<sep::common::CandleData> dq(candles.begin(), candles.end());
                signals_tab_->setCandleData(dq);
            }
        }

        transitionTo(ApplicationState::LANDING_PAGE);

        std::cout << "[WorkbenchEngine] Initialization complete!" << std::endl;
        return true;
        
    } catch (const std::exception& e) {
        reportError(std::string("Initialization exception: ") + e.what());
        return false;
    }
}

bool WorkbenchEngine::initializeGLFW()
{
    glfwSetErrorCallback(errorCallback);
    
    if (!glfwInit()) {
        std::cerr << "[WorkbenchEngine] Failed to initialize GLFW" << std::endl;
        return false;
    }

    std::cout << "[WorkbenchEngine] GLFW initialized successfully" << std::endl;
    return true;
}

bool WorkbenchEngine::createWindow()
{
    // Configure GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    
    // Create window
    window_ = glfwCreateWindow(
        window_config_.width,
        window_config_.height,
        window_config_.title.c_str(),
        window_config_.fullscreen ? glfwGetPrimaryMonitor() : nullptr,
        nullptr
    );
    
    if (!window_) {
        std::cerr << "[WorkbenchEngine] Failed to create GLFW window" << std::endl;
        return false;
    }
    
    glfwMakeContextCurrent(window_);
    glfwSwapInterval(window_config_.vsync ? 1 : 0);
    
    // Set callbacks
    glfwSetKeyCallback(window_, keyCallback);
    glfwSetMouseButtonCallback(window_, mouseButtonCallback);
    glfwSetCursorPosCallback(window_, cursorPosCallback);
    glfwSetFramebufferSizeCallback(window_, framebufferSizeCallback);

    std::cout << "[WorkbenchEngine] Window created successfully" << std::endl;
    return true;
}

bool WorkbenchEngine::initializeOpenGL()
{
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "[WorkbenchEngine] Failed to initialize GLAD" << std::endl;
        return false;
    }

    std::cout << "[WorkbenchEngine] OpenGL Info:" << std::endl;
    std::cout << "  Version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "  Vendor: " << glGetString(GL_VENDOR) << std::endl;
    std::cout << "  Renderer: " << glGetString(GL_RENDERER) << std::endl;
    
    // Configure OpenGL state
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    return true;
}

bool WorkbenchEngine::initializeImGui()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImPlot::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    
    // Setup style
    ImGui::StyleColorsDark();
    auto applyModernTheme = [](){
        ImGuiStyle& style = ImGui::GetStyle();
        style.WindowRounding = 5.0f;
        style.FrameRounding = 5.0f;
        style.ScrollbarRounding = 5.0f;

        ImVec4* colors = style.Colors;
        colors[ImGuiCol_WindowBg] = ImVec4(0.1f, 0.105f, 0.11f, 1.0f);
        colors[ImGuiCol_Header] = ImVec4(0.2f, 0.205f, 0.21f, 1.0f);
        colors[ImGuiCol_HeaderHovered] = ImVec4(0.3f, 0.305f, 0.31f, 1.0f);
        colors[ImGuiCol_HeaderActive] = ImVec4(0.25f, 0.255f, 0.26f, 1.0f);
        colors[ImGuiCol_Button] = ImVec4(0.2f, 0.205f, 0.21f, 1.0f);
        colors[ImGuiCol_ButtonHovered] = ImVec4(0.3f, 0.305f, 0.31f, 1.0f);
        colors[ImGuiCol_ButtonActive] = ImVec4(0.15f, 0.1505f, 0.151f, 1.0f);
    };
    applyModernTheme();
    
    // Setup platform/renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(window_, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    std::cout << "[WorkbenchEngine] ImGui initialized successfully" << std::endl;
    return true;
}

void WorkbenchEngine::cleanupImGui()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImPlot::DestroyContext();
    ImGui::DestroyContext();
}

void WorkbenchEngine::run()
{
    std::cout << "[WorkbenchEngine] Starting main loop..." << std::endl;

    auto last_frame_time = std::chrono::steady_clock::now();
    
    while (!glfwWindowShouldClose(window_) && !should_exit_) {
        auto current_time = std::chrono::steady_clock::now();
        float delta_time = std::chrono::duration<float>(current_time - last_frame_time).count();
        last_frame_time = current_time;
        
        // Poll events
        glfwPollEvents();
        
        // Process input
        processInput();
        
        // Update
        updateFrame(delta_time);
        
        // Render
        renderFrame();
        
        // Update metrics
        updateMetrics(delta_time);
        
        // Swap buffers
        glfwSwapBuffers(window_);
    }
}

void WorkbenchEngine::processInput()
{
    // ESC to exit
    if (glfwGetKey(window_, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        should_exit_ = true;
    }
    
    // F1 for help
    if (glfwGetKey(window_, GLFW_KEY_F1) == GLFW_PRESS) {
        // Show help overlay
    }
}

void WorkbenchEngine::updateFrame(float delta_time)
{
    // Handle state-specific updates
    handleStateTransition();

    // No demo updates needed for trading mode
    updateData();

    {
        std::lock_guard<std::mutex> lock(pending_metrics_mutex_);
        if (metrics_ready_)
        {
            if (signals_tab_)
            {
                signals_tab_->setMetricsMonitor(metrics_monitor_);
                signals_tab_->setLatestMetrics(pending_metrics_);
            }
            if (engine_tab_)
            {
                engine_tab_->setMetricsMonitor(metrics_monitor_);
            }
            metrics_ready_ = false;
        }
    }
}

void WorkbenchEngine::renderFrame()
{
    // Clear
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // Start ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    
    // Render unified dashboard as main interface
    if (layout_manager_) {
        layout_manager_->render();
    }
    
    renderTabs();
    
    // Handle special states
    if (current_state_.load() == ApplicationState::ERROR_RECOVERY) {
        renderErrorRecovery();
    }
    
    // Render status bar
    renderStatusBar();
    
    // Render ImGui
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void WorkbenchEngine::renderLoadingScreen()
{
    ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x * 0.5f, 
                                   ImGui::GetIO().DisplaySize.y * 0.5f), 
                           ImGuiCond_Always, ImVec2(0.5f, 0.5f));
    ImGui::Begin("Loading", nullptr, ImGuiWindowFlags_NoTitleBar | 
                                     ImGuiWindowFlags_NoResize | 
                                     ImGuiWindowFlags_AlwaysAutoResize);
    
    ImGui::Text("Initializing SEP Workbench...");
    
    // Spinner
    // Replace spinner with simple loading text
    ImGui::Text("Loading...");

    ImGui::End();
}

void WorkbenchEngine::renderErrorRecovery()
{
    ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x * 0.5f, 
                                   ImGui::GetIO().DisplaySize.y * 0.5f), 
                           ImGuiCond_Always, ImVec2(0.5f, 0.5f));
    ImGui::Begin("Error", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
    
    ImGui::TextColored(ImVec4(1.0f, 0.2f, 0.2f, 1.0f), "An error occurred:");
    ImGui::TextWrapped("%s", metrics_.last_error.c_str());
    
    ImGui::Spacing();
    
    if (ImGui::Button("Return to Landing Page")) {
        transitionTo(ApplicationState::LANDING_PAGE);
    }
    
    ImGui::SameLine();
    
    if (ImGui::Button("Exit")) {
        should_exit_ = true;
    }
    
    ImGui::End();
}

void WorkbenchEngine::renderStatusBar()
{
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoScrollbar | 
                                   ImGuiWindowFlags_NoSavedSettings | 
                                   ImGuiWindowFlags_MenuBar;
    float height = ImGui::GetFrameHeight();
    
    if (ImGui::BeginViewportSideBar("##StatusBar", nullptr, ImGuiDir_Down, height, window_flags)) {
        if (ImGui::BeginMenuBar()) {
            // Service status
            if (metrics_.service_connected) {
                ImGui::TextColored(ImVec4(0.2f, 1.0f, 0.2f, 1.0f), "Service: Connected");
            } else {
                ImGui::TextColored(ImVec4(1.0f, 0.8f, 0.2f, 1.0f), "Service: Offline Mode");
            }
            
            ImGui::Separator();
            
            // FPS
            ImGui::Text("FPS: %.1f", metrics_.average_fps);
            
            ImGui::Separator();
            
            // Current state
            const char* state_names[] = {
                "Initializing", "Service Check", "Landing Page", 
                "Demo Selection", "Demo Running", "Error Recovery", "Shutting Down"
            };
            ImGui::Text("State: %s", state_names[static_cast<int>(current_state_.load())]);
            
            // Current demo if running
            if (!metrics_.current_demo.empty()) {
                ImGui::Separator();
                ImGui::Text("Demo: %s", metrics_.current_demo.c_str());
            }
            
            ImGui::EndMenuBar();
        }
        ImGui::End();
    }
}

void WorkbenchEngine::handleStateTransition()
{
    switch (current_state_.load()) {
        case ApplicationState::INITIALIZING:
            handleInitializing();
            break;
        case ApplicationState::SERVICE_CHECK:
            handleServiceCheck();
            break;
        case ApplicationState::LANDING_PAGE:
            handleLandingPage();
            break;
        case ApplicationState::DEMO_SELECTION:
            handleDemoSelection();
            break;
        case ApplicationState::DEMO_RUNNING:
            handleDemoRunning();
            break;
        case ApplicationState::ERROR_RECOVERY:
            handleErrorRecovery();
            break;
        default:
            break;
    }
}

void WorkbenchEngine::handleInitializing()
{
    // Initialization is handled in initialize()
}

void WorkbenchEngine::handleServiceCheck()
{
    std::cout << "[WorkbenchEngine] Checking for SEP service..." << std::endl;

    // Attempt to connect to service
    attemptServiceConnection();
    
    // For now, proceed to landing page regardless
    transitionTo(ApplicationState::LANDING_PAGE);
}

void WorkbenchEngine::handleLandingPage()
{
    // Landing page handles its own logic
}

void WorkbenchEngine::handleDemoSelection()
{
    // Demo selection is handled by landing page
}

void WorkbenchEngine::handleDemoRunning()
{
    // Demo running is handled by demo orchestrator
}

void WorkbenchEngine::handleErrorRecovery()
{
    // Error recovery UI is rendered in renderErrorRecovery()
}

void WorkbenchEngine::attemptServiceConnection()
{
    ConnectionState state = ConnectionState::DISCONNECTED;
    if (service_connector_) {
        metrics_.service_connected = service_connector_->connect();
        if (engine_tab_) {
            engine_tab_->setServiceProxyEngine(service_connector_->getServiceProxyEngine());
        }

        if (metrics_.service_connected) {
            std::cout << "[WorkbenchEngine] Successfully connected to SEP service" << std::endl;
            active_engine_ = service_connector_->getEngine();
            if (!active_engine_) {
                std::cout
                    << "[WorkbenchEngine] Service engine is null, falling back to offline mode"
                    << std::endl;
                active_engine_ = offline_engine_.get();
                metrics_.service_connected = false;
                state = ConnectionState::DISCONNECTED;
            } else {
                state = ConnectionState::CONNECTED;
            }
        } else {
            std::cout << "[WorkbenchEngine] Failed to connect to SEP service, using offline mode"
                      << std::endl;
            active_engine_ = offline_engine_.get();
            state = ConnectionState::CONNECTION_FAILED;
        }
    } else {
        std::cout << "[WorkbenchEngine] No service connector available, using offline mode"
                  << std::endl;
        active_engine_ = offline_engine_.get();
        state = ConnectionState::DISCONNECTED;
    }

    globalEventBus().publish(ConnectionStateEvent{state});
    if (state == ConnectionState::CONNECTED) {
        ImGui::Toast("Service Connected", "Successfully connected to SEP service");
    } else if (state == ConnectionState::CONNECTION_FAILED) {
        ImGui::Toast("Connection Failed", "Could not connect to SEP service");
    }

    // Ensure we always have a valid engine
    if (!active_engine_)
    {
        std::cerr << "[WorkbenchEngine] CRITICAL: No engine available!" << std::endl;
        reportError("No engine available - cannot proceed");
    }
}

void WorkbenchEngine::transitionTo(ApplicationState new_state)
{
    ApplicationState old_state = current_state_.load();
    std::cout << "[WorkbenchEngine] State transition: " << static_cast<int>(old_state) << " -> "
              << static_cast<int>(new_state) << std::endl;

    current_state_ = new_state;
}

void WorkbenchEngine::selectDemo(const std::string& demo_name)
{
    // Demo system removed for trading mode
    (void)demo_name;
}

void WorkbenchEngine::stopCurrentDemo()
{
    // Demo system removed for trading mode
}

void WorkbenchEngine::showMetricsDashboard(bool show) {
    // Dashboard is always visible in unified mode
    std::cout << "[WorkbenchEngine] Dashboard is always visible" << std::endl;
}

void WorkbenchEngine::updateMetrics(float delta_time)
{
    metrics_.frame_count++;
    
    // Update FPS (simple moving average)
    float fps = 1.0f / delta_time;
    metrics_.average_fps = metrics_.average_fps * 0.95f + fps * 0.05f;
}

void WorkbenchEngine::reportError(const std::string& error)
{
    std::cerr << "[WorkbenchEngine] ERROR: " << error << std::endl;
    metrics_.last_error = error;
}

void WorkbenchEngine::shutdown()
{
    // Prevent double shutdown - try to transition from any non-shutdown state
    ApplicationState current = current_state_.load();
    if (current == ApplicationState::SHUTTING_DOWN)
    {
        return;  // Already shutting down
    }

    // Try to set to shutting down from current state
    while (!current_state_.compare_exchange_weak(current, ApplicationState::SHUTTING_DOWN))
    {
        if (current == ApplicationState::SHUTTING_DOWN)
        {
            return;  // Another thread started shutdown
        }
    }

    std::cout << "[WorkbenchEngine] Shutting down..." << std::endl;

    // Stop the main loop
    should_exit_ = true;

    if (service_connector_) {
        service_connector_->stopStreaming();
        service_connector_->disconnect();
    }

    // Clean up components (demo system removed)
    
    // Clean up ImGui
    cleanupImGui();
    
    // Clean up window
    if (window_) {
        glfwDestroyWindow(window_);
        window_ = nullptr;
    }
    
    // Terminate GLFW
    glfwTerminate();

    std::cout << "[WorkbenchEngine] Shutdown complete" << std::endl;
}

// Static callbacks
void WorkbenchEngine::errorCallback(int error, const char* description)
{
    std::cerr << "[GLFW Error " << error << "]: " << description << std::endl;
}

void WorkbenchEngine::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    // Demo system removed
    (void)window; (void)key; (void)scancode; (void)action; (void)mods;
}

void WorkbenchEngine::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
    // Demo system removed
    (void)window; (void)button; (void)action; (void)mods;
}

void WorkbenchEngine::cursorPosCallback(GLFWwindow* window, double xpos, double ypos)
{
    // Demo system removed
    (void)window; (void)xpos; (void)ypos;
}

void WorkbenchEngine::framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    if (instance_) {
        instance_->handleWindowResize(width, height);
    }
}

void WorkbenchEngine::handleWindowResize(int width, int height)
{
    if (renderer_) {
        renderer_->init(width, height);
    }
}

bool WorkbenchEngine::isServiceConnected() const { return metrics_.service_connected; }

void WorkbenchEngine::renderTabs()
{
    if (ImGui::Begin("SEP Workbench", nullptr, ImGuiWindowFlags_MenuBar))
    {
        if (ImGui::BeginTabBar("MainTabBar"))
        {
            if (ImGui::BeginTabItem("Signals"))
            {
                signals_tab_->render();
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("Engine"))
            {
                engine_tab_->render();
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("Backend"))
            {
                backend_tab_->render();
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("Backtester"))
            {
                backtester_tab_->render();
                ImGui::EndTabItem();
            }
            ImGui::EndTabBar();
        }
    }
    ImGui::End();
}


void WorkbenchEngine::updateData()
{
    // Real implementation of data fetching pipeline (DATA.md integration)
    std::deque<common::CandleData> candle_data;
    bool fetched = false;
    if (service_connector_ && service_connector_->getOandaConnector()) {
        auto oanda_connector = service_connector_->getOandaConnector();
        try {
            auto now = std::chrono::system_clock::now();
            auto start = now - std::chrono::hours(48);

            auto now_t = std::chrono::system_clock::to_time_t(now);
            auto start_t = std::chrono::system_clock::to_time_t(start);

            auto historical_candles = oanda_connector->getHistoricalData(
                "EUR_USD", "M1", std::to_string(start_t), std::to_string(now_t), 2880);

            for (const auto& oc : historical_candles) {
                std::tm tm = {};
                std::istringstream ss(oc.time);
                ss >> std::get_time(&tm, "%Y-%m-%dT%H:%M:%S");
                auto ts = std::chrono::system_clock::from_time_t(std::mktime(&tm));
                candle_data.emplace_back(oc.open, oc.high, oc.low, oc.close,
                                        static_cast<int>(oc.volume), ts);
            }

            fetched = !candle_data.empty();
            if (fetched && signals_tab_) {
                signals_tab_->setCandleData(candle_data);
            }

            if (fetched) {
                std::cout << "[WorkbenchEngine] Fetched " << candle_data.size()
                          << " candles from OANDA" << std::endl;
            }

        } catch (const std::exception& e) {
            std::cerr << "[WorkbenchEngine] OANDA data fetch error: " << e.what() << std::endl;
        }
    }

    if (!fetched && service_connector_) {
        candle_data = std::deque<common::CandleData>(service_connector_->getInitialData().begin(),
                                     service_connector_->getInitialData().end());
        if (signals_tab_ && !candle_data.empty()) {
            signals_tab_->setCandleData(candle_data);
            auto init_signals = service_connector_->getInitialSignals();
            if (!init_signals.empty()) {
                signals_tab_->setSEPSignals(init_signals);
            }
        }
        if (!candle_data.empty()) {
            std::cout << "[WorkbenchEngine] Loaded " << candle_data.size()
                      << " candles from local cache" << std::endl;
        }
    }

    // Process SEP signals through pattern metric engine (DATA.md pipeline)
    if (active_engine_ && signals_tab_) {
        try {
            auto* pme = active_engine_->getPatternMetricEngine();
            if (pme) {
                // Get current candle data to process
                auto candle_data = signals_tab_->getCandleData();
                if (!candle_data.empty()) {
                    // Convert candle data to bytes for pattern processing
                    std::vector<uint8_t> price_data;
                    for (const auto& candle : candle_data) {
                        float ohlc[4] = {
                            static_cast<float>(candle.open),
                            static_cast<float>(candle.high),
                            static_cast<float>(candle.low),
                            static_cast<float>(candle.close)
                        };
                        
                        const uint8_t* byte_ptr = reinterpret_cast<const uint8_t*>(ohlc);
                        price_data.insert(price_data.end(), byte_ptr, byte_ptr + sizeof(ohlc));
                    }
                    
                    // Clear previous patterns and ingest new data
                    pme->clear();
                    pme->ingestData(price_data.data(), price_data.size());
                    
                    // Process patterns and compute metrics
                    pme->evolvePatterns();
                    const auto& metrics = pme->computeMetrics();
                    
                    // Convert metrics to SEP signals
                    std::deque<common::SEPSignalData> sep_signals;
                    for (size_t i = 0; i < std::min(metrics.size(), candle_data.size()); i++) {
                        const auto& metric = metrics[i];
                        const auto& candle = candle_data[candle_data.size() - metrics.size() + i];
                        
                        common::SEPSignalData signal;
                        signal.coherence = metric.coherence;
                        signal.stability = metric.stability;
                        signal.entropy = metric.entropy;
                        signal.alpha_signal = (metric.coherence + metric.stability - metric.entropy) / 2.0f;
                        signal.trend_strength = (metric.coherence * metric.stability) - metric.entropy;
                        signal.timestamp = candle.timestamp;
                        
                        // Threshold-based signal classification (TODO.md Phase 1.3)
                        if (metric.coherence > 0.8f && metric.stability > 0.7f && metric.entropy < 0.2f) {
                            signal.signal_type = common::MultiTimeframeSignal::STRONG_BUY;
                        } else if (metric.coherence > 0.7f && metric.stability > 0.6f && metric.entropy < 0.3f) {
                            signal.signal_type = common::MultiTimeframeSignal::BUY;
                        } else if (metric.coherence < 0.2f && metric.stability < 0.3f && metric.entropy > 0.8f) {
                            signal.signal_type = common::MultiTimeframeSignal::STRONG_SELL;
                        } else if (metric.coherence < 0.3f && metric.stability < 0.4f && metric.entropy > 0.7f) {
                            signal.signal_type = common::MultiTimeframeSignal::SELL;
                        } else {
                            signal.signal_type = common::MultiTimeframeSignal::NEUTRAL;
                        }
                        
                        sep_signals.push_back(signal);
                    }
                    
                    // Update signals tab with processed SEP signals
                    signals_tab_->setSEPSignals(sep_signals);
                    
                    std::cout << "[WorkbenchEngine] Generated " << sep_signals.size() 
                              << " SEP signals from " << metrics.size() << " patterns" << std::endl;
                }
            }
        } catch (const std::exception& e) {
            std::cerr << "[WorkbenchEngine] SEP signal processing error: " << e.what() << std::endl;
        }
    }
}

void WorkbenchEngine::setupOandaCallbacks(connectors::OandaConnector* oanda_ptr)
{
    if (!oanda_ptr)
        return;

    oanda_ptr->setPriceCallback([this](const connectors::MarketData& md) {
        if (metrics_monitor_) {
            metrics_monitor_->setLatestMarketData(md);
        }

        if (active_engine_) {
            auto* pme = active_engine_->getPatternMetricEngine();
            if (pme) {
                float price = static_cast<float>(md.mid);
                pme->ingestData(reinterpret_cast<const uint8_t*>(&price), sizeof(price));
                pme->evolvePatterns();
                pme->computeMetrics();
            }
        }

        if (multi_timeframe_analyzer_) {
            common::CandleData tick_candle{
                md.mid, md.mid, md.mid, md.mid, md.volume,
                std::chrono::time_point<std::chrono::system_clock>(
                    std::chrono::nanoseconds(md.timestamp))};
            multi_timeframe_analyzer_->ingestMarketData(md.instrument, tick_candle);
        }
    });

    oanda_ptr->setCandleCallback([this](const common::CandleData& c) {
        if (signals_tab_) {
            signals_tab_->addCandle(c);
        }
        if (multi_timeframe_analyzer_) {
            multi_timeframe_analyzer_->ingestMarketData("EUR_USD", c);
        }
    });
}

} // namespace sep::workbench