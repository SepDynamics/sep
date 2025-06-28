#ifndef SEP_QUANTUM_TYPES_H
#define SEP_QUANTUM_TYPES_H

#include <glm/glm.hpp>
#include <vector>
#include <string>

namespace sep {

enum class MemoryTierEnum {
    STM,
    MTM,
    LTM
};

} // namespace sep

namespace sep::quantum {

enum class RelationshipType {
    ENTANGLEMENT,
    CAUSAL,
    SIMILARITY
};

struct QuantumState {
    float coherence;
    float stability;
    float entropy;
    float mutation_rate;
    int generation;
    int mutation_count;
    MemoryTierEnum memory_tier;
    float access_frequency;
};

struct PatternRelationship {
    std::string targetId;
    float strength;
    RelationshipType type;
};

struct Pattern {
    std::string id;
    glm::vec4 position;
    QuantumState quantum_state;
    std::vector<PatternRelationship> relationships;
    std::vector<float> data;
    std::vector<std::string> parent_ids;
    uint64_t timestamp;
    uint64_t last_accessed;
    uint64_t last_modified;
};

struct ProcessingConfig {
    size_t max_patterns{10000};
    float mutation_rate{0.01f};
    float ltm_coherence_threshold{0.9f};
    float mtm_coherence_threshold{0.6f};
    float stability_threshold{0.8f};
    bool enable_cuda{false};
};

struct ProcessingResult {
    bool success{false};
    Pattern pattern;
    std::string error_message;
};

struct BatchProcessingResult {
    bool success{false};
    std::vector<ProcessingResult> results;
    std::string error_message;
};

} // namespace sep::quantum

#endif // SEP_QUANTUM_TYPES_H
