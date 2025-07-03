#include "audio_visualizer.hpp"
#include "quantum/processor.h"
#include "quantum/data.hpp"
#include "audio/pipeline.h"
#include <glm/glm.hpp>

namespace sep {
namespace workbench {

void AudioVisualizerDemo::init() {
    const auto& cfg = Config::getInstance().audio_visualizer();
    audio_pipeline_ = audio::AudioPipeline(cfg.input.sample_rate, 1);
    audio_pipeline_.setPatternQueue(&pattern_queue_);
    pattern_processor_ = std::make_unique<sep::pattern::PatternProcessor>();
    coherence_manager_ = std::make_unique<sep::memory::QuantumCoherenceManager>(sep::memory::QuantumCoherenceManager::Config{});
}

void AudioVisualizerDemo::processQueue() {
    while (!pattern_queue_.empty()) {
        glm::vec3 p = pattern_queue_.front();
        pattern_queue_.pop();
        sep::pattern::PatternData pat;
        pat.position = glm::vec4(p, 1.0f);
        pattern_processor_->addPattern(pat);
    }
}

void AudioVisualizerDemo::update(float dt) {
    (void)dt;
    processQueue();
    if (auto_evolve_) {
        pattern_processor_->evolvePatterns();
    }
    if (renderer_) {
        const auto& patterns = pattern_processor_->getPatterns();
        renderer_->createSceneFromPatterns(patterns);
        sep::blender::ccl::CyclesRenderer::RenderParams params;
        params.width = 640; params.height = 480; params.samples = 16;
        renderer_->renderScene(params);
    }
}

void AudioVisualizerDemo::render() {}

void AudioVisualizerDemo::cleanup() {
    pattern_processor_.reset();
    coherence_manager_.reset();
}

void AudioVisualizerDemo::handleKeyboard(unsigned char key) {
    if (key == ' ') auto_evolve_ = !auto_evolve_;
}

void AudioVisualizerDemo::handleMouse(int, int, int) {}

} // namespace workbench
} // namespace sep
