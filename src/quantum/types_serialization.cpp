#include <algorithm>
#include <cstring>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <nlohmann/json.hpp>
#include <vector>

#include "engine/internal/pattern_types.h"
#include "engine/internal/standard_includes.h"
#include "quantum/types.h"

using namespace glm;

namespace sep::compat {
void to_json(nlohmann::json& j, const PatternData& data) {
    j = nlohmann::json::object();
    j["id"] = std::string(data.id);
    j["generation"] = data.generation;

    j["attributes"] = nlohmann::json::array();
    for (int i = 0; i < data.size; ++i) {
        j["attributes"].push_back(data.attributes[i]);
    }
    j["size"] = data.size;

    j["position"] = {data.position.x, data.position.y, data.position.z, data.position.w};
    j["velocity"] = {data.velocity.x, data.velocity.y, data.velocity.z, data.velocity.w};
    j["coherence"] = data.coherence;

    j["relationships"] = nlohmann::json::array();
    for (int i = 0; i < data.relationship_count; ++i) {
        const auto& rel = data.relationships[i];
        j["relationships"].push_back({
            {"targetId", std::string(rel.target_id)},
            {"strength", rel.strength},
            {"type", rel.type}
        });
    }
    j["relationship_count"] = data.relationship_count;

    j["quantum_state"] = {
        {"coherence", data.quantum_state.coherence},
        {"phase", data.quantum_state.phase},
        {"amplitude", data.quantum_state.amplitude},
        {"entanglement", data.quantum_state.entanglement},
        {"stability", data.quantum_state.stability},
        {"entropy", data.quantum_state.entropy},
        {"mutation_rate", data.quantum_state.mutation_rate}
    };
}

void from_json(const nlohmann::json& j, PatternData& data) {
    std::string id = j.value("id", "");
    std::strncpy(data.id, id.c_str(), PatternData::MAX_ID_LENGTH - 1);
    data.id[PatternData::MAX_ID_LENGTH - 1] = '\0';

    data.generation = j.value("generation", 0);

    data.size = 0;
    if (j.contains("attributes")) {
        const auto& attrs = j.at("attributes");
        for (size_t i = 0; i < attrs.size() && i < PatternData::MAX_ATTRIBUTES; ++i) {
            data.attributes[i] = attrs[i].get<float>();
            data.size = static_cast<int>(i) + 1;
        }
    }

    if (j.contains("position")) {
        auto pos = j.at("position");
        if (pos.size() >= 4) {
            data.position = glm::vec4(pos[0].get<float>(), pos[1].get<float>(),
                                      pos[2].get<float>(), pos[3].get<float>());
        }
    }

    if (j.contains("velocity")) {
        auto vel = j.at("velocity");
        if (vel.size() >= 4) {
            data.velocity = glm::vec4(vel[0].get<float>(), vel[1].get<float>(),
                                      vel[2].get<float>(), vel[3].get<float>());
        }
    }

    data.coherence = j.value("coherence", 0.0f);

    data.relationship_count = 0;
    if (j.contains("relationships")) {
        const auto& rels = j.at("relationships");
        for (size_t i = 0; i < rels.size() && i < PatternData::MAX_RELATIONSHIPS; ++i) {
            const auto& rj = rels[i];
            auto& rel = data.relationships[i];
            std::string tid = rj.value("targetId", "");
            std::strncpy(rel.target_id, tid.c_str(), PatternRelationship::MAX_ID_LENGTH - 1);
            rel.target_id[PatternRelationship::MAX_ID_LENGTH - 1] = '\0';
            rel.strength = rj.value("strength", 0.0f);
            rel.type = rj.value("type", std::string("default"));
            ++data.relationship_count;
        }
    }

    if (j.contains("quantum_state")) {
        const auto& qs = j.at("quantum_state");
        data.quantum_state.coherence = qs.value("coherence", 0.0f);
        data.quantum_state.phase = qs.value("phase", 0.0f);
        data.quantum_state.amplitude = qs.value("amplitude", 1.0f);
        data.quantum_state.entanglement = qs.value("entanglement", 0.0f);
        data.quantum_state.stability = qs.value("stability", 0.0f);
        data.quantum_state.entropy = qs.value("entropy", 0.0f);
        data.quantum_state.mutation_rate = qs.value("mutation_rate", 0.0f);
    }
}
}

namespace sep::quantum {

void to_json(nlohmann::json& j, const QuantumState& state) {
    j = nlohmann::json{
        {"coherence", state.coherence},
        {"stability", state.stability},
        {"entropy", state.entropy},
        {"mutation_rate", state.mutation_rate},
        {"generation", state.generation},
        {"mutation_count", state.mutation_count},
        {"memory_tier", static_cast<int>(state.memory_tier)}, 
        {"access_frequency", state.access_frequency},
        {"state", static_cast<int>(state.state)}
    };
}

void from_json(const nlohmann::json& j, QuantumState& state) {
    j.at("coherence").get_to(state.coherence);
    j.at("stability").get_to(state.stability);
    j.at("entropy").get_to(state.entropy);
    j.at("mutation_rate").get_to(state.mutation_rate);
    j.at("generation").get_to(state.generation);
    j.at("mutation_count").get_to(state.mutation_count);
    state.memory_tier = static_cast<sep::memory::MemoryTierEnum>(j.value("memory_tier", 0)); 
    j.at("access_frequency").get_to(state.access_frequency);
    state.state = static_cast<QuantumState::Status>(j.value("state", 0));
}

void to_json(nlohmann::json& j, const PatternRelationship& rel) {
    j = nlohmann::json{
        {"targetId", rel.targetId},
        {"strength", rel.strength},
        {"type", static_cast<int>(rel.type)}
    };
}

void from_json(const nlohmann::json& j, PatternRelationship& rel) {
    j.at("targetId").get_to(rel.targetId);
    j.at("strength").get_to(rel.strength);
    rel.type = static_cast<RelationshipType>(j.value("type", 0));
}


void to_json(nlohmann::json& j, const Pattern& pattern) {
    j = nlohmann::json::object();
    j["id"] = pattern.id;
    j["position"] = {pattern.position.x, pattern.position.y, pattern.position.z, pattern.position.w};
    j["momentum"] = {pattern.momentum.x, pattern.momentum.y, pattern.momentum.z};
    j["quantum_state"] = pattern.quantum_state;
    j["relationships"] = pattern.relationships;
    j["data"] = pattern.data;
    j["parent_ids"] = pattern.parent_ids;
    j["timestamp"] = pattern.timestamp;
    j["last_accessed"] = pattern.last_accessed;
    j["last_modified"] = pattern.last_modified;
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
    pattern.quantum_state = j.value("quantum_state", QuantumState{}); 
}

void to_json(nlohmann::json& j, const ProcessingConfig& c) {
    j = nlohmann::json{
        {"max_patterns", c.max_patterns},
        {"mutation_rate", c.mutation_rate},
        {"ltm_coherence_threshold", c.ltm_coherence_threshold},
        {"mtm_coherence_threshold", c.mtm_coherence_threshold},
        {"stability_threshold", c.stability_threshold},
        {"enable_cuda", c.enable_cuda}
    };
}

void from_json(const nlohmann::json& j, ProcessingConfig& c) {
    c.max_patterns = j.value("max_patterns", static_cast<size_t>(10000));
    c.mutation_rate = j.value("mutation_rate", 0.01f);
    c.ltm_coherence_threshold = j.value("ltm_coherence_threshold", 0.9f);
    c.mtm_coherence_threshold = j.value("mtm_coherence_threshold", 0.6f);
    c.stability_threshold = j.value("stability_threshold", 0.8f);
    c.enable_cuda = j.value("enable_cuda", false);
}

} // namespace sep::quantum
