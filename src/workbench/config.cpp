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

        auto& window = json["window"];
        window_ = {
            window.value("title", std::string{"SEP"}),
            window.value("width", 800),
            window.value("height", 600),
            window.value("fullscreen", false),
            window.value("vsync", true)
        };

        auto& engine = json["engine"];
        engine_ = {
            engine.value("cuda_enabled", false),
            engine.value("metrics_enabled", false),
            engine.value("log_level", std::string{"info"})
        };

        auto& genesis = json["demos"]["genesis_pattern"];
        auto& initial = genesis["initial_pattern"];
        auto& viz = genesis["visualization"];
        genesis_pattern_ = {
            {
                { initial["dimensions"][0].get<int>(),
                  initial["dimensions"][1].get<int>(),
                  initial["dimensions"][2].get<int>() },
                initial["evolution_rate"].get<float>(),
                initial["coherence_threshold"].get<float>()
            },
            {
                viz["color_mode"].get<std::string>(),
                viz["emission_mode"].get<std::string>(),
                viz["roughness_mode"].get<std::string>()
            }
        };

        auto& audio = json["demos"]["audio_visualizer"];
        auto& input = audio["input"];
        auto& mapping = audio["pattern_mapping"];
        audio_visualizer_ = {
            { input["device"].get<std::string>(),
              input["sample_rate"].get<int>(),
              input["buffer_size"].get<int>(),
              input["fft_size"].get<int>() },
            { mapping["frequency_scale"].get<float>(),
              mapping["amplitude_scale"].get<float>(),
              mapping["evolution_sensitivity"].get<float>() }
        };

        auto& garden = json["demos"]["memory_garden"];
        auto& layout = garden["layout"];
        auto& viz_g = garden["visualization"];
        memory_garden_ = {
            { layout["stm_radius"].get<float>(),
              layout["mtm_radius"].get<float>(),
              layout["ltm_radius"].get<float>() },
            { viz_g["show_connections"].get<bool>(),
              viz_g["connection_opacity"].get<float>(),
              viz_g["pattern_scale"].get<float>() }
        };

        auto& anneal = json["demos"]["annealing"];
        annealing_ = {
            anneal["initial_temperature"].get<float>(),
            anneal["cooling_rate"].get<float>(),
            anneal["particle_count"].get<int>()
        };

        if (json["demos"].contains("annealing_sim")) {
            auto& sim = json["demos"]["annealing_sim"];
            annealing_sim_.particle_count = sim["particle_count"].get<int>();
            for (auto& t : sim["temperature_schedule"])
                annealing_sim_.temperature_schedule.push_back(t.get<float>());
        }

        auto& renderer = json["renderer"];
        auto& cycles = renderer["cycles"];
        renderer_ = {
            { cycles["samples"].get<int>(),
              cycles["denoising"].get<bool>(),
              cycles["device"].get<std::string>() }
        };

        auto& optimizer = json["optimizer"];
        optimizer_ = { optimizer["iterations"].get<int>(),
                       optimizer["mutation_rate"].get<float>() };

        // Load drug discovery demo config
        if (json["demos"].contains("drug_discovery")) {
            auto& drug = json["demos"]["drug_discovery"];
            drug_discovery_.optimizer.iterations = drug["optimizer"]["iterations"].get<int>();
            drug_discovery_.optimizer.mutation_rate = drug["optimizer"]["mutation_rate"].get<float>();
        }

        // Load flocking demo config
        if (json["demos"].contains("flocking")) {
            auto& flock = json["demos"]["flocking"];
            flocking_.agent_count = flock["agent_count"].get<int>();
            flocking_.cohesion_weight = flock.value("cohesion_weight", 1.0f);
            flocking_.separation_weight = flock.value("separation_weight", 1.0f);
            flocking_.alignment_weight = flock.value("alignment_weight", 1.0f);
            flocking_.neighbor_radius = flock["neighbor_radius"].get<float>();
            flocking_.separation_distance = flock.value("separation_distance", 0.0f);
            flocking_.max_speed = flock["max_speed"].get<float>();
        }

        // Load neural demo config
        if (json["demos"].contains("neural_demo")) {
            auto& neural = json["demos"]["neural_demo"];
            neural_demo_.network.neuron_count = neural["network"]["neuron_count"].get<int>();
            neural_demo_.network.connection_prob = neural["network"]["connection_prob"].get<float>();
            neural_demo_.neuron.threshold = neural["neuron"]["threshold"].get<float>();
            neural_demo_.neuron.decay = neural["neuron"]["decay"].get<float>();
            neural_demo_.neuron.input_strength = neural["neuron"]["input_strength"].get<float>();
        }

        // Load digital physics demo config
        if (json["demos"].contains("digital_physics")) {
            auto& physics = json["demos"]["digital_physics"];
            digital_physics_.grid.width = physics["grid"]["width"].get<int>();
            digital_physics_.grid.height = physics["grid"]["height"].get<int>();
            
            if (physics["rules"].contains("birth")) {
                for (auto& b : physics["rules"]["birth"])
                    digital_physics_.rules.birth.push_back(b.get<int>());
            }
            
            if (physics["rules"].contains("survival")) {
                for (auto& s : physics["rules"]["survival"])
                    digital_physics_.rules.survival.push_back(s.get<int>());
            }
        }

        return true;
    } catch (const std::exception& e) {
        spdlog::error("Failed to parse config file: {}", e.what());
        return false;
    }
}

bool Config::save(const std::filesystem::path& path) const {
    try {
        nlohmann::json json;

        json["window"] = {{"title", window_.title},
                          {"width", window_.width},
                          {"height", window_.height},
                          {"fullscreen", window_.fullscreen},
                          {"vsync", window_.vsync}};

        json["engine"] = {{"cuda_enabled", engine_.cuda_enabled},
                          {"metrics_enabled", engine_.metrics_enabled},
                          {"log_level", engine_.log_level}};

        json["demos"]["genesis_pattern"] = {
            {"initial_pattern",
             {{"dimensions",
               {genesis_pattern_.initial_pattern.dimensions[0],
                genesis_pattern_.initial_pattern.dimensions[1],
                genesis_pattern_.initial_pattern.dimensions[2]}},
              {"evolution_rate", genesis_pattern_.initial_pattern.evolution_rate},
              {"coherence_threshold",
               genesis_pattern_.initial_pattern.coherence_threshold}}},
            {"visualization",
             {{"color_mode", genesis_pattern_.visualization.color_mode},
              {"emission_mode", genesis_pattern_.visualization.emission_mode},
              {"roughness_mode",
               genesis_pattern_.visualization.roughness_mode}}}};

        json["demos"]["audio_visualizer"] = {
            {"input",
             {{"device", audio_visualizer_.input.device},
              {"sample_rate", audio_visualizer_.input.sample_rate},
              {"buffer_size", audio_visualizer_.input.buffer_size},
              {"fft_size", audio_visualizer_.input.fft_size}}},
            {"pattern_mapping",
             {{"frequency_scale",
               audio_visualizer_.pattern_mapping.frequency_scale},
              {"amplitude_scale", audio_visualizer_.pattern_mapping.amplitude_scale},
              {"evolution_sensitivity",
               audio_visualizer_.pattern_mapping.evolution_sensitivity}}}};

        json["demos"]["memory_garden"] = {
            {"layout",
             {{"stm_radius", memory_garden_.layout.stm_radius},
              {"mtm_radius", memory_garden_.layout.mtm_radius},
              {"ltm_radius", memory_garden_.layout.ltm_radius}}},
            {"visualization",
             {{"show_connections", memory_garden_.visualization.show_connections},
              {"connection_opacity",
               memory_garden_.visualization.connection_opacity},
              {"pattern_scale", memory_garden_.visualization.pattern_scale}}}};

        json["demos"]["annealing"] = {
            {"initial_temperature", annealing_.initial_temperature},
            {"cooling_rate", annealing_.cooling_rate},
            {"particle_count", annealing_.particle_count}};

        if (!annealing_sim_.temperature_schedule.empty() ||
            annealing_sim_.particle_count != 0) {
            json["demos"]["annealing_sim"] = {
                {"temperature_schedule", annealing_sim_.temperature_schedule},
                {"particle_count", annealing_sim_.particle_count}};
        }

        json["renderer"] = {
            {"cycles",
             {{"samples", renderer_.cycles.samples},
              {"denoising", renderer_.cycles.denoising},
              {"device", renderer_.cycles.device}}}};

        json["optimizer"] = {
            {"iterations", optimizer_.iterations},
            {"mutation_rate", optimizer_.mutation_rate}};

        // Save drug discovery demo config
        json["demos"]["drug_discovery"] = {
            {"optimizer", {
                {"iterations", drug_discovery_.optimizer.iterations},
                {"mutation_rate", drug_discovery_.optimizer.mutation_rate}
            }}
        };

        // Save flocking demo config
        json["demos"]["flocking"] = {
            {"agent_count", flocking_.agent_count},
            {"cohesion_weight", flocking_.cohesion_weight},
            {"separation_weight", flocking_.separation_weight},
            {"alignment_weight", flocking_.alignment_weight},
            {"neighbor_radius", flocking_.neighbor_radius},
            {"separation_distance", flocking_.separation_distance},
            {"max_speed", flocking_.max_speed}
        };

        // Save neural demo config
        json["demos"]["neural_demo"] = {
            {"network", {
                {"neuron_count", neural_demo_.network.neuron_count},
                {"connection_prob", neural_demo_.network.connection_prob}
            }},
            {"neuron", {
                {"threshold", neural_demo_.neuron.threshold},
                {"decay", neural_demo_.neuron.decay},
                {"input_strength", neural_demo_.neuron.input_strength}
            }}
        };

        // Save digital physics demo config
        json["demos"]["digital_physics"] = {
            {"grid", {
                {"width", digital_physics_.grid.width},
                {"height", digital_physics_.grid.height}
            }},
            {"rules", {
                {"birth", digital_physics_.rules.birth},
                {"survival", digital_physics_.rules.survival}
            }}
        };

        std::ofstream file(path);
        if (!file.is_open()) {
            spdlog::error("Failed to open config file for writing: {}",
                          path.string());
            return false;
        }
        file << json.dump(2);
        return true;
    } catch (const std::exception& e) {
        spdlog::error("Failed to save config file: {}", e.what());
        return false;
    }
}

}  // namespace workbench
}  // namespace sep
