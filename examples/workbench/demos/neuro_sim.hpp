#pragma once

#include "demo_manager.hpp"
#include <memory/memory_tier_manager.hpp>
#include <quantum/evolution.h>
#include <quantum/data.hpp>
#include <core/dag_graph.h>
#include <glm/vec3.hpp>
#include <vector>
#include <random>

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
        pattern::PatternData data;
        memory::MemoryBlock* block{nullptr};
        uint64_t node_id{0};
        float threshold{1.f};
    };

    memory::MemoryTierManager* mem_{nullptr};
    dag::DagGraph* graph_{nullptr};
    std::vector<Neuron> neurons_;
    std::mt19937 rng_{std::random_device{}()};
};

} // namespace workbench
} // namespace sep
