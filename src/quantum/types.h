#pragma once

#include <glm/glm.hpp>

#include "engine/pattern_types.h"
#include "engine/shim.h"
#include "memory/types.h"
#include "quantum/config.h"

namespace sep {
namespace quantum {

struct QuantumState {
    float coherence{0.0f};
    float stability{0.0f};
    float entropy{0.0f};
    float mutation_rate{0.0f};
    int generation{0};
    int mutation_count{0};
    sep::memory::MemoryTierEnum memory_tier{sep::memory::MemoryTierEnum::STM};
    int access_frequency{0};

    enum class Status {
        STABLE,
        UNSTABLE,
        COLLAPSED
    };
    Status state{Status::STABLE};
    float phase{0.0f};
};

enum class RelationshipType {
    Generic,
    Entanglement,
    Causality
};

struct PatternRelationship {
    shim::string targetId;
    float strength;
    RelationshipType type;
};

struct Pattern {
    shim::string id;
    glm::vec4 position{0.0f};
    glm::vec3 momentum{0.0f};
    QuantumState quantum_state{};
    shim::vector<PatternRelationship> relationships{};
    sep::compat::PatternData data{};
    shim::vector<shim::string> parent_ids{};
    uint64_t timestamp{0};
    uint64_t last_accessed{0};
    uint64_t last_modified{0};
    QuantumState state;
    uint64_t last_updated{0};
};

} // namespace quantum

namespace compat {
// Alias for backward compatibility or for a stable API
using Pattern = quantum::Pattern;
} // namespace compat

} // namespace sep