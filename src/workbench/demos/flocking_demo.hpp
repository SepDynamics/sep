#pragma once

#include "demo_manager.hpp"
#include <vector>
#include <glm/vec3.hpp>
#include <quantum/data.hpp>

namespace sep {
namespace workbench {

class FlockingDemo : public Demo {
public:
    void init() override;
    void update(float dt) override;
    void render() override;
    void cleanup() override;
    void handleKeyboard(unsigned char key) override;
    void handleMouse(int x, int y, int button) override;

private:
    std::vector<pattern::PatternData> agents_;
    float max_speed_{2.0f};
    float neighbor_radius_{5.0f};
};

} // namespace workbench
} // namespace sep
