#pragma once

#include <glm/vec3.hpp>
#include <vector>

#include "core/types.h"
#include "demo_manager.hpp"
#include "workbench/demos/demo_base.hpp"
#include "imgui.h"

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
    void on_mouse(int x, int y, int button = 0);

private:
    std::vector<sep::pattern::PatternData> bodies_;
    float box_size_{50.0f};
    float time_step_{0.01f};
    float G_{1.0f};
    float threshold_{1.0f};
    float decay_{0.1f};
    float input_strength_{0.5f};
    float learning_rate_{0.05f};
    float connection_prob_{0.2f};

    sep::CyclesRenderer* renderer_{nullptr};

    void initBodies();
    void integrate(float dt);
    void updateCoherence();
};

} // namespace workbench
}  // namespace sep