#include "demo_orchestrator.hpp"

#include <GLFW/glfw3.h>

#include <chrono>
#include <iostream>

#include "../simple_renderer.h"
#include "demos/demo_base.hpp"
#include "demos/demo_manager.hpp"
#include "imgui.h"

namespace sep::workbench {

// Static members
std::unordered_map<std::string, DemoOrchestrator::DemoFactory> DemoOrchestrator::demo_factories_;
bool DemoOrchestrator::factories_registered_ = false;

DemoOrchestrator::DemoOrchestrator() : DemoOrchestrator(OrchestratorConfig{}) {}

DemoOrchestrator::DemoOrchestrator(const OrchestratorConfig& config) 
    : config_(config) {
    
    // Register demo factories if not already done
    if (!factories_registered_) {
        registerDemoFactories();
        factories_registered_ = true;
    }
    
    metrics_.start_time = std::chrono::steady_clock::now();
}

DemoOrchestrator::~DemoOrchestrator() {
    unloadCurrentDemo();
}

void DemoOrchestrator::registerDemoFactories() {
    // Get registered demos from DemoManager
    auto& manager = DemoManager::getInstance();
    auto demo_names = manager.getRegisteredDemos();
    
    std::cout << "[DemoOrchestrator] Registering " << demo_names.size() << " demo factories" << std::endl;
    
    // Note: In a real implementation, we'd have the factories from demo registration
    // For now, this acts as a bridge to the existing DemoManager system
}

bool DemoOrchestrator::loadDemo(const std::string& demo_id, sep::core::Engine* engine,
                                sep::CyclesRenderer* renderer)
{
    std::lock_guard<std::mutex> lock(state_mutex_);
    
    std::cout << "[DemoOrchestrator] Loading demo: " << demo_id << std::endl;
    
    // Unload current demo if any
    if (current_demo_) {
        unloadCurrentDemo();
    }
    
    setState(DemoState::LOADING);
    
    try {
        // Store engine references
        engine_ = engine;
        renderer_ = renderer;

        // Use DemoManager to create the demo
        auto& manager = DemoManager::getInstance();
        // CyclesRenderer is a type alias for SimpleRenderer, so we can safely cast
        sep::SimpleRenderer* simple_renderer = static_cast<sep::SimpleRenderer*>(renderer);
        manager.initialize(engine, simple_renderer);

        if (!manager.switchToDemo(demo_id)) {
            throw std::runtime_error("Failed to create demo: " + demo_id);
        }
        
        // Store demo info
        current_demo_id_ = demo_id;
        
        // Reset metrics
        resetMetrics();
        
        setState(DemoState::LOADED);
        setState(DemoState::RUNNING);
        
        std::cout << "[DemoOrchestrator] Demo loaded successfully: " << demo_id << std::endl;
        return true;
        
    } catch (const std::exception& e) {
        handleDemoError(std::string("Failed to load demo: ") + e.what());
        setState(DemoState::ERROR);
        return false;
    }
}

void DemoOrchestrator::unloadCurrentDemo() {
    std::lock_guard<std::mutex> lock(state_mutex_);
    
    if (current_state_ == DemoState::UNLOADED) {
        return;
    }
    
    std::cout << "[DemoOrchestrator] Unloading demo: " << current_demo_id_ << std::endl;
    
    setState(DemoState::UNLOADING);
    
    try {
        // Use DemoManager to unload
        auto& manager = DemoManager::getInstance();
        manager.on_unload();
        
        current_demo_id_.clear();
        engine_ = nullptr;
        renderer_ = nullptr;
        
        setState(DemoState::UNLOADED);
        
        std::cout << "[DemoOrchestrator] Demo unloaded successfully" << std::endl;
        
    } catch (const std::exception& e) {
        handleDemoError(std::string("Error during unload: ") + e.what());
    }
}

bool DemoOrchestrator::reloadCurrentDemo() {
    if (current_demo_id_.empty()) {
        return false;
    }
    
    std::string demo_id = current_demo_id_;
    sep::core::Engine* engine = engine_;
    sep::CyclesRenderer* renderer = renderer_;
    
    unloadCurrentDemo();
    return loadDemo(demo_id, engine, renderer);
}

void DemoOrchestrator::update(float delta_time) {
    if (current_state_ != DemoState::RUNNING) {
        return;
    }
    
    update_start_ = std::chrono::steady_clock::now();
    
    try {
        // Profile update if enabled
        if (config_.enable_profiling) {
            profileUpdate(delta_time);
        }
        
        // Use DemoManager for update
        auto& manager = DemoManager::getInstance();
        manager.on_update(delta_time);
        
        updateMetrics(delta_time);
        checkPerformance();
        
    } catch (const std::exception& e) {
        handleDemoError(std::string("Update error: ") + e.what());
    }
}

void DemoOrchestrator::render() {
    if (current_state_ != DemoState::RUNNING && current_state_ != DemoState::PAUSED) {
        return;
    }
    
    render_start_ = std::chrono::steady_clock::now();
    
    try {
        // Profile render if enabled
        if (config_.enable_profiling) {
            profileRender();
        }
        
        // Use DemoManager for rendering
        auto& manager = DemoManager::getInstance();
        manager.on_render();
        
    } catch (const std::exception& e) {
        handleDemoError(std::string("Render error: ") + e.what());
    }
}

void DemoOrchestrator::renderUI() {
    if (current_state_ == DemoState::UNLOADED || current_state_ == DemoState::UNLOADING) {
        return;
    }
    
    try {
        // Render demo UI
        auto& manager = DemoManager::getInstance();
        manager.on_ui_render();
        
        // Render debug overlay if profiling enabled
        if (config_.enable_profiling) {
            renderProfilingOverlay();
        }
        
    } catch (const std::exception& e) {
        handleDemoError(std::string("UI render error: ") + e.what());
    }
}

void DemoOrchestrator::renderProfilingOverlay() {
    ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(300, 200), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowBgAlpha(0.8f);
    
    if (ImGui::Begin("Demo Profiler", nullptr, ImGuiWindowFlags_NoFocusOnAppearing)) {
        ImGui::Text("Demo: %s", current_demo_id_.c_str());
        ImGui::Text("State: %s", getDemoStateName(current_state_));
        
        ImGui::Separator();
        
        ImGui::Text("Performance:");
        ImGui::Text("Update: %.2f ms", metrics_.average_update_time);
        ImGui::Text("Render: %.2f ms", metrics_.average_render_time);
        ImGui::Text("Total: %.2f ms", metrics_.average_update_time + metrics_.average_render_time);
        
        float fps = 1000.0f / (metrics_.average_update_time + metrics_.average_render_time);
        ImGui::Text("FPS: %.1f", fps);
        
        ImGui::Separator();
        
        ImGui::Text("Frames: %lu", metrics_.frame_count);
        ImGui::Text("Memory: %.2f MB", metrics_.memory_usage / (1024.0f * 1024.0f));
        
        if (!metrics_.last_error.empty()) {
            ImGui::Separator();
            ImGui::TextColored(ImVec4(1.0f, 0.2f, 0.2f, 1.0f), "Error:");
            ImGui::TextWrapped("%s", metrics_.last_error.c_str());
        }
    }
    ImGui::End();
}

void DemoOrchestrator::pauseDemo() {
    std::lock_guard<std::mutex> lock(state_mutex_);
    
    if (current_state_ == DemoState::RUNNING) {
        setState(DemoState::PAUSED);
        std::cout << "[DemoOrchestrator] Demo paused" << std::endl;
    }
}

void DemoOrchestrator::resumeDemo() {
    std::lock_guard<std::mutex> lock(state_mutex_);
    
    if (current_state_ == DemoState::PAUSED) {
        setState(DemoState::RUNNING);
        std::cout << "[DemoOrchestrator] Demo resumed" << std::endl;
    }
}

void DemoOrchestrator::resetDemo() {
    if (!current_demo_id_.empty()) {
        reloadCurrentDemo();
    }
}

void DemoOrchestrator::handleKeyPress(int key, int /*scancode*/, int action, int mods) {
    if (current_state_ != DemoState::RUNNING) {
        return;
    }
    
    // Global demo controls
    if (action == GLFW_PRESS) {
        switch (key) {
            case GLFW_KEY_P:
                if (current_state_ == DemoState::RUNNING) {
                    pauseDemo();
                } else if (current_state_ == DemoState::PAUSED) {
                    resumeDemo();
                }
                return;
                
            case GLFW_KEY_R:
                if (mods & GLFW_MOD_CONTROL) {
                    resetDemo();
                    return;
                }
                break;
        }
    }
    
    // Pass to demo
    auto& manager = DemoManager::getInstance();
    manager.on_key(key);
}

void DemoOrchestrator::handleMouseButton(int /*button*/, int /*action*/, int /*mods*/, double /*x*/, double /*y*/) {
    if (current_state_ != DemoState::RUNNING) {
        return;
    }
    
    // Mouse handling would be passed to demo
    // For now, DemoManager handles it differently
}

void DemoOrchestrator::handleMouseMove(double /*x*/, double /*y*/) {
    if (current_state_ != DemoState::RUNNING) {
        return;
    }
    
    // Mouse move handling
}

void DemoOrchestrator::handleScroll(double /*x_offset*/, double /*y_offset*/) {
    if (current_state_ != DemoState::RUNNING) {
        return;
    }
    
    // Scroll handling
}

void DemoOrchestrator::setState(DemoState new_state) {
    if (current_state_ != new_state) {
        std::cout << "[DemoOrchestrator] State change: " 
                  << getDemoStateName(current_state_) << " -> " 
                  << getDemoStateName(new_state) << std::endl;
        current_state_ = new_state;
    }
}

void DemoOrchestrator::updateMetrics(float /*delta_time*/) {
    metrics_.frame_count++;
    metrics_.last_update = std::chrono::steady_clock::now();
    
    // Update moving averages
    float alpha = 0.95f; // Smoothing factor
    
    auto update_end = std::chrono::steady_clock::now();
    float update_time = std::chrono::duration<float, std::milli>(update_end - update_start_).count();
    metrics_.average_update_time = metrics_.average_update_time * alpha + update_time * (1.0f - alpha);
}

void DemoOrchestrator::checkPerformance() {
    // Check if update time exceeds target
    if (metrics_.average_update_time > config_.max_update_time_ms) {
        std::cout << "[DemoOrchestrator] WARNING: Update time exceeds target: " 
                  << metrics_.average_update_time << " ms" << std::endl;
    }
    
    // Check if render time exceeds target
    if (metrics_.average_render_time > config_.max_render_time_ms) {
        std::cout << "[DemoOrchestrator] WARNING: Render time exceeds target: " 
                  << metrics_.average_render_time << " ms" << std::endl;
    }
}

void DemoOrchestrator::handleDemoError(const std::string& error) {
    std::cerr << "[DemoOrchestrator] ERROR: " << error << std::endl;
    metrics_.last_error = error;
    
    if (config_.enable_error_recovery) {
        // Attempt to recover
        setState(DemoState::ERROR);
        
        // Could implement auto-reload or other recovery strategies
    }
}

void DemoOrchestrator::profileUpdate(float /*delta_time*/) {
    // Additional profiling logic
}

void DemoOrchestrator::profileRender() {
    auto render_end = std::chrono::steady_clock::now();
    float render_time = std::chrono::duration<float, std::milli>(render_end - render_start_).count();
    
    float alpha = 0.95f;
    metrics_.average_render_time = metrics_.average_render_time * alpha + render_time * (1.0f - alpha);
}

void DemoOrchestrator::resetMetrics() {
    metrics_ = DemoMetrics{};
    metrics_.start_time = std::chrono::steady_clock::now();
    metrics_.last_update = metrics_.start_time;
}

const char* DemoOrchestrator::getDemoStateName(DemoState state) {
    switch (state) {
        case DemoState::UNLOADED: return "Unloaded";
        case DemoState::LOADING: return "Loading";
        case DemoState::LOADED: return "Loaded";
        case DemoState::RUNNING: return "Running";
        case DemoState::PAUSED: return "Paused";
        case DemoState::UNLOADING: return "Unloading";
        case DemoState::ERROR: return "Error";
        default: return "Unknown";
    }
}

} // namespace sep::workbench