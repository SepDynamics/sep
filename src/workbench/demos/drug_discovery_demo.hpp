#pragma once

#include <glm/vec3.hpp>
#include <vector>

#include "demo_base.hpp"
#include "demo_manager.hpp"
#include "imgui.h"

namespace sep {
namespace workbench {

struct Pose {
    glm::vec3 position{0.0f};
    glm::vec3 orientation{0.0f};
    float binding_affinity{0.0f};
};

class DrugDiscoveryDemo : public Demo {
public:
    void on_load(sep::Engine* engine, sep::CyclesRenderer* renderer) override;
    void on_ui_render() override;
    void on_update(float dt) override;
    void on_render() override;
    void on_unload() override;
    void on_key_press(int key) override;
    void on_mouse(int x, int y, int button);

private:
    void optimizePoses();

    std::vector<Pose> poses_;
    int iterations_{10};
    float mutation_rate_{0.05f};
    float threshold_{1.0f};
    float decay_{0.1f};
    float input_strength_{0.5f};
    float learning_rate_{0.05f};
    float connection_prob_{0.2f};
    sep::CyclesRenderer* renderer_{nullptr};
};

} // namespace workbench
}  // namespace sep