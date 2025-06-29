#include "quantum/types.h"
#include <nlohmann/json.hpp>
#include "compat/shim.h" // Ensure shim is included if needed
#include "memory/types.h" // Add include for MemoryTierEnum

namespace sep::quantum {

void to_json(nlohmann::json& j, const QuantumState& state) {
    j = nlohmann::json{
        {"coherence", state.coherence},
        {"stability", state.stability},
        {"entropy", state.entropy},
        {"mutation_rate", state.mutation_rate},
        {"generation", state.generation},
        {"mutation_count", state.mutation_count},
        {"memory_tier", static_cast<int>(state.memory_tier)}, // Fix: serialize memory_tier as int
        {"access_frequency", state.access_frequency}
    };
}

void from_json(const nlohmann::json& j, QuantumState& state) {
    j.at("coherence").get_to(state.coherence);
    j.at("stability").get_to(state.stability);
    j.at("entropy").get_to(state.entropy);
    j.at("mutation_rate").get_to(state.mutation_rate);
    j.at("generation").get_to(state.generation);
    j.at("mutation_count").get_to(state.mutation_count);
    state.memory_tier = static_cast<sep::memory::MemoryTierEnum>(j.value("memory_tier", 0)); // Fix: default value for memory_tier
    j.at("access_frequency").get_to(state.access_frequency);
}

void to_json(nlohmann::json& j, const sep::quantum::PatternRelationship& rel) {
    j = nlohmann::json{
        {"targetId", rel.targetId},
        {"strength", rel.strength},
        {"type", static_cast<int>(rel.type)}
    };
}

void from_json(const nlohmann::json& j, sep::quantum::PatternRelationship& rel) {
    j.at("targetId").get_to(rel.targetId);
    j.at("strength").get_to(rel.strength);
    rel.type = static_cast<RelationshipType>(j.value("type", 0)); // Fix: default value for type
}

void to_json(nlohmann::json& j, const Pattern& pattern) {
    j = nlohmann::json{
        {"id", pattern.id},
        {"position", {pattern.position.x, pattern.position.y, pattern.position.z, pattern.position.w}},
        {"momentum", {pattern.momentum.x, pattern.momentum.y, pattern.momentum.z}},
        {"quantum_state", pattern.quantum_state},
        {"relationships", pattern.relationships},
        {"data", pattern.data},
        {"parent_ids", pattern.parent_ids},
        {"timestamp", pattern.timestamp},
        {"last_accessed", pattern.last_accessed},
        {"last_modified", pattern.last_modified}
    };
}

void from_json(const nlohmann::json& j, Pattern& pattern) {
    j.at("id").get_to(pattern.id);
    auto pos = j.at("position").get<std::vector<float>>();
    pattern.position = glm::vec4(pos[0], pos[1], pos[2], pos[3]);
    if (j.contains("momentum")) {
        auto mom = j.at("momentum").get<std::vector<float>>();
        pattern.momentum = glm::vec3(mom[0], mom[1], mom[2]);
    } else {
        pattern.momentum = glm::vec3(0.0f);
    }
    j.at("relationships").get_to(pattern.relationships);
    j.at("data").get_to(pattern.data);
    j.at("parent_ids").get_to(pattern.parent_ids);
    j.at("timestamp").get_to(pattern.timestamp);
    j.at("last_accessed").get_to(pattern.last_accessed);
    j.at("last_modified").get_to(pattern.last_modified);
    pattern.quantum_state = j.value("quantum_state", QuantumState{}); // Fix: Deserialize quantum_state
}

} // namespace sep::quantum
