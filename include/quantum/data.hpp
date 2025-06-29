#pragma once

#include "compat/shim.h"
#include "quantum/types.h"
#include "memory/types.h"
#include <glm/glm.hpp>
#include <vector>

namespace sep {
namespace pattern {

struct PatternData {
    shim::string id;
    int generation{0};
    glm::vec4 position{0.0f};
    glm::vec4 velocity{0.0f};
    glm::vec4 attributes{0.0f};
    float coherence{0.0f};
    float stability{0.0f};
    float entropy{0.0f};
    float mutation_rate{0.0f};
    std::uint32_t mutation_count{0};
    ::sep::memory::MemoryTierEnum memory_tier{::sep::memory::MemoryTierEnum::STM};
    std::vector<quantum::PatternRelationship> relationships;
};

struct PatternConfig {
    float update_threshold{0.1f};
    bool enable_mutations{true};
    std::size_t max_patterns{1000};
    std::size_t batch_size{64};
};

inline bool isValidConfig(const PatternConfig& cfg) {
    return cfg.max_patterns > 0 && cfg.batch_size > 0;
}

enum class PatternResult {
    SUCCESS = 0,
    INVALID_ARGUMENT = -1,
    ALLOCATION_FAILED = -2,
    PROCESSING_ERROR = -3
};

} // namespace pattern
} // namespace sep
