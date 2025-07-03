#include "audio_visualizer.hpp"
#include "config.hpp"
#include "audio/capture.h"
#include "audio/pipeline.h"
#include <glm/vec3.hpp>

namespace sep {
namespace workbench {

using namespace sep::audio;

void AudioVisualizerDemo::init() {
    const auto& cfg = Config::getInstance().audio_visualizer();
    pipeline_ = std::make_unique<AudioPipeline>(cfg.input.sample_rate);
    capture_ = AudioCapture::create();

    AudioConfig acfg;
    acfg.device = cfg.input.device;
    acfg.sample_rate = cfg.input.sample_rate;
    acfg.buffer_size = cfg.input.buffer_size;
    acfg.channels = 2;

    capture_->setCallback([this](const float* data, size_t len) {
        std::vector<float> samples(data, data + len);
        pipeline_->processAudioFrame(samples);
        latest_patterns_ = pipeline_->getPatterns();
    });
    capture_->init(acfg);
    capture_->start();
}

void AudioVisualizerDemo::update(float) {
    // No-op, patterns updated in callback
}

void AudioVisualizerDemo::render() {
    if (!renderer_) return;
    renderer_->renderDebugPoints(latest_patterns_);
}

void AudioVisualizerDemo::cleanup() {
    if (capture_) {
        capture_->stop();
    }
    pipeline_.reset();
    capture_.reset();
}

void AudioVisualizerDemo::handleKeyboard(unsigned char) {}
void AudioVisualizerDemo::handleMouse(int, int, int) {}

} // namespace workbench
} // namespace sep
