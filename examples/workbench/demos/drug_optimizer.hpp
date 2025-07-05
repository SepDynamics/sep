#pragma once

#include "demo_manager.hpp"
#include "quantum/quantum_manifold_optimizer.h"
#include <vector>

namespace sep {
namespace workbench {

struct MolecularPose {
    sep::quantum::Pattern pattern;
    float score{0.0f};
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
    std::vector<MolecularPose> poses_{};
    sep::quantum::manifold::QuantumManifoldOptimizer optimizer_{};
};

} // namespace workbench
} // namespace sep
