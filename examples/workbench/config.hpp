#pragma once

#include <nlohmann/json.hpp>
#include <string>
#include <memory>
#include <filesystem>

namespace sep {
namespace workbench {

struct ViewSettings {
    float rotation{0.0f};
    float zoom{1.0f};
    bool wireframe{false};
};

struct WindowConfig {
    std::string title;
    int width;
    int height;
    bool fullscreen;
    bool vsync;
};

struct EngineConfig {
    bool cuda_enabled;
    bool metrics_enabled;
    std::string log_level;
};

struct GenesisPatternConfig {
    struct {
        std::array<int, 3> dimensions;
        float evolution_rate;
        float coherence_threshold;
    } initial_pattern;

    struct Evolution {
        float rate_multiplier{1.0f};
        float rate_step{1.1f};
        float max_rate{2.0f};
        float min_rate{0.01f};
        int iterations_per_frame{1};
    } evolution;

    struct {
        std::string color_mode;
        std::string emission_mode;
        std::string roughness_mode;
        float coherence_threshold{0.1f};
    } visualization;

    struct {
        float rotation_sensitivity{0.005f};
        float zoom_sensitivity{0.01f};
        float min_zoom{0.5f};
        float max_zoom{5.0f};
    } controls;

    struct {
        float evolution_rate{0.1f};
        float coherence_threshold{0.5f};
        ViewSettings view_settings{};
    } save_state;
};

struct AudioVisualizerConfig {
    struct {
        std::string device;
        int sample_rate;
        int buffer_size;
        int fft_size;
    } input;

    struct {
        float frequency_scale;
        float amplitude_scale;
        float evolution_sensitivity;
    } pattern_mapping;
};

struct MemoryGardenConfig {
    struct {
        float stm_radius;
        float mtm_radius;
        float ltm_radius;
    } layout;

    struct {
        bool show_connections;
        float connection_opacity;
        float pattern_scale;
    } visualization;
};

struct RendererConfig {
    struct {
        int samples;
        bool denoising;
        std::string device;
    } cycles;
};

class Config {
public:
    static Config& getInstance() {
        static Config instance;
        return instance;
    }

    bool load(const std::filesystem::path& path);
    bool save(const std::filesystem::path& path) const;

    const WindowConfig& window() const { return window_; }
    const EngineConfig& engine() const { return engine_; }
    const GenesisPatternConfig& genesis_pattern() const { return genesis_pattern_; }
    const AudioVisualizerConfig& audio_visualizer() const { return audio_visualizer_; }
    const MemoryGardenConfig& memory_garden() const { return memory_garden_; }
    const RendererConfig& renderer() const { return renderer_; }

private:
    Config() = default;
    
    WindowConfig window_;
    EngineConfig engine_;
    GenesisPatternConfig genesis_pattern_;
    AudioVisualizerConfig audio_visualizer_;
    MemoryGardenConfig memory_garden_;
    RendererConfig renderer_;
};

} // namespace workbench
} // namespace sep