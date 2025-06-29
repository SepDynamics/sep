#ifndef SEP_QUANTUM_TYPES_H
#define SEP_QUANTUM_TYPES_H

#include <glm/glm.hpp>
#include <vector>
#include <string>



namespace sep::quantum {

enum class RelationshipType {
    ENTANGLEMENT,
    CAUSAL,
    SIMILARITY
};

struct QuantumState {
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
    sep::memory::MemoryTierEnum memory_tier{sep::memory::MemoryTierEnum::STM};
    float access_frequency{0.0f};
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
    int error_code{0};
};

} // namespace sep::quantum

#endif // SEP_QUANTUM_TYPES_H
