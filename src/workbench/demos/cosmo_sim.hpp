#pragma once

#include <glm/vec3.hpp>
#include <vector>

#include "../../workbench_demo_adapter.hpp"
#include "demo_manager.hpp"
#include "imgui.h"
#include "quantum/data.hpp"

namespace sep {
namespace workbench {

class CosmoSim : public Demo {
public:
    void on_load(sep::Engine* engine, sep::CyclesRenderer* renderer) override;
    void on_ui_render() override;
    void on_update(float dt) override;
    void on_render() override;
    void on_unload() override;
    void on_key_press(int key) override;
    void on_mouse(int x, int y, int button = 0) override;

private:
    std::vector<sep::pattern::PatternData> bodies_;
    float box_size_{50.0f};
    float time_step_{0.01f};
    float G_{1.0f};

    void initBodies();
    void integrate(float dt);
    void updateCoherence();
};

} // namespace workbench
} // namespace sep
