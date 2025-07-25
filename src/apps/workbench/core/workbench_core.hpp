#pragma once

#include "std_includes.h"
#include "tabs/signals_tab_controller.h"
#include "tabs/engine_tab_controller.h"
#include "tabs/backend_tab_controller.h"
#include "ui_layout_manager.h"
#include "multi_timeframe_analyzer.h"

// Forward declaration for GLFW
struct GLFWwindow;

// External classes from sep namespace (not in sep::workbench)
namespace sep {
    namespace core
    {
        class Engine;
    }
}  // namespace sep

namespace sep::workbench {

// Forward declarations
class ServiceConnector;
class LandingPage;
class Renderer;
class QuantumSignalGenerator;
class MetricsMonitor;

enum class ApplicationState {
    INITIALIZING,
    SERVICE_CHECK,
    LANDING_PAGE,
    DEMO_SELECTION,
    DEMO_RUNNING,
    ERROR_RECOVERY,
    SHUTTING_DOWN
};

struct ApplicationMetrics {
    bool service_connected{false};
    std::chrono::steady_clock::time_point startup_time;
    std::string current_demo;
    size_t frame_count{0};
    float average_fps{0.0f};
    std::string last_error;
};

class WorkbenchEngine
{
public:
    WorkbenchEngine();
    ~WorkbenchEngine();

    // Lifecycle management
    bool initialize();
    void run();
    void shutdown();

    // State management
    ApplicationState getCurrentState() const { return current_state_; }
    void transitionTo(ApplicationState new_state);

    // Service connectivity
    bool isServiceConnected() const;
    void attemptServiceConnection();
    
    // Window management
    GLFWwindow* getWindow() const { return window_; }
    void handleWindowResize(int width, int height);
    
    // Error handling
    void reportError(const std::string& error);
    std::string getLastError() const { return metrics_.last_error; }
    
    // Metrics
    const ApplicationMetrics& getMetrics() const { return metrics_; }

    // Demo management
    void selectDemo(const std::string& demo_name);
    void stopCurrentDemo();
    
    // Metrics dashboard
    void showMetricsDashboard(bool show);

    // Signal generator
    QuantumSignalGenerator* getSignalGenerator() const { return signal_generator_.get(); }
    sep::quantum::PatternMetricEngine* getPatternMetricEngine() const { return active_engine_ ? active_engine_->getPatternMetricEngine() : nullptr; }

    // Static callbacks for GLFW
    static void errorCallback(int error, const char* description);
    static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
    static void cursorPosCallback(GLFWwindow* window, double xpos, double ypos);
    static void framebufferSizeCallback(GLFWwindow* window, int width, int height);

private:
    // Engine state
    std::atomic<ApplicationState> current_state_{ApplicationState::INITIALIZING};
    std::atomic<bool> should_exit_{false};
    ApplicationMetrics metrics_;

    // Engine components
    GLFWwindow* window_{nullptr};
    std::unique_ptr<ServiceConnector> service_connector_;
    // Demo orchestrator removed for trading-focused version
    std::unique_ptr<LandingPage> landing_page_;
    std::unique_ptr<Renderer> renderer_;
    std::unique_ptr<UILayoutManager> layout_manager_;
    std::unique_ptr<SignalsTabController> signals_tab_;
    std::unique_ptr<EngineTabController> engine_tab_;
    std::unique_ptr<BackendTabController> backend_tab_;
    std::unique_ptr<QuantumSignalGenerator> signal_generator_;
    std::unique_ptr<MetricsMonitor> metrics_monitor_;
    std::unique_ptr<MultiTimeframeAnalyzer> multi_timeframe_analyzer_;
    
    // Engine components (may be null if service not connected)
    std::unique_ptr<sep::core::Engine> offline_engine_;
    sep::core::Engine* active_engine_{nullptr};

    // Window configuration
    struct WindowConfig {
        std::string title{"SEP Workbench"};
        int width{1280};
        int height{720};
        bool vsync{true};
        bool fullscreen{false};
    } window_config_;

    // Internal methods
    bool initializeGLFW();
    bool createWindow();
    bool initializeOpenGL();
    bool initializeImGui();
    void cleanupImGui();
    
    void processInput();
    void updateFrame(float delta_time);
    void renderFrame();
    void updateData();
    
    void handleStateTransition();
    void updateMetrics(float delta_time);
    
    // Rendering functions
    void renderLoadingScreen();
    void renderErrorRecovery();
    void renderStatusBar();
    void renderTabs();
    
    // State handlers
    void handleInitializing();
    void handleServiceCheck();
    void handleLandingPage();
    void handleDemoSelection();
    void handleDemoRunning();
    void handleErrorRecovery();
    
    // Singleton instance for static callbacks
    static WorkbenchEngine* instance_;
};

} // namespace sep::workbench