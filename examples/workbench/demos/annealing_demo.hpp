#pragma once

#include "demo_manager.hpp"
#include <vector>
#include <glm/vec3.hpp>

namespace sep {
namespace workbench {

struct Particle {
    glm::vec3 position;
    glm::vec3 velocity;
};

class AnnealingDemo : public Demo {
public:
    void init() override;
    void update(float dt) override;
    void render() override;
    void cleanup() override;
    void handleKeyboard(unsigned char key) override;
    void handleMouse(int x, int y, int button) override;

private:
    void applyForces(float dt);
    void coolSystem();

    std::vector<Particle> particles_;
    float temperature_{1.0f};
    float cooling_rate_{0.99f};
};

} // namespace workbench
} // namespace sep
