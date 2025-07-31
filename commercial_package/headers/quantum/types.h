#pragma once

#include <complex>
#include <glm/glm.hpp>

#include "engine/internal/pattern_types.h"
#include "engine/internal/standard_includes.h"
#include "memory/types.h"
#include "quantum/config.h"

// Forward declarations to avoid circular dependency
namespace sep {
    struct PinState {
        uint64_t pin_id;
        double value;
        float coherence;
        uint64_t tick;
        std::vector<uint32_t> bits;
    };
}

namespace sep {
namespace quantum {

struct QuantumState {
    float coherence{0.0f};
    float stability{0.0f};
    float entropy{0.0f};
    float mutation_rate{0.0f};
    float evolution_rate{0.0f};
    float energy{0.0f};
    float coupling_strength{0.0f};
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
    ENTANGLEMENT = Entanglement,  // Add alias for compatibility
    Causality
};

struct PatternRelationship {
    std::string targetId;
    float strength;
    RelationshipType type;
};

struct Pattern {
    std::string id;
    glm::vec4 position{0.0f};
    glm::vec3 momentum{0.0f};
    QuantumState quantum_state{};
    std::vector<PatternRelationship> relationships{};
    sep::compat::PatternData data{};
    std::vector<std::string> parent_ids{};
    uint64_t timestamp{0};
    uint64_t last_accessed{0};
    uint64_t last_modified{0};
    int generation{0};
    float coherence{0.0f};
    glm::vec4 velocity{0.0f};
    glm::vec4 attributes{0.0f};
    std::complex<float> amplitude{1.0f, 0.0f};
    QuantumState state;
    uint64_t last_updated{0};
};

} // namespace quantum

namespace compat {
// Alias for backward compatibility or for a stable API
using Pattern = quantum::Pattern;
} // namespace compat

} // namespace sep