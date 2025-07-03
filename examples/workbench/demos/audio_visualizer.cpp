#include "audio_visualizer.hpp"

namespace sep {
namespace workbench {

void AudioVisualizerDemo::init() {
    const auto& config = Config::getInstance();
    const auto& audio_cfg = config.audio_visualizer();

    audio_pipeline_ = std::make_unique<audio::AudioPipeline>(audio_cfg.input.sample_rate, 2);
    audio_pipeline_->setPatternQueue(&pattern_queue_);
    pattern_processor_ = std::make_unique<pattern::PatternProcessor>();
    evolution_rate_ = audio_cfg.pattern_mapping.evolution_sensitivity;
}

void AudioVisualizerDemo::update(float dt) {
    (void)dt;
    auto patterns = audio_pipeline_->getPatterns();
    for (const auto& p : patterns) {
        pattern::PatternData pd;
        pd.position = glm::vec4(p, 1.0f);
        pattern_processor_->addPattern(pd);
    }
}

void AudioVisualizerDemo::render() {
    // Rendering handled by renderer from Demo base
}

void AudioVisualizerDemo::cleanup() {
    audio_pipeline_.reset();
    pattern_processor_.reset();
    std::queue<glm::vec3> empty;
    std::swap(pattern_queue_, empty);
}

void AudioVisualizerDemo::handleKeyboard(unsigned char key) {
    if (key == '+') {
        evolution_rate_ *= 1.1f;
    } else if (key == '-') {
        evolution_rate_ *= 0.9f;
    }
}

void AudioVisualizerDemo::handleMouse(int, int, int) {}

} // namespace workbench
} // namespace sep
