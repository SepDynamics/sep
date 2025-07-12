#ifndef SEP_CORE_TYPES_H
#define SEP_CORE_TYPES_H

#include "memory/types.h"
#include "compat/shim.h"
#include <glm/glm.hpp>
#include <nlohmann/json.hpp>
#include <string>
#include <vector>
#include <complex>

namespace sep {

// --- Canonical Data Structures ---

namespace quantum {
    enum class RelationshipType { ENTANGLEMENT, CAUSAL, SIMILARITY };

    struct QuantumState {
        enum class Status { SUPERPOSITION, COHERENT, COLLAPSED };
        float coherence{0.0f};
        float stability{0.0f};
        float entropy{0.0f};
        float phase{0.0f};
        float evolution_rate{0.0f};
        float energy{0.0f};
        float coupling_strength{0.0f};
        float mutation_rate{0.0f};
        int generation{0};
        int mutation_count{0};
        ::sep::memory::MemoryTierEnum memory_tier{::sep::memory::MemoryTierEnum::STM};
        float access_frequency{0.0f};
        Status state{Status::SUPERPOSITION};
    };

    struct PatternRelationship {
      std::string targetId;
      float strength;
      RelationshipType type;
    };
} // namespace quantum

// This is the SINGLE canonical definition of a Pattern.
struct Pattern {
    shim::string id;
    int generation{0};
    glm::vec4 position{0.0f};
    glm::vec4 velocity{0.0f};
    glm::vec4 attributes{0.0f};
    std::complex<float> amplitude{0.0f};
    quantum::QuantumState quantum_state{};
    std::vector<quantum::PatternRelationship> relationships;
    std::vector<float> data;
};

// --- Aliases for Backwards Compatibility ---
// Modules that used different names can use these type aliases.
namespace pattern {
    using PatternData = sep::Pattern;
}
namespace workbench {
    using Pattern = sep::Pattern;
}
// Note: The original sep::quantum::Pattern is now sep::Pattern

// --- Configuration Structs ---
namespace config {
    // ... (Your config structs like MemoryThresholdConfig, APIConfig, etc. go here) ...
    // (No changes needed to the config structs themselves for now)
    struct MemoryThresholdConfig {
        float promote_stm_to_mtm{0.7f};
        float promote_mtm_to_ltm{0.9f};
        // ... all other fields
    };
    // ... etc for all other config structs
} // namespace config

} // namespace sep

#endif // SEP_CORE_TYPES_H
