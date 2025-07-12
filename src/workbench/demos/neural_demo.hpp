#pragma once

#include <vector>

#include "core/dag_graph.h"
#include "demo_manager.hpp"
#include "workbench/demos/demo_base.hpp"
#include "imgui.h"
#include "sep_engine_wrapper.h"

namespace sep {
namespace workbench {

class NeuralDemo : public Demo {
public:
    void on_load(sep::Engine* engine, sep::CyclesRenderer* renderer) override;
    void on_ui_render() override;
    void on_update(float dt) override;
    void on_render() override;
    void on_unload() override;
    void on_key_press(int key) override;
    void on_mouse(int x, int y, int button) override;

private:
    struct Neuron {
        uint64_t id;
        float potential{0.f};
    };

    std::vector<Neuron> neurons_;
    dag::DagGraph graph_;
    float threshold_{1.0f};
    float decay_{0.1f};
    float weight_{0.5f};
    float input_strength_{0.5f};
    float learning_rate_{0.05f};
    float connection_prob_{0.2f};
};

} // namespace workbench
}  // namespace sep