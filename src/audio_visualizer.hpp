#pragma once

#include "workbench_demo_adapter.hpp"
#include <memory>
#include <vector>
#include <glm/glm.hpp>
#include <glm/vec3.hpp>

namespace sep {
namespace workbench {

// Forward declarations for audio capture components
namespace audio {
    class AudioCapture;
    class AudioPipeline;
    
    struct AudioConfig {
        int sample_rate = 44100;
        int buffer_size = 2048;
        int channels = 2;
    };
}

// Simple configuration singleton
class Config {
public:
    static Config& getInstance() {
        static Config instance;
        return instance;
    }
    
    struct AudioVisualizerConfig {
        int spectrum_size = 64;
        float sensitivity = 1.0f;
        bool use_smoothing = true;
    };
    
    const AudioVisualizerConfig& audio_visualizer() const { return av_config_; }
    
private:
    Config() = default;
    AudioVisualizerConfig av_config_;
};

// Audio visualizer demo implementation
class AudioVisualizerDemo : public Demo {
public:
    void on_load() override;
    void on_update(float dt) override;
    void on_render() override;
    void on_unload() override;
    void on_key_press(int key) override;
    void on_mouse(int x, int y, int button) override;
    
private:
    std::unique_ptr<audio::AudioCapture> capture_;
    std::unique_ptr<audio::AudioPipeline> pipeline_;
    std::vector<glm::vec3> latest_patterns_;
    Window* window_ = nullptr;
    Renderer* renderer_ = nullptr;
    UIManager* uiManager_ = nullptr;
};

// Audio capture interface
namespace audio {
    class AudioCapture {
    public:
        virtual ~AudioCapture() = default;
        virtual bool init(const AudioConfig& config) = 0;
        virtual bool start() = 0;
        virtual void stop() = 0;
        
        // Factory method to create appropriate implementation
        static std::unique_ptr<AudioCapture> create();
    };
    
    class AudioPipeline {
    public:
        virtual ~AudioPipeline() = default;
        virtual void processSamples(const std::vector<float>& samples) = 0;
        virtual std::vector<glm::vec3> getVisualPatterns() const = 0;
        
        // Factory method to create appropriate implementation
        static std::unique_ptr<AudioPipeline> create(int spectrum_size = 64);
    };
}

// Factory function
std::unique_ptr<Demo> createAudioVisualizerDemo(Window* window, Renderer* renderer, UIManager* uiManager);

} // namespace workbench
} // namespace sep