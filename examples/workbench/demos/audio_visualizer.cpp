#include "audio_visualizer.hpp"
#include "blender/cycles_renderer.h"
#include <cmath>

namespace sep {
namespace workbench {

void AudioVisualizerDemo::init() {
    const auto& cfg = Config::getInstance().audio_visualizer();
    pattern_processor_ = std::make_unique<pattern::PatternProcessor>();
    audio_capture_ = audio::createAudioCapture();
    if (audio_capture_) {
        audio::AudioConfig ac{};
        ac.device = cfg.input.device;
        ac.sample_rate = cfg.input.sample_rate;
        ac.buffer_size = cfg.input.buffer_size;
        ac.fft_size = cfg.input.fft_size;
        audio_capture_->init(ac);
    audio_capture_->setCallback([this](const float* data, size_t frames) {
        processAudio(data, frames);
    });
        audio_capture_->start();
    }
}

void AudioVisualizerDemo::processAudio(const float* data, size_t frames) {
    // Very simple amplitude measure
    float sum = 0.0f;
    for (size_t i = 0; i < frames; ++i) sum += std::abs(data[i]);
    float amplitude = sum / static_cast<float>(frames);
    const auto& cfg = Config::getInstance().audio_visualizer();
    evolution_rate_ = amplitude * cfg.pattern_mapping.amplitude_scale;
}

void AudioVisualizerDemo::update(float dt) {
    (void)dt;
    pattern_processor_->evolvePatterns();
    // Patterns could be updated based on evolution_rate_ here
}

void AudioVisualizerDemo::render() {
    // Rendering integration would go here
}

void AudioVisualizerDemo::cleanup() {
    if (audio_capture_) {
        audio_capture_->stop();
        audio_capture_.reset();
    }
    pattern_processor_.reset();
}

void AudioVisualizerDemo::handleKeyboard(unsigned char) {}
void AudioVisualizerDemo::handleMouse(int, int, int) {}

} // namespace workbench
} // namespace sep
