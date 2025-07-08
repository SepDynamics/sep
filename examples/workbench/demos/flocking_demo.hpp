#pragma once

#include "../demo_manager.hpp"
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
    std::vector<sep::pattern::PatternData> agents_;
    float neighbor_radius_{5.0f};
    float separation_distance_{1.0f};
    float max_speed_{2.0f};
};

} // namespace workbench
} // namespace sep
