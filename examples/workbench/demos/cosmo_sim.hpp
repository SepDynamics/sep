#pragma once

#include "demo_manager.hpp"
#include <vector>
#include <quantum/data.hpp>
#include <glm/vec3.hpp>

namespace sep {
namespace workbench {

class CosmoSim : public Demo {
public:
    void init() override;
    void update(float dt) override;
    void render() override;
    void cleanup() override;
    void handleKeyboard(unsigned char key) override;
    void handleMouse(int x, int y, int button) override;

private:
    std::vector<pattern::PatternData> bodies_;
    float box_size_{100.0f};
    float time_step_{0.01f};
    std::size_t particle_count_{100};

    void initBodies();
    void integrate(float dt);
};

} // namespace workbench
} // namespace sep
