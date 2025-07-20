#include "workbench_core.hpp"

#include "demo_orchestrator.hpp"
#include "engine/cycles_renderer.h"
#include "engine/engine.h"
#include "glad/glad.h"
#include "landing_page.hpp"
#include "service_connector.hpp"

// Include non-OpenGL headers
#include <imgui.h>

#include "imgui_internal.h"

// Use correct paths for ImGui implementation files
#include "../../third_party/imgui/backends/imgui_impl_glfw.h"
#include "../../third_party/imgui/backends/imgui_impl_opengl3.h"
#include <iostream>
#include <thread>
#include <chrono>

namespace sep::workbench {

// Static instance for GLFW callbacks
WorkbenchCore* WorkbenchCore::instance_ = nullptr;

WorkbenchCore::WorkbenchCore() {
    instance_ = this;
    metrics_.startup_time = std::chrono::steady_clock::now();
}

WorkbenchCore::~WorkbenchCore() {
    shutdown();
    instance_ = nullptr;
}

bool WorkbenchCore::initialize() {
    std::cout << "[WorkbenchCore] Starting initialization sequence..." << std::endl;
    
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
        std::cout << "[WorkbenchCore] Initializing core components..." << std::endl;
        
        service_connector_ = std::make_unique<ServiceConnector>();
        demo_orchestrator_ = std::make_unique<DemoOrchestrator>();
        landing_page_ = std::make_unique<LandingPage>(this);
        renderer_ = std::make_unique<Renderer>();
        
        // Initialize renderer
        int width, height;
        glfwGetFramebufferSize(window_, &width, &height);
        renderer_->init(width, height);
        
        // Create offline engine as fallback
        std::cout << "[WorkbenchCore] Creating offline engine..." << std::endl;
        offline_engine_ = std::make_unique<sep::Engine>();
        offline_engine_->initialize();
        active_engine_ = offline_engine_.get();
        
        // Create Cycles renderer
        cycles_renderer_ = std::make_unique<sep::CyclesRenderer>();
        
        // Transition to service check
        transitionTo(ApplicationState::SERVICE_CHECK);
        
        std::cout << "[WorkbenchCore] Initialization complete!" << std::endl;
        return true;
        
    } catch (const std::exception& e) {
        reportError(std::string("Initialization exception: ") + e.what());
        return false;
    }
}

bool WorkbenchCore::initializeGLFW() {
    glfwSetErrorCallback(errorCallback);
    
    if (!glfwInit()) {
        std::cerr << "[WorkbenchCore] Failed to initialize GLFW" << std::endl;
        return false;
    }
    
    std::cout << "[WorkbenchCore] GLFW initialized successfully" << std::endl;
    return true;
}

bool WorkbenchCore::createWindow() {
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
        std::cerr << "[WorkbenchCore] Failed to create GLFW window" << std::endl;
        return false;
    }
    
    glfwMakeContextCurrent(window_);
    glfwSwapInterval(window_config_.vsync ? 1 : 0);
    
    // Set callbacks
    glfwSetKeyCallback(window_, keyCallback);
    glfwSetMouseButtonCallback(window_, mouseButtonCallback);
    glfwSetCursorPosCallback(window_, cursorPosCallback);
    glfwSetFramebufferSizeCallback(window_, framebufferSizeCallback);
    
    std::cout << "[WorkbenchCore] Window created successfully" << std::endl;
    return true;
}

bool WorkbenchCore::initializeOpenGL() {
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "[WorkbenchCore] Failed to initialize GLAD" << std::endl;
        return false;
    }
    
    std::cout << "[WorkbenchCore] OpenGL Info:" << std::endl;
    std::cout << "  Version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "  Vendor: " << glGetString(GL_VENDOR) << std::endl;
    std::cout << "  Renderer: " << glGetString(GL_RENDERER) << std::endl;
    
    // Configure OpenGL state
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    return true;
}

bool WorkbenchCore::initializeImGui() {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    
    // Setup style
    ImGui::StyleColorsDark();
    
    // Setup platform/renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(window_, true);
    ImGui_ImplOpenGL3_Init("#version 330");
    
    std::cout << "[WorkbenchCore] ImGui initialized successfully" << std::endl;
    return true;
}

void WorkbenchCore::cleanupImGui() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void WorkbenchCore::run() {
    std::cout << "[WorkbenchCore] Starting main loop..." << std::endl;
    
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

void WorkbenchCore::processInput() {
    // ESC to exit
    if (glfwGetKey(window_, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        if (current_state_ == ApplicationState::DEMO_RUNNING) {
            stopCurrentDemo();
        } else if (current_state_ == ApplicationState::LANDING_PAGE) {
            should_exit_ = true;
        }
    }
    
    // F1 for help
    if (glfwGetKey(window_, GLFW_KEY_F1) == GLFW_PRESS) {
        // Show help overlay
    }
}

void WorkbenchCore::updateFrame(float delta_time) {
    // Handle state-specific updates
    handleStateTransition();
    
    // Update demo if running
    if (current_state_ == ApplicationState::DEMO_RUNNING && demo_orchestrator_) {
        demo_orchestrator_->update(delta_time);
    }
}

void WorkbenchCore::renderFrame() {
    // Clear
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // Start ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    
    // Render based on state
    switch (current_state_.load()) {
        case ApplicationState::LANDING_PAGE:
        case ApplicationState::DEMO_SELECTION:
            if (landing_page_) {
                landing_page_->render();
            }
            break;
            
        case ApplicationState::DEMO_RUNNING:
            if (demo_orchestrator_) {
                demo_orchestrator_->render();
                demo_orchestrator_->renderUI();
            }
            break;
            
        case ApplicationState::ERROR_RECOVERY:
            renderErrorRecovery();
            break;
            
        default:
            renderLoadingScreen();
            break;
    }
    
    // Render status bar
    renderStatusBar();
    
    // Render ImGui
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void WorkbenchCore::renderLoadingScreen() {
    ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x * 0.5f, 
                                   ImGui::GetIO().DisplaySize.y * 0.5f), 
                           ImGuiCond_Always, ImVec2(0.5f, 0.5f));
    ImGui::Begin("Loading", nullptr, ImGuiWindowFlags_NoTitleBar | 
                                     ImGuiWindowFlags_NoResize | 
                                     ImGuiWindowFlags_AlwaysAutoResize);
    
    ImGui::Text("Initializing SEP Workbench...");
    
    // Spinner
    float time = ImGui::GetTime();
    ImGui::Spinner("##spinner", 16, 3, ImGui::GetColorU32(ImGuiCol_ButtonActive));
    
    ImGui::End();
}

void WorkbenchCore::renderErrorRecovery() {
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

void WorkbenchCore::renderStatusBar() {
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

void WorkbenchCore::handleStateTransition() {
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

void WorkbenchCore::handleInitializing() {
    // Initialization is handled in initialize()
}

void WorkbenchCore::handleServiceCheck() {
    std::cout << "[WorkbenchCore] Checking for SEP service..." << std::endl;
    
    // Attempt to connect to service
    attemptServiceConnection();
    
    // For now, proceed to landing page regardless
    transitionTo(ApplicationState::LANDING_PAGE);
}

void WorkbenchCore::handleLandingPage() {
    // Landing page handles its own logic
}

void WorkbenchCore::handleDemoSelection() {
    // Demo selection is handled by landing page
}

void WorkbenchCore::handleDemoRunning() {
    // Demo running is handled by demo orchestrator
}

void WorkbenchCore::handleErrorRecovery() {
    // Error recovery UI is rendered in renderErrorRecovery()
}

void WorkbenchCore::attemptServiceConnection() {
    if (service_connector_) {
        metrics_.service_connected = service_connector_->connect();
        
        if (metrics_.service_connected) {
            std::cout << "[WorkbenchCore] Successfully connected to SEP service" << std::endl;
            // Switch to service engine
            active_engine_ = service_connector_->getEngine();
        } else {
            std::cout << "[WorkbenchCore] Failed to connect to SEP service, using offline mode" << std::endl;
            active_engine_ = offline_engine_.get();
        }
    }
}

void WorkbenchCore::transitionTo(ApplicationState new_state) {
    ApplicationState old_state = current_state_.load();
    std::cout << "[WorkbenchCore] State transition: " << static_cast<int>(old_state) 
              << " -> " << static_cast<int>(new_state) << std::endl;
    
    current_state_ = new_state;
}

void WorkbenchCore::selectDemo(const std::string& demo_name) {
    std::cout << "[WorkbenchCore] Selecting demo: " << demo_name << std::endl;
    
    if (demo_orchestrator_) {
        bool success = demo_orchestrator_->loadDemo(demo_name, active_engine_, cycles_renderer_.get());
        
        if (success) {
            metrics_.current_demo = demo_name;
            transitionTo(ApplicationState::DEMO_RUNNING);
        } else {
            reportError("Failed to load demo: " + demo_name);
            transitionTo(ApplicationState::ERROR_RECOVERY);
        }
    }
}

void WorkbenchCore::stopCurrentDemo() {
    std::cout << "[WorkbenchCore] Stopping current demo" << std::endl;
    
    if (demo_orchestrator_) {
        demo_orchestrator_->unloadCurrentDemo();
    }
    
    metrics_.current_demo.clear();
    transitionTo(ApplicationState::LANDING_PAGE);
}

void WorkbenchCore::updateMetrics(float delta_time) {
    metrics_.frame_count++;
    
    // Update FPS (simple moving average)
    float fps = 1.0f / delta_time;
    metrics_.average_fps = metrics_.average_fps * 0.95f + fps * 0.05f;
}

void WorkbenchCore::reportError(const std::string& error) {
    std::cerr << "[WorkbenchCore] ERROR: " << error << std::endl;
    metrics_.last_error = error;
}

void WorkbenchCore::shutdown() {
    std::cout << "[WorkbenchCore] Shutting down..." << std::endl;
    
    current_state_ = ApplicationState::SHUTTING_DOWN;
    
    // Clean up components
    if (demo_orchestrator_) {
        demo_orchestrator_->unloadCurrentDemo();
    }
    
    // Clean up ImGui
    cleanupImGui();
    
    // Clean up window
    if (window_) {
        glfwDestroyWindow(window_);
        window_ = nullptr;
    }
    
    // Terminate GLFW
    glfwTerminate();
    
    std::cout << "[WorkbenchCore] Shutdown complete" << std::endl;
}

// Static callbacks
void WorkbenchCore::errorCallback(int error, const char* description) {
    std::cerr << "[GLFW Error " << error << "]: " << description << std::endl;
}

void WorkbenchCore::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (instance_ && instance_->demo_orchestrator_ && 
        instance_->current_state_ == ApplicationState::DEMO_RUNNING) {
        instance_->demo_orchestrator_->handleKeyPress(key, scancode, action, mods);
    }
}

void WorkbenchCore::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    if (instance_ && instance_->demo_orchestrator_ && 
        instance_->current_state_ == ApplicationState::DEMO_RUNNING) {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        instance_->demo_orchestrator_->handleMouseButton(button, action, mods, xpos, ypos);
    }
}

void WorkbenchCore::cursorPosCallback(GLFWwindow* window, double xpos, double ypos) {
    if (instance_ && instance_->demo_orchestrator_ && 
        instance_->current_state_ == ApplicationState::DEMO_RUNNING) {
        instance_->demo_orchestrator_->handleMouseMove(xpos, ypos);
    }
}

void WorkbenchCore::framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
    if (instance_) {
        instance_->handleWindowResize(width, height);
    }
}

void WorkbenchCore::handleWindowResize(int width, int height) {
    if (renderer_) {
        renderer_->init(width, height);
    }
}

bool WorkbenchCore::isServiceConnected() const {
    return metrics_.service_connected;
}

} // namespace sep::workbench