#pragma once

#include <memory>
#include <atomic>
#include <string>
#include <chrono>
#include <functional>
#include <GLFW/glfw3.h>

namespace sep::workbench {

// Forward declarations
class ServiceConnector;
class DemoOrchestrator;
class LandingPage;
class Renderer;
namespace sep { class Engine; class CyclesRenderer; }

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

class WorkbenchCore {
public:
    WorkbenchCore();
    ~WorkbenchCore();

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

    // Static callbacks for GLFW
    static void errorCallback(int error, const char* description);
    static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
    static void cursorPosCallback(GLFWwindow* window, double xpos, double ypos);
    static void framebufferSizeCallback(GLFWwindow* window, int width, int height);

private:
    // Core state
    std::atomic<ApplicationState> current_state_{ApplicationState::INITIALIZING};
    std::atomic<bool> should_exit_{false};
    ApplicationMetrics metrics_;

    // Core components
    GLFWwindow* window_{nullptr};
    std::unique_ptr<ServiceConnector> service_connector_;
    std::unique_ptr<DemoOrchestrator> demo_orchestrator_;
    std::unique_ptr<LandingPage> landing_page_;
    std::unique_ptr<Renderer> renderer_;
    
    // Engine components (may be null if service not connected)
    std::unique_ptr<sep::Engine> offline_engine_;
    sep::Engine* active_engine_{nullptr};
    std::unique_ptr<sep::CyclesRenderer> cycles_renderer_;

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
    
    void handleStateTransition();
    void updateMetrics(float delta_time);
    
    // State handlers
    void handleInitializing();
    void handleServiceCheck();
    void handleLandingPage();
    void handleDemoSelection();
    void handleDemoRunning();
    void handleErrorRecovery();
    
    // Singleton instance for static callbacks
    static WorkbenchCore* instance_;
};

} // namespace sep::workbench