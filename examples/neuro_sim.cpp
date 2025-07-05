#include <iostream>
#include <random>
#include <vector>

#include "memory/memory_tier_manager.hpp"
#include "quantum/evolution.h"

using namespace sep;

struct Neuron {
    quantum::Pattern pattern;
    uint64_t node_id{0};
};

int main(int argc, char** argv) {
    std::size_t neuron_count = 10;
    std::size_t steps = 50;
    float threshold = 1.f;
    float decay = 0.1f;
    float input_strength = 0.5f;
    float connection_prob = 0.2f;

    if (argc > 1) neuron_count = std::stoul(argv[1]);
    if (argc > 2) steps = std::stoul(argv[2]);

    std::mt19937 rng{std::random_device{}()};
    std::uniform_real_distribution<float> dist(0.f, 1.f);

    memory::MemoryTierManager manager;
    auto& dag = manager.getDagGraph();

    std::vector<Neuron> neurons(neuron_count);
    for (std::size_t i = 0; i < neuron_count; ++i) {
        auto& n = neurons[i];
        n.pattern.id = std::to_string(i);
        n.pattern.quantum_state.coherence = 0.f;
        n.pattern.quantum_state.stability = 0.5f;
        n.pattern.memory_tier = memory::MemoryTierEnum::STM;
        n.pattern.position = glm::vec4(static_cast<float>(i), 0.f, 0.f, 1.f);
        manager.registerPattern(i, n.pattern);
        n.node_id = dag.addNode(glm::vec3(n.pattern.position), n.pattern.quantum_state.coherence, {});
    }

    for (std::size_t j = 0; j < neurons.size(); ++j) {
        for (std::size_t i = 0; i < neurons.size(); ++i) {
            if (i == j) continue;
            if (dist(rng) < connection_prob) {
                auto parents = dag.getParents(neurons[j].node_id);
                parents.push_back(neurons[i].node_id);
                dag.updateNodeParents(neurons[j].node_id, parents);
            }
        }
    }

    for (std::size_t step = 0; step < steps; ++step) {
        std::size_t fired = 0;
        float sum_coherence = 0.f;
        for (auto& n : neurons) {
            quantum::evolution::applySpike(n.pattern, input_strength, decay, threshold);
            if (n.pattern.quantum_state.coherence >= 1.0f) {
                ++fired;
                for (auto& tgt : neurons) {
                    auto parents = dag.getParents(tgt.node_id);
                    if (std::find(parents.begin(), parents.end(), n.node_id) != parents.end()) {
                        tgt.pattern.quantum_state.coherence = glm::clamp(tgt.pattern.quantum_state.coherence + 0.5f, 0.f, 1.f);
                        quantum::evolution::hebbianUpdate(n.pattern, tgt.pattern, 0.05f);
                    }
                }
                n.pattern.quantum_state.coherence = 0.f;
            }

            auto tier = manager.determineTier(n.pattern.quantum_state.coherence,
                                             n.pattern.quantum_state.stability,
                                             n.pattern.quantum_state.generation);
            n.pattern.memory_tier = static_cast<memory::MemoryTierEnum>(tier->getType());
            dag.updateCoherence(n.node_id, n.pattern.quantum_state.coherence);
            manager.registerPattern(std::stoull(n.pattern.id), n.pattern);
            sum_coherence += n.pattern.quantum_state.coherence;
        }
        float avg = sum_coherence / neurons.size();
        std::cout << "Step " << step << ": fired=" << fired << " avg_coherence=" << avg << "\n";
    }

    return 0;
}

