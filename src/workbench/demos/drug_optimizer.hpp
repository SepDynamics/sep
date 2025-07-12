#include "workbench/demos/demo_base.hpp"
#pragma once
#include "workbench/demos/demo_base.hpp"

#include "workbench/demos/demo_base.hpp"
#include <glm/vec3.hpp>
#include <vector>

#include "demo_manager.hpp"
#include "workbench/demos/demo_base.hpp"
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
    float threshold_{1.0f};
    float decay_{0.1f};
    float input_strength_{0.5f};
    float learning_rate_{0.05f};
    float connection_prob_{0.2f};
};

} // namespace workbench
}  // namespace sep