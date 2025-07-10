#include "sep_engine_wrapper.h"

// Only compile these implementations when NOT in demo mode
#ifndef SEP_DEMO_MODE

namespace sep {

// Concrete implementation of Engine for non-demo mode
class ConcreteEngine : public Engine {
public:
    bool initialize() override { return true; }
    void shutdown() override {}
    void setCudaEnabled(bool enabled) override { (void)enabled; }
    void setMetricsEnabled(bool enabled) override { (void)enabled; }
    void setLogLevel(int level) override { (void)level; }
};

// Concrete implementation of CyclesRenderer for non-demo mode
class ConcreteCyclesRenderer : public CyclesRenderer {
public:
    bool initialize() override { return true; }
    void render() override {}
    void present() override {}
    bool shouldClose() override { return false; }

    void setWindowTitle(const std::string& title) override { (void)title; }
    void setWindowSize(int width, int height) override {
        (void)width;
        (void)height;
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
    void renderPatternState(const std::vector<glm::vec3>& patterns) override { (void)patterns; }
    void renderConnection(const glm::vec3& start, const glm::vec3& end, float strength) override {
        (void)start;
        (void)end;
        (void)strength;
    }
};

// Factory function implementations for non-demo mode
std::unique_ptr<Engine> createEngine() {
    return std::unique_ptr<Engine>(new ConcreteEngine());
}

std::unique_ptr<CyclesRenderer> createRenderer() {
    return std::unique_ptr<CyclesRenderer>(new ConcreteCyclesRenderer());
}

} // namespace sep
#endif // SEP_DEMO_MODE