#pragma once

#include "../demo_manager.hpp"
#include <vector>
#include <random>
#include <core/dag_graph.h>
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
    struct Neuron {
        uint64_t id{0};
        float potential{0.f};
        float threshold{1.f};
    };

    dag::DagGraph graph_;
    std::vector<Neuron> neurons_;
    float decay_{0.1f};
    float input_strength_{0.5f};
    std::mt19937 rng_{std::random_device{}()};
};

} // namespace workbench
} // namespace sep
