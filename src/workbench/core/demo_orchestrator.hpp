#pragma once

#include <chrono>
#include <functional>
#include <memory>
#include <mutex>
#include <string>
#include <unordered_map>

#include "demos/demo_base.hpp"

// Forward declarations
namespace sep {
    namespace core
    {
        class Engine;
    }
    class SimpleRenderer;
}

namespace sep::workbench {

// Forward declarations
class Demo;

// Demo state tracking
enum class DemoState {
    UNLOADED,
    LOADING,
    LOADED,
    RUNNING,
    PAUSED,
    UNLOADING,
    ERROR
};

// Demo execution metrics
struct DemoMetrics {
    std::chrono::steady_clock::time_point start_time;
    std::chrono::steady_clock::time_point last_update;
    uint64_t frame_count{0};
    float average_update_time{0.0f};
    float average_render_time{0.0f};
    size_t memory_usage{0};
    std::string last_error;
};

// Demo orchestration configuration
struct OrchestratorConfig {
    bool enable_profiling{true};
    bool enable_error_recovery{true};
    float max_update_time_ms{16.67f}; // 60 FPS target
    float max_render_time_ms{16.67f};
    size_t max_memory_mb{512};
};

class DemoOrchestrator {
public:
    DemoOrchestrator();
    explicit DemoOrchestrator(const OrchestratorConfig& config);
    ~DemoOrchestrator();

    // Demo lifecycle management
    bool loadDemo(const std::string& demo_id, sep::core::Engine* engine,
                  sep::SimpleRenderer* renderer);
    void unloadCurrentDemo();
    bool reloadCurrentDemo();
    
    // Demo execution
    void update(float delta_time);
    void render();
    void renderUI();
    
    // State management
    DemoState getCurrentState() const { return current_state_; }
    std::string getCurrentDemoId() const { return current_demo_id_; }
    bool isDemoRunning() const { return current_state_ == DemoState::RUNNING; }
    
    // Demo control
    void pauseDemo();
    void resumeDemo();
    void resetDemo();
    
    // Input handling
    void handleKeyPress(int key, int scancode, int action, int mods);
    void handleMouseButton(int button, int action, int mods, double x, double y);
    void handleMouseMove(double x, double y);
    void handleScroll(double x_offset, double y_offset);
    
    // Metrics and profiling
    const DemoMetrics& getMetrics() const { return metrics_; }
    void resetMetrics();
    
    // Error handling
    std::string getLastError() const { return metrics_.last_error; }
    void clearError() { metrics_.last_error.clear(); }
    
    // Configuration
    void setConfig(const OrchestratorConfig& config) { config_ = config; }
    const OrchestratorConfig& getConfig() const { return config_; }

private:
    // Configuration
    OrchestratorConfig config_;
    
    // Current demo state
    std::unique_ptr<Demo> current_demo_;
    std::string current_demo_id_;
    DemoState current_state_{DemoState::UNLOADED};
    
    // Engine references
    sep::core::Engine* engine_{nullptr};
    sep::SimpleRenderer* renderer_{nullptr};
    
    // Metrics
    DemoMetrics metrics_;
    std::chrono::steady_clock::time_point update_start_;
    std::chrono::steady_clock::time_point render_start_;
    
    // Thread safety
    mutable std::mutex state_mutex_;
    
    // Demo factory registry
    using DemoFactory = std::function<std::unique_ptr<Demo>()>;
    static std::unordered_map<std::string, DemoFactory> demo_factories_;
    
    // Internal methods
    void setState(DemoState new_state);
    void updateMetrics(float delta_time);
    void checkPerformance();
    void handleDemoError(const std::string& error);
    void profileUpdate(float delta_time);
    void profileRender();
    void renderProfilingOverlay();
    static const char* getDemoStateName(DemoState state);
    
    // Demo factory registration
    static void registerDemoFactories();
    static bool factories_registered_;
};

} // namespace sep::workbench