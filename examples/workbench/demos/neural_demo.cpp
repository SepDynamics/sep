#include "neural_demo.hpp"
#include <config.hpp>
#include <glm/gtc/constants.hpp>

namespace sep {
namespace workbench {

void NeuralDemo::init() {
#ifdef SEP_WORKBENCH_DEMO
    // Use default parameters
    params_ = {10, 1.0f, 0.1f};
#else
    const auto& cfg = getConfigManager().getEngineConfig().neural();
    params_.neuron_count = cfg.neuron_count;
    params_.threshold = cfg.threshold;
    params_.decay = cfg.decay;
#endif

    potentials_.assign(params_.neuron_count, 0.0f);
    outputs_.assign(params_.neuron_count, 0.0f);
    positions_.clear();

    // Create simple ring connectivity
    for (int i = 0; i < params_.neuron_count; ++i) {
        int parent = (i == 0) ? params_.neuron_count - 1 : i - 1;
        graph_.addNodeWithId(i, glm::vec3(0.0f), 0.0f, {static_cast<uint64_t>(parent)});
        float angle = 2.0f * glm::pi<float>() * (static_cast<float>(i) / params_.neuron_count);
        positions_.push_back(glm::vec3(std::cos(angle) * 5.0f, 0.0f, std::sin(angle) * 5.0f));
    }
}

void NeuralDemo::update(float dt) {
    std::vector<float> inputs(params_.neuron_count, 0.0f);

    // Integrate inputs from parent spikes
    for (int i = 0; i < params_.neuron_count; ++i) {
        auto parents = graph_.getParents(i);
        float sum = 0.0f;
        for (auto p : parents) {
            if (p < outputs_.size()) sum += outputs_[p];
        }
        inputs[i] = sum;
    }

    std::vector<float> new_outputs(params_.neuron_count, 0.0f);
    for (int i = 0; i < params_.neuron_count; ++i) {
        potentials_[i] += inputs[i] * dt;
        potentials_[i] *= (1.0f - params_.decay * dt);
        if (potentials_[i] >= params_.threshold) {
            potentials_[i] = 0.0f;
            new_outputs[i] = 1.0f;
        }
    }
    outputs_ = std::move(new_outputs);
}

void NeuralDemo::render() {
#ifdef SEP_WORKBENCH_DEMO
    if (renderer_) {
        renderer_->renderPatternState(positions_);
    }
#else
    if (!renderer_) return;
    renderer_->renderPatternState(positions_);
#endif
}

void NeuralDemo::cleanup() {
    potentials_.clear();
    outputs_.clear();
    positions_.clear();
}

void NeuralDemo::handleKeyboard(unsigned char) {}
void NeuralDemo::handleMouse(int, int, int) {}

} // namespace workbench
} // namespace sep
