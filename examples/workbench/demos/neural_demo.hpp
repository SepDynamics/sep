#pragma once

#include "demo_manager.hpp"
#include <core/dag_graph.h>
#include <glm/vec3.hpp>
#include <vector>

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
    std::vector<float> potentials_;
    std::vector<float> outputs_;
    std::vector<glm::vec3> positions_;

    struct Params {
        int neuron_count{10};
        float threshold{1.0f};
        float decay{0.1f};
    } params_;
};

} // namespace workbench
} // namespace sep
