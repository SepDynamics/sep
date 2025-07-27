#pragma once

#include <glm/vec3.hpp>
#include <vector>

#include "demo_base.hpp"
#include "demo_manager.hpp"
#include "quantum/pattern_metric_engine.h"
#include "imgui.h"

// Forward declarations
namespace sep {
namespace core {
class Engine;
}
}

namespace sep {
namespace workbench {

class CosmoDemo : public Demo {
public:
    void on_load(sep::core::Engine* engine, sep::SimpleRenderer* renderer) override;
    void setPatternMetricEngine(sep::quantum::PatternMetricEngine* engine) override;
    void on_update(float dt) override;
    void on_render() override;
    void on_ui_render() override;
    void on_unload() override;
    void on_key_press(int key) override;
    void on_mouse(int x, int y, int button);

private:
    struct Particle {
        glm::vec3 position;
        glm::vec3 velocity;
        float mass;
    };

    std::vector<Particle> particles_;

    float box_size_{50.0f};
    float time_step_{0.01f};

    sep::core::Engine* engine_{nullptr};
    sep::SimpleRenderer* renderer_{nullptr};
    sep::quantum::PatternMetricEngine* pattern_engine_{nullptr};

    void initParticles();
    void integrate(float dt);
};

} // namespace workbench
} // namespace sep
