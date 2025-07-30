#include "quantum/pattern_evolution.h"

#include <string.h>
#include <time.h>
#include <unistd.h>

#include <chrono>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <nlohmann/json.hpp>
#include <random>

#include "engine/internal/types.h"  // For PatternData/PatternConfig
#include "engine/internal/types.h"
#include "quantum/quantum_processor_qfh.h"

// Standard Library Includes
#include <cmath>
#include <cstring>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "quantum/pattern_evolution_bridge.h"

sep::compat::PatternData sep::quantum::mcp::PatternEvolution::evolvePattern(
    const nlohmann::json& config, const std::string& patternId)
{
    sep::compat::PatternData pattern;

    // Set pattern ID
    if (!patternId.empty())
    {
        std::strncpy(pattern.id, patternId.c_str(), sizeof(pattern.id) - 1);
        pattern.id[sizeof(pattern.id) - 1] = '\0';
    }
    else
    {
        // Pattern ID based on time of occurrence - the most relevant identifier
        std::string id_str = "pat-" + std::to_string(time(0));
        std::strncpy(pattern.id, id_str.c_str(), sizeof(pattern.id) - 1);
        pattern.id[sizeof(pattern.id) - 1] = '\0';
    }

    // Extract configuration values
    float coherence = config.value("coherence", 0.5f);
    float stability = config.value("stability", 0.5f);
    float entropy = config.value("entropy", 0.3f);
    float mutation_rate = config.value("mutation_rate", 0.1f);
    
    // Extract position data if available
    if (config.contains("position") && config["position"].is_array() && config["position"].size() >= 4)
    {
        float x = config["position"][0].get<float>();
        float y = config["position"][1].get<float>();
        float z = config["position"][2].get<float>();
        float w = config["position"][3].get<float>();
        
        pattern.position = glm::vec4(x, y, z, w);
    }
    else
    {
        // Default position
        pattern.position = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
    }
    
    // Set metadata properties
    pattern.quantum_state.coherence = coherence;
    pattern.quantum_state.stability = stability;
    pattern.quantum_state.entropy = entropy;
    pattern.quantum_state.mutation_rate = mutation_rate;
    
    // Set generation count
    pattern.generation = config.value("generation", 0) + 1;
    
    // Process relationships if available
    if (config.contains("relationships") && config["relationships"].is_array())
    {
        for (const auto& rel_json : config["relationships"])
        {
            sep::compat::PatternRelationship rel;

            std::string target_id = rel_json.value("target", "");
            if (!target_id.empty() &&
                pattern.relationship_count < sep::compat::PatternData::MAX_RELATIONSHIPS)
            {
                sep::compat::PatternData::HostRelationship host_rel;
                host_rel.target_id = target_id;
                host_rel.strength = rel_json.value("strength", 0.0f);
                pattern.host_relationships.push_back(host_rel);

                // Also update the fixed array
                auto& rel = pattern.relationships[pattern.relationship_count];
                std::strncpy(rel.target_id, target_id.c_str(), sizeof(rel.target_id) - 1);
                rel.target_id[sizeof(rel.target_id) - 1] = '\0';
                rel.strength = rel_json.value("strength", 0.0f);
                pattern.relationship_count++;
            }
        }
    }
    
    return pattern;
}
std::vector<sep::compat::PatternData> sep::quantum::mcp::PatternEvolution::getPatterns(
    const nlohmann::json& args)
{
    std::vector<sep::compat::PatternData> patterns;
    auto json_patterns = args.value("patterns", nlohmann::json::array());
    float min_coherence = args.value("min_coherence", 0.0f);
    float min_stability = args.value("min_stability", 0.0f);

    for (const auto& jp : json_patterns)
    {
        auto p = fromJson(jp);
        if (p.get_id().empty())
        {
            // Pattern ID based on time of occurrence
            std::string id_str = "pat-" + std::to_string(time(0));
            std::strncpy(p.id, id_str.c_str(), sizeof(p.id) - 1);
            p.id[sizeof(p.id) - 1] = '\0';
        }
        if (p.quantum_state.coherence >= min_coherence &&
            p.quantum_state.stability >= min_stability)
        {
            patterns.push_back(p);
        }
    }

    return patterns;
}

float sep::quantum::mcp::PatternEvolution::calculateRelationshipStrength(
    const sep::compat::PatternData& pattern1, const sep::compat::PatternData& pattern2)
{
    // Calculate Euclidean distance between position vectors
    glm::vec4 diff = pattern1.position - pattern2.position;
    float distance = glm::length(diff);
    float data_similarity = 1.0f / (1.0f + distance);
    
    // Calculate metadata similarity
    float coherence_diff = std::abs(pattern1.quantum_state.coherence - pattern2.quantum_state.coherence);
    float stability_diff = std::abs(pattern1.quantum_state.stability - pattern2.quantum_state.stability);
    float entropy_diff = std::abs(pattern1.quantum_state.entropy - pattern2.quantum_state.entropy);
    
    float metadata_similarity = 1.0f - (coherence_diff + stability_diff + entropy_diff) / 3.0f;
    
    // Combine similarities
    return (data_similarity + metadata_similarity) / 2.0f;
}

nlohmann::json sep::quantum::mcp::PatternEvolution::toJson(const sep::compat::PatternData& pattern)
{
    nlohmann::json j;

    j["id"] = std::string(pattern.id);
    j["generation"] = pattern.generation;
    
    j["position"] = {
        pattern.position.x,
        pattern.position.y,
        pattern.position.z,
        pattern.position.w
    };
    
    // Export metadata
    j["coherence"] = pattern.quantum_state.coherence;
    j["stability"] = pattern.quantum_state.stability;
    j["entropy"] = pattern.quantum_state.entropy;
    j["mutation_rate"] = pattern.quantum_state.mutation_rate;
    
    // Export relationships
    if (pattern.relationship_count > 0)
    {
        j["relationships"] = nlohmann::json::array();
        for (int i = 0; i < pattern.relationship_count; ++i)
        {
            const auto& rel = pattern.relationships[i];
            nlohmann::json rel_json;
            rel_json["target"] = std::string(rel.target_id);
            rel_json["strength"] = rel.strength;
            j["relationships"].push_back(rel_json);
        }
    }
    
    return j;
}

sep::compat::PatternData sep::quantum::mcp::PatternEvolution::fromJson(const nlohmann::json& j)
{
    sep::compat::PatternData p;

    // Import basic properties
    if (j.contains("id") && j["id"].is_string())
    {
        std::string id_str = j["id"].get<std::string>();
        std::strncpy(p.id, id_str.c_str(), sizeof(p.id) - 1);
        p.id[sizeof(p.id) - 1] = '\0';
    }
    
    p.generation = j.value("generation", 0);
    
    // Import position data
    if (j.contains("position") && j["position"].is_array() && j["position"].size() >= 4)
    {
        p.position = glm::vec4(
            j["position"][0].get<float>(),
            j["position"][1].get<float>(),
            j["position"][2].get<float>(),
            j["position"][3].get<float>()
        );
    }
    
    // Import metadata
    p.quantum_state.coherence = j.value("coherence", 0.0f);
    p.quantum_state.stability = j.value("stability", 0.0f);
    p.quantum_state.entropy = j.value("entropy", 0.0f);
    p.quantum_state.mutation_rate = j.value("mutation_rate", 0.0f);
    
    // Import relationships
    if (j.contains("relationships") && j["relationships"].is_array())
    {
        for (const auto& rel_json : j["relationships"])
        {
            if (p.relationship_count >= sep::compat::PatternData::MAX_RELATIONSHIPS) break;

            if (rel_json.contains("target") && rel_json["target"].is_string())
            {
                std::string target_str = rel_json["target"].get<std::string>();

                // Add to host relationships for easier manipulation
                sep::compat::PatternData::HostRelationship host_rel;
                host_rel.target_id = target_str;
                host_rel.strength = rel_json.value("strength", 0.0f);
                p.host_relationships.push_back(host_rel);

                // Add to fixed array
                auto& rel = p.relationships[p.relationship_count];
                std::strncpy(rel.target_id, target_str.c_str(), sizeof(rel.target_id) - 1);
                rel.target_id[sizeof(rel.target_id) - 1] = '\0';
                rel.strength = rel_json.value("strength", 0.0f);
                p.relationship_count++;
            }
        }
    }
    
    return p;
}
