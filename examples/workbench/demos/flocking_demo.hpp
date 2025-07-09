#pragma once

#include "demo_manager.hpp"
#include <vector>
#include <glm/vec3.hpp>
#include <quantum/data.hpp>

namespace sep {
namespace workbench {

class FlockingDemo : public Demo {
public:
    void on_load() override;
    void on_update(float dt) override;
    void on_render() override;
    void on_unload() override;
    void on_key_press(unsigned char key) override;
    void on_mouse(int x, int y, int button);

private:
    std::vector<pattern::PatternData> agents_;
    float max_speed_{2.0f};
    float neighbor_radius_{5.0f};
};

} // namespace workbench
} // namespace sep
