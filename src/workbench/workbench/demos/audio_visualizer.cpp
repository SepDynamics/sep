#include "audio_visualizer.hpp"
#include "../config.hpp"
#ifndef SEP_WORKBENCH_DEMO
// Only include real implementation headers when not in demo mode
#include <audio/capture.h>
#include <audio/pipeline.h>
#endif
#include <glm/vec3.hpp>
#include <glm/gtc/constants.hpp>

namespace sep {
namespace workbench {

using namespace sep::audio;

void AudioVisualizerDemo::init() {
#ifdef SEP_WORKBENCH_DEMO
    // Use the full implementation classes from sep_engine_wrapper.h
    pipeline_ = std::make_unique<AudioPipeline>(44100); // Default sample rate
    capture_ = AudioCapture::create();

    AudioCapture::AudioConfig acfg;
    acfg.device = "default";
    acfg.sample_rate = 44100;
    acfg.buffer_size = 1024;
    acfg.channels = 2;

    capture_->setCallback([this](const float* data, size_t len) {
        std::vector<float> samples(data, data + len);
        pipeline_->processAudioFrame(samples);
        latest_patterns_ = pipeline_->getPatterns();
    });
    
    // Initialize with default patterns
    latest_patterns_.push_back(glm::vec3(0.5f, 0.5f, 0.5f));
    latest_patterns_.push_back(glm::vec3(0.7f, 0.3f, 0.6f));
    latest_patterns_.push_back(glm::vec3(0.3f, 0.7f, 0.4f));
#else
    const auto& cfg = ConfigManager::getInstance().getEngineConfig().audio_visualizer();
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
#endif
}

void AudioVisualizerDemo::update(float dt) {
#ifdef SEP_WORKBENCH_DEMO
    // Simple update for workbench demo
    std::vector<glm::vec3> visual_patterns;
    for (const auto& pattern : latest_patterns_) {
        // Apply simple transformations based on pattern_mapping_ values
        float freq = pattern.x * pattern_mapping_.frequency_scale;
        float amp = pattern.y * pattern_mapping_.amplitude_scale;
        float evolution = pattern.z * pattern_mapping_.evolution_sensitivity;
        
        visual_patterns.push_back(glm::vec3(freq, amp, evolution));
    }
    latest_visual_patterns_ = std::move(visual_patterns);
    
    // Simulate pattern evolution
    for (auto& pattern : latest_patterns_) {
        pattern.x += (std::rand() % 100 - 50) * 0.001f * dt;
        pattern.y += (std::rand() % 100 - 50) * 0.001f * dt;
        pattern.z += (std::rand() % 100 - 50) * 0.001f * dt;
        
        pattern = glm::clamp(pattern, glm::vec3(0.0f), glm::vec3(1.0f));
    }
#else
    const auto& cfg = ConfigManager::getInstance().getEngineConfig().audio_visualizer();
    
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
        renderer_->setRoughnessMode("evolution");  // Surface detail based on evolution
    }

    latest_visual_patterns_ = std::move(visual_patterns);
#endif
}

void AudioVisualizerDemo::render() {
#ifdef SEP_WORKBENCH_DEMO
    // Use the CyclesRenderer implementation from sep_engine_wrapper.h
    if (renderer_) {
        renderer_->renderPatternState(latest_visual_patterns_);
    }
#else
    if (!renderer_) return;
    
    // Use Cycles renderer for advanced visualization
    renderer_->renderPatternState(latest_visual_patterns_);
#endif
}

void AudioVisualizerDemo::cleanup() {
    if (capture_) {
        capture_->stop();
    }
    pipeline_.reset();
    capture_.reset();
    
    // Clear pattern data
    latest_patterns_.clear();
    latest_visual_patterns_.clear();
}

void AudioVisualizerDemo::handleKeyboard(unsigned char key) {
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
void AudioVisualizerDemo::handleMouse(int, int, int) {}

} // namespace workbench
} // namespace sep
