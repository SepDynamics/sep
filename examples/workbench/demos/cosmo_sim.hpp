#pragma once

#include "demo_manager.hpp"
#include <quantum/data.hpp>
#include <vector>
#include <glm/vec3.hpp>

namespace sep {
namespace workbench {

class CosmoSim : public Demo {
public:
    void on_load() override;
    void on_update(float dt) override;
    void on_render() override;
    void on_unload() override;
    void on_key_press(unsigned char key) override;
    void on_mouse(int x, int y, int button);

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
