#include "workbench/demos/demo_base.hpp"
#pragma once
#include "workbench/demos/demo_base.hpp"

#include "workbench/demos/demo_base.hpp"
#include <glm/vec3.hpp>
#include <vector>

#include "core/types.h"
#include "demo_manager.hpp"
#include "imgui.h"

namespace sep {
namespace workbench {

class FlockingDemo : public Demo {
public:
    void on_load(sep::Engine* engine, sep::CyclesRenderer* renderer) override;
    void on_ui_render() override;
    void on_update(float dt) override;
    void on_render() override;
    void on_unload() override;
    void on_key_press(int key) override;
    void on_mouse(int x, int y, int button) override;

private:
    std::vector<pattern::PatternData> agents_;
    float max_speed_{2.0f};
    float neighbor_radius_{5.0f};
};

} // namespace workbench
} // namespace sep
