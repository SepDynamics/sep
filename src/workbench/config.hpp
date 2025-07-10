#pragma once

#include <array>
#include <filesystem>
#include <nlohmann/json.hpp>
#include <string>
#include <vector>

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
    struct InitialPattern {
        std::array<int,3> dimensions;
        float evolution_rate;
        float coherence_threshold;
    } initial_pattern;

    struct Visualization {
        std::string color_mode;
        std::string emission_mode;
        std::string roughness_mode;
    } visualization;
};

struct AudioVisualizerConfig {
    struct Input {
        std::string device;
        int sample_rate;
        int buffer_size;
        int fft_size;
    } input;

    struct PatternMapping {
        float frequency_scale;
        float amplitude_scale;
        float evolution_sensitivity;
    } pattern_mapping;
};

struct MemoryGardenConfig {
    struct Layout {
        float stm_radius;
        float mtm_radius;
        float ltm_radius;
    } layout;

    struct Visualization {
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

struct AnnealingSimConfig {
    std::vector<float> temperature_schedule;
    int particle_count{0};
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

struct FlockingConfig {
    int agent_count{0};
    float cohesion_weight{1.0f};
    float separation_weight{1.0f};
    float alignment_weight{1.0f};
    float neighbor_radius{0.0f};
    float separation_distance{0.0f};
    float max_speed{0.0f};
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

struct NeuralConfig {
    int neuron_count;
    float threshold;
    float decay;
    float input_strength;
};

struct CosmoConfig {
    float box_size;
    float time_step;
};

struct RendererConfig {
    struct Cycles {
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
    const AnnealingSimConfig& annealing_sim() const { return annealing_sim_; }
    const RendererConfig& renderer() const { return renderer_; }
    const OptimizerConfig& optimizer() const { return optimizer_; }
    const DrugDiscoveryConfig& drug_discovery() const { return drug_discovery_; }
    const FlockingConfig& flocking() const { return flocking_; }
    const NeuralDemoConfig& neural_demo() const { return neural_demo_; }
    const DigitalPhysicsConfig& digital_physics() const { return digital_physics_; }

private:
    Config() = default;

    WindowConfig window_{};
    EngineConfig engine_{};
    GenesisPatternConfig genesis_pattern_{};
    AudioVisualizerConfig audio_visualizer_{};
    MemoryGardenConfig memory_garden_{};
    AnnealingConfig annealing_{};
    AnnealingSimConfig annealing_sim_{};
    RendererConfig renderer_{};
    OptimizerConfig optimizer_{};
    DrugDiscoveryConfig drug_discovery_{};
    FlockingConfig flocking_{};
    NeuralDemoConfig neural_demo_{};
    DigitalPhysicsConfig digital_physics_{};
};

} // namespace workbench
} // namespace sep
