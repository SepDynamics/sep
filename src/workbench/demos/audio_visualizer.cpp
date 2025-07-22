#include "audio_visualizer.hpp"

#include <glm/vec3.hpp>

#include "../simple_renderer.h"
#include "audio/capture.h"
#include "audio/pipeline.h"
#include "config.hpp"

namespace sep {
namespace workbench {

using namespace sep::audio;

void AudioVisualizerDemo::on_load(sep::core::Engine* engine, sep::SimpleRenderer* renderer)
{
    engine_ = engine;
    renderer_ = renderer;
    const auto& cfg = Config::getInstance().audio_visualizer();

#ifdef SEP_HAS_AUDIO
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
#else
    // Generate some initial patterns for visualization
    latest_patterns_.clear();

    // Use FFT size from config if available, otherwise default to 128
    int spectrum_size = cfg.input.fft_size > 0 ? cfg.input.fft_size : 128;

    for (int i = 0; i < spectrum_size; i++)
    {
        float x = (float)i / spectrum_size * 2.0f - 1.0f;
        float y = 0.0f;
        float z = 0.0f;
        latest_patterns_.push_back(glm::vec3(x, y, z));
    }

    // Initialize pattern mapping from config
    pattern_mapping_.frequency_scale = cfg.pattern_mapping.frequency_scale;
    pattern_mapping_.amplitude_scale = cfg.pattern_mapping.amplitude_scale;
    pattern_mapping_.evolution_sensitivity = cfg.pattern_mapping.evolution_sensitivity;
#endif
}

void AudioVisualizerDemo::on_update(float dt) {
    (void)dt; // Unused parameter
    const auto& cfg = Config::getInstance().audio_visualizer();

#ifndef SEP_HAS_AUDIO
    // In non-audio builds, generate random patterns
    const int spectrum_size = cfg.input.fft_size > 0 ? cfg.input.fft_size : 128;
    latest_patterns_.clear();
    latest_patterns_.reserve(spectrum_size);

    for (int i = 0; i < spectrum_size; i++)
    {
        float x = (float)i / spectrum_size * 2.0f - 1.0f;
        // Generate some random movement
        float y = (rand() % 1000) / 1000.0f * 0.5f * cfg.pattern_mapping.amplitude_scale;
        float z = 0.0f;
        latest_patterns_.push_back(glm::vec3(x, y, z));
    }
#endif

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
        // renderer_->setRoughnessMode(1);  // Method not available in SimpleRenderer
    }

    latest_visual_patterns_ = std::move(visual_patterns);
}

void AudioVisualizerDemo::on_render() {
    if (!renderer_) return;
    
    // Use Cycles renderer for advanced visualization
    renderer_->renderPatternState(latest_visual_patterns_);
}

void AudioVisualizerDemo::on_unload() {
#ifdef SEP_HAS_AUDIO
    if (capture_) {
        capture_->stop();
    }
    pipeline_.reset();
    capture_.reset();
#else
    latest_patterns_.clear();
    latest_visual_patterns_.clear();
#endif
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
void AudioVisualizerDemo::on_ui_render() {
    ImGui::Begin("Audio Visualizer Controls");
    
    // Pattern mapping controls
    ImGui::Text("Pattern Mapping Parameters");
    ImGui::SliderFloat("Frequency Scale", &pattern_mapping_.frequency_scale, 0.1f, 2.0f);
    ImGui::SliderFloat("Amplitude Scale", &pattern_mapping_.amplitude_scale, 0.1f, 2.0f);
    ImGui::SliderFloat("Evolution Sensitivity", &pattern_mapping_.evolution_sensitivity, 0.1f, 1.0f);
    
    if (ImGui::Button("Reset to Defaults")) {
        pattern_mapping_.frequency_scale = 0.5f;
        pattern_mapping_.amplitude_scale = 1.0f;
        pattern_mapping_.evolution_sensitivity = 0.75f;
    }
    
    // Display pattern statistics
    ImGui::Separator();
    ImGui::Text("Pattern Statistics");
    ImGui::Text("Number of patterns: %zu", latest_patterns_.size());
    
    // Audio status
#ifdef SEP_HAS_AUDIO
    ImGui::Separator();
    ImGui::Text("Audio Capture Status: %s", capture_ ? "Available" : "Not Available");
    
    if (capture_) {
        if (ImGui::Button("Toggle Audio Capture")) {
            static bool is_running = false;
            if (is_running) {
                capture_->stop();
                is_running = false;
            } else {
                capture_->start();
                is_running = true;
            }
        }
        
        // Display audio metrics if available
        auto metrics = capture_->getMetrics();
        ImGui::Text("Peak Level: %.2f", metrics.peak_level);
        ImGui::Text("RMS Level: %.2f", metrics.rms_level);
        ImGui::Text("Latency: %.1f ms", metrics.latency_ms);
        ImGui::Text("Xruns: %d", metrics.xruns);
        ImGui::Text("Total Samples: %lu", metrics.total_samples);
        ImGui::Text("Dropped Samples: %lu", metrics.dropped_samples);
    }
#endif

    ImGui::End();
}

void AudioVisualizerDemo::on_mouse(int, int, int) {}

} // namespace workbench
} // namespace sep
