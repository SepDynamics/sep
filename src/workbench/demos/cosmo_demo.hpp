#pragma once

#include <glm/vec3.hpp>
#include <vector>

#include "demo_manager.hpp"
#include "workbench/demos/demo_base.hpp"
#include "imgui.h"

namespace sep {
namespace workbench {

class CosmoDemo : public Demo {
public:
    void on_load(sep::core::Engine* engine, sep::blender::CyclesRenderer* renderer) override;
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
    sep::blender::CyclesRenderer* renderer_{nullptr};

    void initParticles();
    void integrate(float dt);
};

} // namespace workbench
} // namespace sep
