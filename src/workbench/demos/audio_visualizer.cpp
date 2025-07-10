#include "audio_visualizer.hpp"
#include <config.hpp>
#include <audio/capture.h>
#include <audio/pipeline.h>
#include <glm/vec3.hpp>

namespace sep {
namespace workbench {

using namespace sep::audio;

void AudioVisualizerDemo::on_load() {
    const auto& cfg = Config::getInstance().audio_visualizer();
    pipeline_ = std::make_unique<AudioPipeline>(cfg.input.sample_rate);
    capture_ = AudioCapture::create();

    AudioConfig acfg;
    acfg.source = cfg.input.device;
    acfg.rate = cfg.input.sample_rate;
    acfg.buffer_frames = cfg.input.buffer_size;
    acfg.channels = 2;

    capture_->setCallback([this](const float* data, size_t len) {
        std::vector<float> samples(data, data + len);
        pipeline_->processAudioFrame(samples);
        latest_patterns_ = pipeline_->getPatterns();
    });
    capture_->init(acfg);
    capture_->start();
}

void AudioVisualizerDemo::on_update(float dt) {
    (void)dt; // Unused parameter
    const auto& cfg = Config::getInstance().audio_visualizer();
    
    // Map audio patterns to visual patterns using config parameters
    std::vector<glm::vec3> visual_patterns;
    for (const auto& pattern : latest_patterns_) {
        // Scale frequency components
        float freq = pattern.x * cfg.pattern_mapping.frequency_scale;
        // Scale amplitude
        float amp = pattern.y * cfg.pattern_mapping.amplitude_scale;
        // Apply evolution sensitivity
        float evolution = pattern.z * cfg.pattern_mapping.evolution_sensitivity;
        
        visual_patterns.push_back(glm::vec3(freq, amp, evolution));
    }

    // Update visualization parameters
    if (renderer_) {
        renderer_->setColorMode("frequency");  // Color based on frequency
        renderer_->setEmissionMode("amplitude");  // Emission based on amplitude
        renderer_->setRoughnessMode(1);  // Surface detail based on evolution (using mode 1)
    }

    latest_visual_patterns_ = std::move(visual_patterns);
}

void AudioVisualizerDemo::on_render() {
    if (!renderer_) return;
    
    // Use Cycles renderer for advanced visualization
    renderer_->renderPatternState(latest_visual_patterns_);
}

void AudioVisualizerDemo::on_unload() {
    if (capture_) {
        capture_->stop();
    }
    pipeline_.reset();
    capture_.reset();
}

void AudioVisualizerDemo::on_key_press(int key) {
    const float SCALE_STEP = 0.1f;
    
    switch (key) {
        case 'f':  // Increase frequency scale
            pattern_mapping_.frequency_scale += SCALE_STEP;
            break;
        case 'F':  // Decrease frequency scale
            pattern_mapping_.frequency_scale = std::max(0.1f, pattern_mapping_.frequency_scale - SCALE_STEP);
            break;
        case 'a':  // Increase amplitude scale
            pattern_mapping_.amplitude_scale += SCALE_STEP;
            break;
        case 'A':  // Decrease amplitude scale
            pattern_mapping_.amplitude_scale = std::max(0.1f, pattern_mapping_.amplitude_scale - SCALE_STEP);
            break;
        case 'e':  // Increase evolution sensitivity
            pattern_mapping_.evolution_sensitivity += SCALE_STEP;
            pattern_mapping_.evolution_sensitivity = std::min(1.0f, pattern_mapping_.evolution_sensitivity);
            break;
        case 'E':  // Decrease evolution sensitivity
            pattern_mapping_.evolution_sensitivity = std::max(0.1f, pattern_mapping_.evolution_sensitivity - SCALE_STEP);
            break;
        case 'r':  // Reset to defaults
            pattern_mapping_.frequency_scale = 0.5f;
            pattern_mapping_.amplitude_scale = 1.0f;
            pattern_mapping_.evolution_sensitivity = 0.75f;
            break;
    }
}
void AudioVisualizerDemo::on_mouse(int, int, int) {}

} // namespace workbench
} // namespace sep
