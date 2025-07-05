#pragma once

#include "demo_manager.hpp"
#include <glm/vec3.hpp>
#include <glm/gtc/random.hpp>
#include <vector>

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
    void initializeAgents();

    std::vector<PatternData> agents_;
    float cohesion_weight_{1.0f};
    float separation_weight_{1.0f};
    float alignment_weight_{1.0f};
    float neighbor_radius_{5.0f};
    float max_speed_{1.0f};
};

} // namespace workbench
} // namespace sep
