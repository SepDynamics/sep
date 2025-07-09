#pragma once

#include "demo_manager.hpp"
#include "sep_engine_wrapper.h"
#include <vector>

namespace sep {
namespace workbench {

class NeuralDemo : public Demo {
public:
    void on_load() override;
    void on_update(float dt) override;
    void on_render() override;
    void on_unload() override;
    void on_key_press(unsigned char key) override;
    void on_mouse(int x, int y, int button);

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
};

} // namespace workbench
} // namespace sep
