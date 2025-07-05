#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <glm/glm.hpp>

#include "core/dag_graph.h"
#include "quantum/types.h"
#include "quantum/evolution.h"

// Simple neuron network simulation using SEP primitives
int main() {
    const std::size_t neuron_count = 10;
    const float threshold = 1.0f;
    const float decay = 0.1f;
    const float input_strength = 0.5f;
    const float learning_rate = 0.05f;
    const float connection_prob = 0.2f;
    const int steps = 20;

    struct Neuron {
        sep::quantum::Pattern pattern;
        uint64_t node_id{0};
    };

    sep::dag::DagGraph graph;
    std::vector<Neuron> neurons(neuron_count);

    for (std::size_t i = 0; i < neuron_count; ++i) {
        auto &n = neurons[i];
        n.pattern.id = std::to_string(i);
        n.pattern.quantum_state.coherence = 0.0f;
        n.pattern.quantum_state.stability = 0.5f;
        n.pattern.position = glm::vec4(static_cast<float>(i), 0.f, 0.f, 1.f);
        n.node_id = graph.addNode(glm::vec3(n.pattern.position), 0.0f, {});
    }

    std::mt19937 rng(std::random_device{}());
    std::uniform_real_distribution<float> dist(0.f, 1.f);

    for (std::size_t j = 0; j < neuron_count; ++j) {
        for (std::size_t i = 0; i < neuron_count; ++i) {
            if (i == j) continue;
            if (dist(rng) < connection_prob) {
                auto parents = graph.getParents(neurons[j].node_id);
                parents.push_back(neurons[i].node_id);
                graph.updateNodeParents(neurons[j].node_id, parents);
            }
        }
    }

    for (int step = 0; step < steps; ++step) {
        float avg = 0.f;
        for (auto &n : neurons) {
            sep::quantum::evolution::applySpike(n.pattern, input_strength, decay, threshold);
            if (n.pattern.quantum_state.coherence >= 1.0f) {
                for (auto &tgt : neurons) {
                    auto parents = graph.getParents(tgt.node_id);
                    if (std::find(parents.begin(), parents.end(), n.node_id) != parents.end()) {
                        tgt.pattern.quantum_state.coherence = glm::clamp(tgt.pattern.quantum_state.coherence + 0.5f, 0.f, 1.f);
                        sep::quantum::evolution::hebbianUpdate(n.pattern, tgt.pattern, learning_rate);
                    }
                }
                n.pattern.quantum_state.coherence = 0.f;
            }
            graph.updateCoherence(n.node_id, n.pattern.quantum_state.coherence);
            avg += n.pattern.quantum_state.coherence;
        }
        avg /= static_cast<float>(neuron_count);
        std::cout << "Step " << step << " average coherence: " << avg << std::endl;
    }

    float final_avg = 0.f;
    int coherent = 0;
    for (auto &n : neurons) {
        final_avg += n.pattern.quantum_state.coherence;
        if (n.pattern.quantum_state.coherence > 0.5f)
            ++coherent;
    }
    final_avg /= static_cast<float>(neuron_count);

    std::cout << "Final average coherence: " << final_avg << std::endl;
    std::cout << "Neurons with coherence > 0.5: " << coherent << " / " << neuron_count << std::endl;
    return 0;
}

