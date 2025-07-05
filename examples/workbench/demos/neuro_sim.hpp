#pragma once

#include "demo_manager.hpp"
#include "sep_engine_wrapper.h"
#include <memory>
#include <random>
#include <vector>

namespace sep {
namespace workbench {

class NeuroSimDemo : public Demo {
public:
    void init() override;
    void update(float dt) override;
    void render() override;
    void cleanup() override;
    void handleKeyboard(unsigned char key) override;
    void handleMouse(int x, int y, int button) override;

private:
    struct Neuron {
        quantum::Pattern pattern;
        float potential{0.f};
        uint64_t node_id{0};
    };

    std::unique_ptr<memory::MemoryTierManager> memory_manager_;
    std::vector<Neuron> neurons_;
    std::mt19937 rng_{std::random_device{}()};

    float threshold_{1.f};
    float decay_{0.1f};
    float input_strength_{0.5f};
    float learning_rate_{0.05f};
    float connection_prob_{0.2f};
};

} // namespace workbench
} // namespace sep
