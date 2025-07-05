#include "neuro_sim.hpp"
#include <config.hpp>
#include <algorithm>
#include <glm/glm.hpp>
#include "quantum/evolution.h"

namespace sep {
namespace workbench {

void NeuroSimDemo::init() {
#ifdef SEP_WORKBENCH_DEMO
    std::size_t neuron_count = 10;
    threshold_ = 1.f;
    decay_ = 0.1f;
    input_strength_ = 0.5f;
    connection_prob_ = 0.2f;
#else
    const auto& cfg = sep::core::config::ConfigManager::getInstance().getEngineConfig().neural_demo();
    std::size_t neuron_count = cfg.network.neuron_count;
    threshold_ = cfg.neuron.threshold;
    decay_ = cfg.neuron.decay;
    input_strength_ = cfg.neuron.input_strength;
    connection_prob_ = cfg.network.connection_prob;
#endif
    memory_manager_ = std::make_unique<memory::MemoryTierManager>();
    auto& dag = memory_manager_->getDagGraph();

    neurons_.resize(neuron_count);
    for (std::size_t i = 0; i < neuron_count; ++i) {
        auto& n = neurons_[i];
        n.pattern.id = std::to_string(i);
        n.pattern.coherence = 0.f;
        n.pattern.stability = 0.5f;
        n.pattern.memory_tier = memory::MemoryTierEnum::STM;
        n.pattern.position = glm::vec4(static_cast<float>(i), 0.f, 0.f, 1.f);
        memory_manager_->registerPattern(i, n.pattern);
        n.node_id = dag.addNode(glm::vec3(n.pattern.position), n.pattern.coherence, {});
    }

    std::uniform_real_distribution<float> dist(0.f, 1.f);
    for (std::size_t j = 0; j < neurons_.size(); ++j) {
        for (std::size_t i = 0; i < neurons_.size(); ++i) {
            if (i == j) continue;
            if (dist(rng_) < connection_prob_) {
                auto parents = dag.getParents(neurons_[j].node_id);
                parents.push_back(neurons_[i].node_id);
                dag.updateNodeParents(neurons_[j].node_id, parents);
            }
        }
    }
}

void NeuroSimDemo::update(float dt) {
    auto& dag = memory_manager_->getDagGraph();
    for (auto& n : neurons_) {
        quantum::evolution::applySpike(n.pattern, input_strength_ * dt, decay_ * dt, threshold_);
        if (n.pattern.quantum_state.coherence >= 1.0f) {
            for (auto& tgt : neurons_) {
                auto parents = dag.getParents(tgt.node_id);
                if (std::find(parents.begin(), parents.end(), n.node_id) != parents.end()) {
                    tgt.pattern.quantum_state.coherence = glm::clamp(tgt.pattern.quantum_state.coherence + 0.5f, 0.f, 1.f);
                    quantum::evolution::hebbianUpdate(n.pattern, tgt.pattern, learning_rate_);
                }
            }
            n.pattern.quantum_state.coherence = 0.f;
        }

        auto tier = memory_manager_->determineTier(n.pattern.quantum_state.coherence,
                                                   n.pattern.quantum_state.stability,
                                                   n.pattern.quantum_state.generation);
        n.pattern.memory_tier = static_cast<memory::MemoryTierEnum>(tier->getType());
        dag.updateCoherence(n.node_id, n.pattern.quantum_state.coherence);
        memory_manager_->registerPattern(std::stoull(n.pattern.id), n.pattern);
    }
}

void NeuroSimDemo::render() {
    if (!renderer_) return;
    std::vector<glm::vec3> pts;
    pts.reserve(neurons_.size());
    for (const auto& n : neurons_) {
        pts.emplace_back(n.pattern.position.x, n.pattern.position.y, n.pattern.position.z);
    }
    renderer_->renderPatternState(pts);
}

void NeuroSimDemo::cleanup() {
    neurons_.clear();
    memory_manager_.reset();
}

void NeuroSimDemo::handleKeyboard(unsigned char) {}
void NeuroSimDemo::handleMouse(int, int, int) {}

} // namespace workbench
} // namespace sep
