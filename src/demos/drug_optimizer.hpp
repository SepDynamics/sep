#pragma once

#include "demo_manager.hpp"
#include "quantum/quantum_manifold_optimizer.h"
#include <glm/vec3.hpp>
#include <vector>

namespace sep {
namespace workbench {

struct MoleculePose {
    glm::vec3 position{0.0f};
    glm::vec3 orientation{0.0f};
    float binding_affinity{0.0f};
};

class DrugOptimizerDemo : public Demo {
public:
    void init() override;
    void update(float dt) override;
    void render() override;
    void cleanup() override;
    void handleKeyboard(unsigned char key) override;
    void handleMouse(int x, int y, int button) override;

private:
    float computeBindingScore(const MoleculePose& pose);

    std::vector<MoleculePose> poses_;
    sep::quantum::manifold::QuantumManifoldOptimizer optimizer_{};
};

} // namespace workbench
} // namespace sep
