#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <glm/glm.hpp>

#include "core/dag_graph.h"
#include "quantum/types.h"
#include "quantum/evolution.h"

using namespace sep;

struct Neuron {
    quantum::Pattern pattern;
    float potential{0.f};
    uint64_t node_id{0};
};

int main() {
    constexpr std::size_t neuron_count = 10;
    constexpr float threshold = 0.6f;
    constexpr float decay = 0.1f;
    constexpr float input_strength = 0.5f;
    constexpr float learning_rate = 0.05f;
    constexpr std::size_t steps = 50;

    dag::DagGraph graph;
    std::vector<Neuron> neurons(neuron_count);

    for (std::size_t i = 0; i < neuron_count; ++i) {
        auto &n = neurons[i];
        n.pattern.id = std::to_string(i);
        n.pattern.position = glm::vec4(static_cast<float>(i), 0.f, 0.f, 1.f);
        n.pattern.quantum_state.coherence = 0.f;
        n.pattern.quantum_state.stability = 0.5f;
        n.node_id = graph.addNode(glm::vec3(n.pattern.position), n.pattern.quantum_state.coherence, {});
    }

    std::mt19937 rng{std::random_device{}()};
    std::uniform_real_distribution<float> dist(0.f, 1.f);

    for (std::size_t j = 0; j < neuron_count; ++j) {
        for (std::size_t i = 0; i < neuron_count; ++i) {
            if (i == j) continue;
            if (dist(rng) < 0.3f) {
                auto parents = graph.getParents(neurons[j].node_id);
                parents.push_back(neurons[i].node_id);
                graph.updateNodeParents(neurons[j].node_id, parents);
            }
        }
    }

    for (std::size_t step = 0; step < steps; ++step) {
        for (auto &n : neurons) {
            quantum::evolution::applySpike(n.pattern, input_strength, decay, threshold);
            if (n.pattern.quantum_state.coherence >= 1.0f) {
                for (auto &tgt : neurons) {
                    auto parents = graph.getParents(tgt.node_id);
                    if (std::find(parents.begin(), parents.end(), n.node_id) != parents.end()) {
                        tgt.pattern.quantum_state.coherence = glm::clamp(tgt.pattern.quantum_state.coherence + 0.5f, 0.f, 1.f);
                        quantum::evolution::hebbianUpdate(n.pattern, tgt.pattern, learning_rate);
                    }
                }
                n.pattern.quantum_state.coherence = 0.f;
            }
            graph.updateCoherence(n.node_id, n.pattern.quantum_state.coherence);
        }

        float total_coherence = 0.f;
        std::size_t active = 0;
        for (const auto &n : neurons) {
            total_coherence += n.pattern.quantum_state.coherence;
            if (n.pattern.quantum_state.coherence > threshold) ++active;
        }
        float avg_coherence = total_coherence / static_cast<float>(neurons.size());
        std::cout << "Step " << step << ": avg coherence=" << avg_coherence
                  << ", active=" << active << "/" << neuron_count << std::endl;
    }

    return 0;
}


