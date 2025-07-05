#pragma once

#include "demo_manager.hpp"
#include "core/dag_graph.h"
#include <vector>
#include <glm/vec3.hpp>

namespace sep {
namespace workbench {

class NeuralDemo : public Demo {
public:
    void init() override;
    void update(float dt) override;
    void render() override;
    void cleanup() override;
    void handleKeyboard(unsigned char key) override;
    void handleMouse(int x, int y, int button) override;

private:
    dag::DagGraph graph_;

    struct Neuron {
        uint64_t id;
        float potential{0.f};
    };
    std::vector<Neuron> neurons_;

    float threshold_{1.0f};
    float decay_{0.1f};
    float input_strength_{0.5f};

    std::vector<glm::vec3> visualize_;
};

} // namespace workbench
} // namespace sep
