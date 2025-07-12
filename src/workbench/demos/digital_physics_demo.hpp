#pragma once

#include <algorithm>
#include <vector>

#include "core/types.h"
#include "demo_manager.hpp"
#include "workbench/demos/demo_base.hpp"
#include "imgui.h"

namespace sep {
namespace workbench {

class DigitalPhysicsDemo : public Demo {
public:
    void on_load(sep::Engine* engine, sep::CyclesRenderer* renderer) override;
    void on_ui_render() override;
    void on_update(float dt) override;
    void on_render() override;
    void on_unload() override;
    void on_key_press(int key) override;
    void on_mouse(int x, int y, int button) override;

private:
    std::size_t width_{0};
    std::size_t height_{0};
    std::vector<sep::pattern::PatternData> grid_;
    std::vector<int> birth_rules_;
    std::vector<int> survival_rules_;
    float threshold_{1.0f};
    float decay_{0.1f};
    float input_strength_{0.5f};
    float learning_rate_{0.05f};
    float connection_prob_{0.2f};

    std::size_t index(std::size_t x, std::size_t y) const { return y * width_ + x; }
};

} // namespace workbench
}  // namespace sep