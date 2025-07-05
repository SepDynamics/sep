#include "neural_demo.hpp"
#include <config.hpp>
#include <algorithm>

namespace sep {
namespace workbench {

void NeuralDemo::init() {
    const auto& cfg = sep::core::config::ConfigManager::getInstance().getEngineConfig().neural_demo();

    decay_ = cfg.neuron.decay;
    input_strength_ = cfg.neuron.input_strength;

    neurons_.resize(cfg.network.neuron_count);
    for (size_t i = 0; i < neurons_.size(); ++i) {
        neurons_[i].threshold = cfg.neuron.threshold;
        neurons_[i].id = graph_.addNode(glm::vec3(0.0f), 0.0f, {});
    }

    std::uniform_real_distribution<float> dist(0.0f, 1.0f);
    for (size_t i = 0; i < neurons_.size(); ++i) {
        for (size_t j = 0; j < neurons_.size(); ++j) {
            if (i == j) continue;
            if (dist(rng_) < cfg.network.connection_prob) {
                auto parents = graph_.getParents(neurons_[j].id);
                parents.push_back(neurons_[i].id);
                graph_.updateNodeParents(neurons_[j].id, parents);
            }
        }
    }
}

void NeuralDemo::update(float dt) {
    for (auto& n : neurons_) {
        n.potential += input_strength_ * dt;
        n.potential -= decay_ * n.potential * dt;

        if (n.potential >= n.threshold) {
            n.potential = 0.f;
            // propagate to connected neurons
            for (auto& target : neurons_) {
                auto parents = graph_.getParents(target.id);
                if (std::find(parents.begin(), parents.end(), n.id) != parents.end()) {
                    target.potential += 0.5f;
                }
            }
        }
    }
}

void NeuralDemo::render() {
    if (!renderer_) return;

    std::vector<glm::vec3> points;
    for (const auto& n : neurons_) {
        points.emplace_back(n.potential);
    }
    renderer_->renderPatternState(points);
}

void NeuralDemo::cleanup() {
    neurons_.clear();
}

void NeuralDemo::handleKeyboard(unsigned char) {}
void NeuralDemo::handleMouse(int, int, int) {}

} // namespace workbench
} // namespace sep
