#pragma once

#include <nlohmann/json.hpp>
#include <string>
#include <memory>
#include <filesystem>
#include <array>

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

struct AnnealingConfig {
    float initial_temperature;
    float cooling_rate;
    int particle_count;
};

struct DrugDiscoveryConfig {
    struct {
        int iterations;
        float mutation_rate;
    } optimizer;
};

struct DrugDiscoveryConfig {
    struct {
        int iterations;
        float mutation_rate;
    } optimizer;
};

struct NeuralDemoConfig {
    struct {
        int neuron_count;
        float connection_prob;
    } network;
    struct {
        float threshold;
        float decay;
        float input_strength;
    } neuron;
};

struct DrugDiscoveryConfig {
    int iterations;
    float mutation_rate;
};

struct DigitalPhysicsConfig {
    struct {
        int width;
        int height;
    } grid;
    struct {
        std::string rule;
    } rules;
};

struct AnnealingConfig {
    int particle_count;
    float initial_temperature;
    float cooling_rate;
};

struct DigitalPhysicsConfig {
    struct {
        int width;
        int height;
    } grid;

    struct {
        int birth;
        int survive_min;
        int survive_max;
    } rules;
};

struct FlockingConfig {
    int agent_count;
    float cohesion_weight;
    float separation_weight;
    float alignment_weight;
    float neighbor_radius;
    float max_speed;
};

struct FlockingConfig {
    float cohesion_weight;
    float separation_weight;
    float alignment_weight;
};

struct FlockingConfig {
    int agent_count;
    float cohesion_weight;
    float separation_weight;
    float alignment_weight;
    float neighbor_radius;
    float max_speed;
};

struct FlockingConfig {
    int agent_count;
    float cohesion_weight;
    float separation_weight;
    float alignment_weight;
    float neighbor_radius;
    float max_speed;
};

struct FlockingConfig {
    int agent_count;
    float neighbor_radius;
    float max_speed;
};

struct DigitalPhysicsConfig {
    struct {
        int width;
        int height;
    } grid;

    struct {
        std::vector<int> birth;
        std::vector<int> survival;
    } rules;
};

struct FlockingConfig {
    int agent_count;
    float neighbor_radius;
    float max_speed;
};

struct FlockingConfig {
    int agent_count;
    float cohesion_weight;
    float separation_weight;
    float alignment_weight;
    float neighbor_radius;
    float max_speed;
};

struct NeuralConfig {
    int neuron_count;
    float threshold;
    float decay;
};

struct RendererConfig {
    struct {
        int samples;
        bool denoising;
        std::string device;
    } cycles;
};

struct OptimizerConfig {
    int iterations;
    float mutation_rate;
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
    const AnnealingConfig& annealing() const { return annealing_; }
    const RendererConfig& renderer() const { return renderer_; }
    const OptimizerConfig& optimizer() const { return optimizer_; }

private:
    Config() = default;
    
    WindowConfig window_;
    EngineConfig engine_;
    GenesisPatternConfig genesis_pattern_;
    AudioVisualizerConfig audio_visualizer_;
    MemoryGardenConfig memory_garden_;
    AnnealingConfig annealing_;
    RendererConfig renderer_;
    OptimizerConfig optimizer_;
};

} // namespace workbench
} // namespace sep