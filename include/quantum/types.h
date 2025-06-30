#ifndef SEP_QUANTUM_TYPES_H
#define SEP_QUANTUM_TYPES_H

#include "memory/types.h"
#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <nlohmann/json.hpp>

namespace sep::quantum {

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
    sep::memory::MemoryTierEnum memory_tier{sep::memory::MemoryTierEnum::STM};
    float access_frequency{0.0f};
    Status state{Status::SUPERPOSITION};
};

struct PatternRelationship {
  std::string targetId;
  float strength;
  RelationshipType type;
};

struct Pattern {
  std::string id;
  glm::vec4 position;
  glm::vec3 momentum{0.0f};
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

inline void to_json(nlohmann::json& j, const ProcessingConfig& c) {
    j = nlohmann::json{
        {"max_patterns", c.max_patterns},
        {"mutation_rate", c.mutation_rate},
        {"ltm_coherence_threshold", c.ltm_coherence_threshold},
        {"mtm_coherence_threshold", c.mtm_coherence_threshold},
        {"stability_threshold", c.stability_threshold},
        {"enable_cuda", c.enable_cuda}
    };
}

inline void from_json(const nlohmann::json& j, ProcessingConfig& c) {
    c.max_patterns = j.value("max_patterns", static_cast<size_t>(10000));
    c.mutation_rate = j.value("mutation_rate", 0.01f);
    c.ltm_coherence_threshold = j.value("ltm_coherence_threshold", 0.9f);
    c.mtm_coherence_threshold = j.value("mtm_coherence_threshold", 0.6f);
    c.stability_threshold = j.value("stability_threshold", 0.8f);
    c.enable_cuda = j.value("enable_cuda", false);
}

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
