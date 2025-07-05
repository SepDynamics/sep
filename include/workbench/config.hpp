#pragma once

#include <nlohmann/json.hpp>
#include <string>
#include <memory>
#include <filesystem>

namespace sep {
namespace workbench {

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

    struct {
        std::string color_mode;
        std::string emission_mode;
        std::string roughness_mode;
    } visualization;
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

struct FlockingConfig {
    int agent_count;
    float neighbor_radius;
    float separation_distance;
    float max_speed;
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
    const FlockingConfig& flocking() const { return flocking_; }
    const RendererConfig& renderer() const { return renderer_; }

private:
    Config() = default;
    
    WindowConfig window_;
    EngineConfig engine_;
    GenesisPatternConfig genesis_pattern_;
    AudioVisualizerConfig audio_visualizer_;
    MemoryGardenConfig memory_garden_;
    FlockingConfig flocking_;
    RendererConfig renderer_;
};

} // namespace workbench
} // namespace sep