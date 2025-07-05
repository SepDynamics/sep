#pragma once

#include "demo_manager.hpp"
#include <vector>
#include <glm/vec3.hpp>

namespace sep {
namespace workbench {

struct PatternData {
    glm::vec3 position;
    glm::vec3 velocity;
};

class FlockingDemo : public Demo {
public:
    void init() override;
    void update(float dt) override;
    void render() override;
    void cleanup() override;
    void handleKeyboard(unsigned char key) override;
    void handleMouse(int x, int y, int button) override;

private:
    std::vector<PatternData> agents_;

    float cohesion_weight_{1.0f};
    float separation_weight_{1.5f};
    float alignment_weight_{1.0f};
};

} // namespace workbench
} // namespace sep
