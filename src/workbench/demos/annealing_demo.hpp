#include "workbench/demos/demo_base.hpp"
#pragma once
#include "workbench/demos/demo_base.hpp"

#include "workbench/demos/demo_base.hpp"
#include <glm/vec3.hpp>
#include <vector>

#include "demo_manager.hpp"
#include "imgui.h"

namespace sep {
namespace workbench {

class AnnealingDemo : public Demo {
public:
    void on_load(sep::Engine* engine, sep::CyclesRenderer* renderer) override;
    void on_update(float dt) override;
    void on_render() override;
    void on_ui_render() override;
    void on_unload() override;
    void on_key_press(int key) override;
    void on_mouse(int x, int y, int button) override;

private:
    struct Particle {
        glm::vec3 position{0.0f};
        glm::vec3 velocity{0.0f};
    };

    std::vector<Particle> particles_;
    float temperature_{1.0f};
    float cooling_rate_{0.99f};
    bool paused_{false};
};

} // namespace workbench
} // namespace sep
