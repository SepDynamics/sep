#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <glm/vec4.hpp>
#include <glm/gtc/clamp.hpp>

#include "core/dag_graph.h"
#include "quantum/evolution.h"

struct Neuron {
    sep::quantum::Pattern pattern;
    uint64_t node_id{0};
};

int main() {
    constexpr std::size_t kNeuronCount = 10;
    constexpr float threshold = 1.0f;
    constexpr float decay = 0.1f;
    constexpr float input_strength = 0.5f;
    constexpr float learning_rate = 0.05f;
    constexpr float connection_prob = 0.3f;
    const int steps = 50;

    std::mt19937 rng{std::random_device{}()};
    std::uniform_real_distribution<float> dist(0.f, 1.f);

    sep::dag::DagGraph graph;
    std::vector<Neuron> neurons(kNeuronCount);

    for (std::size_t i = 0; i < kNeuronCount; ++i) {
        auto &n = neurons[i];
        n.pattern.id = std::to_string(i);
        n.pattern.position = glm::vec4(static_cast<float>(i), 0.f, 0.f, 1.f);
        n.pattern.quantum_state.coherence = 0.f;
        n.pattern.quantum_state.stability = 0.5f;
        n.node_id = graph.addNode(glm::vec3(n.pattern.position), 0.f, {});
    }

    for (std::size_t j = 0; j < neurons.size(); ++j) {
        std::vector<uint64_t> parents;
        for (std::size_t i = 0; i < neurons.size(); ++i) {
            if (i == j) continue;
            if (dist(rng) < connection_prob) {
                parents.push_back(neurons[i].node_id);
            }
        }
        graph.updateNodeParents(neurons[j].node_id, parents);
    }

    for (int step = 0; step < steps; ++step) {
        float total_coh = 0.f;
        for (auto &n : neurons) {
            sep::quantum::evolution::applySpike(n.pattern, input_strength, decay, threshold);
            if (n.pattern.quantum_state.coherence >= 1.f) {
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
            total_coh += n.pattern.quantum_state.coherence;
        }
        float avg = total_coh / static_cast<float>(neurons.size());
        std::cout << "Step " << step << ": avg coherence = " << avg << std::endl;
    }
    return 0;
}
