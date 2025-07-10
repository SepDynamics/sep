#pragma once

#include <vector>
#include <string>
#include <glm/vec3.hpp>

#include "sep_engine_wrapper.h"
#include "renderer.h"

namespace sep {
namespace workbench {

/**
 * Adapter class that makes a sep::workbench::Renderer compatible with the sep::CyclesRenderer interface
 * This allows the demo code to work with either a real CyclesRenderer or our simplified Renderer
 */
class CyclesRendererAdapter : public sep::CyclesRenderer {
public:
    CyclesRendererAdapter(Renderer* renderer) : renderer_(renderer) {}
    ~CyclesRendererAdapter() override = default;

    // CyclesRenderer interface implementation
    bool initialize() override { return true; }
    void render() override { if (renderer_) renderer_->render(); }
    void present() override {}
    bool shouldClose() override { return false; }

    void setWindowTitle(const std::string& title) override { (void)title; }
    void setWindowSize(int width, int height) override { 
        if (renderer_) renderer_->init(width, height); 
    }
    void setFullscreen(bool fullscreen) override { (void)fullscreen; }
    void setVSync(bool vsync) override { (void)vsync; }
    void setSamples(int samples) override { (void)samples; }
    void setDenoising(bool denoising) override { (void)denoising; }
    void setDevice(const std::string& device) override { (void)device; }

    bool hasKeyEvent() override { return false; }
    unsigned char getLastKey() override { return 0; }
    bool hasMouseEvent() override { return false; }
    void getLastMouseEvent(int& x, int& y, int& button) override { x = y = button = 0; }

    void setColorMode(const std::string& mode) override { (void)mode; }
    void setEmissionMode(const std::string& mode) override { (void)mode; }
    void setRotation(float rotation) override { (void)rotation; }
    void setZoom(float zoom) override { (void)zoom; }
    void setWireframe(bool wireframe) override { (void)wireframe; }
    void setRoughnessMode(int mode) override { (void)mode; }
    void cycleColorMode() override {}

    // Convert glm::vec3 to Pattern for our renderer
    void renderPatternState(const std::vector<glm::vec3>& patterns) override {
        if (!renderer_) return;
        
        std::vector<Pattern> workbench_patterns;
        for (const auto& p : patterns) {
            Pattern pattern;
            // Use x, y, z components to populate pattern values
            pattern.values = {p.x, p.y, p.z};
            // Set coherence based on z component (often used as coherence value)
            pattern.quantum_state.coherence = p.z;
            workbench_patterns.push_back(pattern);
        }
        
        renderer_->render(workbench_patterns);
    }

    void renderConnection(const glm::vec3& start, const glm::vec3& end, float strength) override {
        (void)start;
        (void)end;
        (void)strength;
    }

private:
    Renderer* renderer_ = nullptr;
};

} // namespace workbench
} // namespace sep