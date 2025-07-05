#pragma once

#include "demo_manager.hpp"
#include <glm/vec3.hpp>
#include <vector>

namespace sep {
namespace workbench {

class DrugDiscoveryDemo : public Demo {
public:
    void init() override;
    void update(float dt) override;
    void render() override;
    void cleanup() override;
    void handleKeyboard(unsigned char key) override;
    void handleMouse(int x, int y, int button) override;

private:
    struct Pose {
        glm::vec3 position{0.f};
        glm::vec3 orientation{0.f};
        float binding_affinity{0.f};
    };

    std::vector<Pose> poses_;

    int iterations_{100};
    float mutation_rate_{0.05f};

    void runOptimizationStep();
};

} // namespace workbench
} // namespace sep
