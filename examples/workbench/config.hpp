#pragma once

#include <string>
#include <memory>
#include <glm/glm.hpp>

namespace sep {
namespace workbench {

struct CosmoConfig {
    float box_size = 100.0f;
    float time_step = 0.01f;
};

struct GenesisPatternConfig {
    struct InitialPattern {
        float evolution_rate = 0.1f;
        float coherence_threshold = 0.5f;
        float dimensions[3] = {32.0f, 32.0f, 32.0f};
    } initial_pattern;

    struct Evolution {
        float rate_multiplier = 1.0f;
        float rate_step = 1.25f;
        float min_rate = 0.01f;
        float max_rate = 10.0f;
        int iterations_per_frame = 1;
    } evolution;

    struct Visualization {
        std::string color_mode = "quantum";
        std::string emission_mode = "coherence";
        int roughness_mode = 1;
        float coherence_threshold = 0.05f;
    } visualization;

    struct Controls {
        float rotation_sensitivity = 0.01f;
        float zoom_sensitivity = 0.01f;
        float min_zoom = 0.1f;
        float max_zoom = 10.0f;
    } controls;

    struct SaveState {
        float evolution_rate = 0.1f;
        float coherence_threshold = 0.5f;
        
        struct ViewSettings {
            float rotation = 0.0f;
            float zoom = 1.0f;
            bool wireframe = false;
        } view_settings;
    } save_state;
};

class Config {
public:
    static Config& getInstance() {
        static Config instance;
        return instance;
    }

    GenesisPatternConfig& genesis_pattern() { return genesis_pattern_; }
    const GenesisPatternConfig& genesis_pattern() const { return genesis_pattern_; }
    
    CosmoConfig& cosmo() { return cosmo_; }
    const CosmoConfig& cosmo() const { return cosmo_; }

private:
    Config() = default;
    GenesisPatternConfig genesis_pattern_;
    CosmoConfig cosmo_;
};

} // namespace workbench
} // namespace sep