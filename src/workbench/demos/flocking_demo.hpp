#pragma once

#include <glm/vec3.hpp>
#include <vector>

#include "demo_base.hpp"
#include "demo_manager.hpp"
#include "engine/types.h"
#include "imgui.h"

// Forward declarations
namespace sep {
namespace core {
class Engine;
}
}

namespace sep {
namespace workbench {

class FlockingDemo : public Demo {
public:
    void on_load(sep::core::Engine* engine, sep::SimpleRenderer* renderer) override;
    void on_ui_render() override;
    void on_update(float dt) override;
    void on_render() override;
    void on_unload() override;
    void on_key_press(int key) override;
    void on_mouse(int x, int y, int button);

private:
    std::vector<sep::compat::PatternData> agents_;
    float max_speed_{2.0f};
    float neighbor_radius_{5.0f};
    sep::SimpleRenderer* renderer_{nullptr};
};

} // namespace workbench
} // namespace sep
