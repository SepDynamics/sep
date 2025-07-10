#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <iostream>

#include "../../workbench_demo_adapter.hpp"
#include "audio_visualizer.hpp"
#include "config.hpp"

namespace sep {
namespace workbench {

// AudioVisualizerDemo implementation
void AudioVisualizerDemo::on_load() {
    std::cout << "Initializing Audio Visualizer Demo..." << std::endl;
    
    // Get configuration
    const auto& cfg = Config::getInstance().audio_visualizer();
    
    // In a stub implementation, we don't actually create real audio capture/pipeline
    // Just generate random data for visualization
    std::cout << "Using FFT size: " << cfg.input.fft_size << std::endl;
    std::cout << "Using buffer size: " << cfg.input.buffer_size << std::endl;
    
    // Initialize pattern mapping from config
    pattern_mapping_.frequency_scale = cfg.pattern_mapping.frequency_scale;
    pattern_mapping_.amplitude_scale = cfg.pattern_mapping.amplitude_scale;
    pattern_mapping_.evolution_sensitivity = cfg.pattern_mapping.evolution_sensitivity;
    
    std::cout << "Audio Visualizer Demo initialized successfully." << std::endl;
}

void AudioVisualizerDemo::on_update(float dt) {
    (void)dt; // Unused parameter
    
    // Get configuration
    const auto& cfg = Config::getInstance().audio_visualizer();
    
    // Use FFT size as spectrum size
    const int spectrum_size = cfg.input.fft_size;
    
    // Generate random patterns for visualization
    latest_patterns_.clear();
    latest_patterns_.reserve(spectrum_size);
    
    for (int i = 0; i < spectrum_size; i++) {
        float x = (float)i / spectrum_size * 2.0f - 1.0f;
        // Use amplitude_scale as sensitivity
        float y = (rand() % 1000) / 1000.0f * 0.5f * cfg.pattern_mapping.amplitude_scale;
        float z = 0.0f;
        
        latest_patterns_.push_back(glm::vec3(x, y, z));
    }
}

void AudioVisualizerDemo::on_render() {
    // In a stub implementation, we don't actually render anything
    // This would be implemented in a real version
}

void AudioVisualizerDemo::on_unload() {
    latest_patterns_.clear();
}

void AudioVisualizerDemo::on_key_press(int key) {
    // Handle keyboard input for audio visualizer
    switch (key) {
        case '1':
            // Change visualization mode
            break;
        case '2':
            // Adjust sensitivity
            break;
    }
}

void AudioVisualizerDemo::on_mouse(int x, int y, int button) {
    // Not used in this demo
    (void)x;      // Unused parameter
    (void)y;      // Unused parameter
    (void)button; // Unused parameter
}

// Factory function implementation
std::unique_ptr<Demo> createAudioVisualizerDemo(Window* window, Renderer* renderer, UIManager* uiManager) {
    (void)window;   // Unused parameter
    (void)renderer; // Unused parameter
    (void)uiManager; // Unused parameter
    
    auto demo = std::make_unique<AudioVisualizerDemo>();
    return demo;
}

} // namespace workbench
} // namespace sep