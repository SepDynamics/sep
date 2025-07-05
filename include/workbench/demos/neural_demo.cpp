#include "neural_demo.hpp"
#include "config.hpp"
#include <algorithm>

namespace sep {
namespace workbench {

void NeuralDemo::init() {
#ifdef SEP_WORKBENCH_DEMO
    // Default parameters for demo build
    threshold_ = 1.0f;
    decay_ = 0.1f;
    input_strength_ = 0.5f;
#else
    const auto& cfg = Config::getInstance().neural();
    threshold_ = cfg.threshold;
    decay_ = cfg.decay;
    input_strength_ = cfg.input_strength;
#endif

    // Create a small feed-forward chain of three neurons
    uint64_t n1 = graph_.addNode(glm::vec3(0.0f), 0.0f, {});
    uint64_t n2 = graph_.addNode(glm::vec3(0.0f), 0.0f, {n1});
    uint64_t n3 = graph_.addNode(glm::vec3(0.0f), 0.0f, {n2});

    neurons_.push_back({n1, 0.0f});
    neurons_.push_back({n2, 0.0f});
    neurons_.push_back({n3, 0.0f});

    visualize_.resize(neurons_.size(), glm::vec3(0.0f));
}

void NeuralDemo::update(float dt) {
    if (neurons_.empty()) return;

    // External input to first neuron
    neurons_[0].potential += input_strength_ * dt;

    // Integrate input from parents
    for (size_t i = 1; i < neurons_.size(); ++i) {
        float input = 0.f;
        auto parents = graph_.getParents(neurons_[i].id);
        for (auto p : parents) {
            auto it = std::find_if(neurons_.begin(), neurons_.end(), [p](const Neuron& n){ return n.id == p; });
            if (it != neurons_.end()) {
                input += it->potential;
            }
        }
        neurons_[i].potential += input * dt;
    }

    // Apply decay and fire
    for (auto& n : neurons_) {
        n.potential -= decay_ * dt;
        if (n.potential < 0.f) n.potential = 0.f;

        if (n.potential >= threshold_) {
            n.potential = 0.f;
            // Propagate spike to children
            for (auto& target : neurons_) {
                auto parents = graph_.getParents(target.id);
                if (std::find(parents.begin(), parents.end(), n.id) != parents.end()) {
                    target.potential += 1.0f;
                }
            }
        }
    }

    // Update visualization patterns
    for (size_t i = 0; i < neurons_.size(); ++i) {
        visualize_[i] = glm::vec3(static_cast<float>(i), 0.0f, neurons_[i].potential);
    }
}

void NeuralDemo::render() {
#ifdef SEP_WORKBENCH_DEMO
    if (renderer_) {
        renderer_->renderPatternState(visualize_);
    }
#else
    if (renderer_) {
        renderer_->renderPatternState(visualize_);
    }
#endif
}

void NeuralDemo::cleanup() {
    neurons_.clear();
    visualize_.clear();
}

void NeuralDemo::handleKeyboard(unsigned char) {}
void NeuralDemo::handleMouse(int, int, int) {}

} // namespace workbench
} // namespace sep
