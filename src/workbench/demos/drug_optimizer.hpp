#pragma once

#include <glm/vec3.hpp>
#include <vector>

#include "../../workbench_demo_adapter.hpp"
#include "demo_manager.hpp"
#include "imgui.h"
#include "quantum/quantum_manifold_optimizer.h"

namespace sep {
namespace workbench {

struct MoleculePose {
    glm::vec3 position{0.0f};
    glm::vec3 orientation{0.0f};
    float binding_affinity{0.0f};
};

class DrugOptimizerDemo : public Demo {
public:
    void on_load(sep::Engine* engine, sep::CyclesRenderer* renderer) override;
    void on_ui_render() override;
    void on_update(float dt) override;
    void on_render() override;
    void on_unload() override;
    void on_key_press(int key) override;
    void on_mouse(int x, int y, int button) override;

private:
    float computeBindingScore(const MoleculePose& pose);

    std::vector<MoleculePose> poses_;
    sep::quantum::manifold::QuantumManifoldOptimizer optimizer_{};
};

} // namespace workbench
} // namespace sep
