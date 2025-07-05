#include "config.hpp"
#include <fstream>
#include <spdlog/spdlog.h>

namespace sep {
namespace workbench {

bool Config::load(const std::filesystem::path& path) {
    try {
        std::ifstream file(path);
        if (!file.is_open()) {
            spdlog::error("Failed to open config file: {}", path.string());
            return false;
        }

        nlohmann::json json;
        file >> json;

        // Window config
        auto& window = json["window"];
        window_ = {
            window["title"].get<std::string>(),
            window["width"].get<int>(),
            window["height"].get<int>(),
            window["fullscreen"].get<bool>(),
            window["vsync"].get<bool>()
        };

        // Engine config
        auto& engine = json["engine"];
        engine_ = {
            engine["cuda_enabled"].get<bool>(),
            engine["metrics_enabled"].get<bool>(),
            engine["log_level"].get<std::string>()
        };

        // Genesis Pattern config
        auto& genesis = json["demos"]["genesis_pattern"];
        auto& initial = genesis["initial_pattern"];
        auto& viz = genesis["visualization"];
        genesis_pattern_ = {
            {
                {
                    initial["dimensions"][0].get<int>(),
                    initial["dimensions"][1].get<int>(),
                    initial["dimensions"][2].get<int>()
                },
                initial["evolution_rate"].get<float>(),
                initial["coherence_threshold"].get<float>()
            },
            {
                viz["color_mode"].get<std::string>(),
                viz["emission_mode"].get<std::string>(),
                viz["roughness_mode"].get<std::string>()
            }
        };

        // Audio Visualizer config
        auto& audio = json["demos"]["audio_visualizer"];
        auto& input = audio["input"];
        auto& mapping = audio["pattern_mapping"];
        audio_visualizer_ = {
            {
                input["device"].get<std::string>(),
                input["sample_rate"].get<int>(),
                input["buffer_size"].get<int>(),
                input["fft_size"].get<int>()
            },
            {
                mapping["frequency_scale"].get<float>(),
                mapping["amplitude_scale"].get<float>(),
                mapping["evolution_sensitivity"].get<float>()
            }
        };

        // Memory Garden config
        auto& garden = json["demos"]["memory_garden"];
        auto& layout = garden["layout"];
        auto& garden_viz = garden["visualization"];
        memory_garden_ = {
            {
                layout["stm_radius"].get<float>(),
                layout["mtm_radius"].get<float>(),
                layout["ltm_radius"].get<float>()
            },
            {
                garden_viz["show_connections"].get<bool>(),
                garden_viz["connection_opacity"].get<float>(),
                garden_viz["pattern_scale"].get<float>()
            }
        };

        // Cosmo demo config
        auto& cosmo = json["demos"]["cosmo"];
        cosmo_ = {
            cosmo["box_size"].get<float>(),
            cosmo["timestep"].get<float>()
        };

        // Renderer config
        auto& renderer = json["renderer"];
        auto& cycles = renderer["cycles"];
        renderer_ = {
            {
                cycles["samples"].get<int>(),
                cycles["denoising"].get<bool>(),
                cycles["device"].get<std::string>()
            }
        };

        return true;
    }
    catch (const std::exception& e) {
        spdlog::error("Failed to parse config file: {}", e.what());
        return false;
    }
}

bool Config::save(const std::filesystem::path& path) const {
    try {
        nlohmann::json json;

        // Window config
        json["window"] = {
            {"title", window_.title},
            {"width", window_.width},
            {"height", window_.height},
            {"fullscreen", window_.fullscreen},
            {"vsync", window_.vsync}
        };

        // Engine config
        json["engine"] = {
            {"cuda_enabled", engine_.cuda_enabled},
            {"metrics_enabled", engine_.metrics_enabled},
            {"log_level", engine_.log_level}
        };

        // Genesis Pattern config
        json["demos"]["genesis_pattern"] = {
            {"initial_pattern", {
                {"dimensions", {
                    genesis_pattern_.initial_pattern.dimensions[0],
                    genesis_pattern_.initial_pattern.dimensions[1],
                    genesis_pattern_.initial_pattern.dimensions[2]
                }},
                {"evolution_rate", genesis_pattern_.initial_pattern.evolution_rate},
                {"coherence_threshold", genesis_pattern_.initial_pattern.coherence_threshold}
            }},
            {"visualization", {
                {"color_mode", genesis_pattern_.visualization.color_mode},
                {"emission_mode", genesis_pattern_.visualization.emission_mode},
                {"roughness_mode", genesis_pattern_.visualization.roughness_mode}
            }}
        };

        // Audio Visualizer config
        json["demos"]["audio_visualizer"] = {
            {"input", {
                {"device", audio_visualizer_.input.device},
                {"sample_rate", audio_visualizer_.input.sample_rate},
                {"buffer_size", audio_visualizer_.input.buffer_size},
                {"fft_size", audio_visualizer_.input.fft_size}
            }},
            {"pattern_mapping", {
                {"frequency_scale", audio_visualizer_.pattern_mapping.frequency_scale},
                {"amplitude_scale", audio_visualizer_.pattern_mapping.amplitude_scale},
                {"evolution_sensitivity", audio_visualizer_.pattern_mapping.evolution_sensitivity}
            }}
        };

        // Memory Garden config
        json["demos"]["memory_garden"] = {
            {"layout", {
                {"stm_radius", memory_garden_.layout.stm_radius},
                {"mtm_radius", memory_garden_.layout.mtm_radius},
                {"ltm_radius", memory_garden_.layout.ltm_radius}
            }},
            {"visualization", {
                {"show_connections", memory_garden_.visualization.show_connections},
                {"connection_opacity", memory_garden_.visualization.connection_opacity},
                {"pattern_scale", memory_garden_.visualization.pattern_scale}
            }}
        };

        // Cosmo demo config
        json["demos"]["cosmo"] = {
            {"box_size", cosmo_.box_size},
            {"timestep", cosmo_.timestep}
        };

        // Renderer config
        json["renderer"] = {
            {"cycles", {
                {"samples", renderer_.cycles.samples},
                {"denoising", renderer_.cycles.denoising},
                {"device", renderer_.cycles.device}
            }}
        };

        std::ofstream file(path);
        if (!file.is_open()) {
            spdlog::error("Failed to open config file for writing: {}", path.string());
            return false;
        }

        file << json.dump(2);
        return true;
    }
    catch (const std::exception& e) {
        spdlog::error("Failed to save config file: {}", e.what());
        return false;
    }
}

} // namespace workbench
} // namespace sep