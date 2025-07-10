#pragma once

#include "compat/shim.h"
#include "memory/types.h"
#include "quantum/types.h"
#include <complex>
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
  std::complex<float> amplitude{0.0f};
  ::sep::quantum::QuantumState::Status state{
      ::sep::quantum::QuantumState::Status::SUPERPOSITION};
  float phase{0.0f};
  float coherence{0.0f};
  float stability{0.0f};
  float entropy{0.0f};
  float mutation_rate{0.0f};
  std::uint32_t mutation_count{0};
  ::sep::memory::MemoryTierEnum memory_tier{::sep::memory::MemoryTierEnum::STM};
  std::vector<::sep::quantum::PatternRelationship> relationships;
  std::vector<float> data;
};

struct PatternConfig {
  float update_threshold{0.1f};
  bool enable_mutations{true};
  std::size_t max_patterns{1000};
  std::size_t batch_size{64};
};

inline bool isValidConfig(const PatternConfig &cfg) {
  return cfg.max_patterns > 0 && cfg.batch_size > 0;
}

enum class PatternResult {
  SUCCESS = 0,
  INVALID_ARGUMENT = -1,
  ALLOCATION_FAILED = -2,
  PROCESSING_ERROR = -3
};

// Converter function to convert from sep::quantum::Pattern to sep::pattern::PatternData
inline PatternData convertFromQuantumPattern(const ::sep::quantum::Pattern& pattern) {
  PatternData data;
  data.id = pattern.id;
  data.generation = pattern.quantum_state.generation;
  data.position = pattern.position;
  data.velocity = glm::vec4(pattern.momentum, 0.0f); // Convert vec3 to vec4
  data.attributes = glm::vec4(0.0f); // Default attributes
  data.amplitude = std::complex<float>(0.0f, 0.0f); // Default amplitude
  data.state = pattern.quantum_state.state;
  data.phase = pattern.quantum_state.phase;
  data.coherence = pattern.quantum_state.coherence;
  data.stability = pattern.quantum_state.stability;
  data.entropy = pattern.quantum_state.entropy;
  data.mutation_rate = pattern.quantum_state.mutation_rate;
  data.mutation_count = pattern.quantum_state.mutation_count;
  data.memory_tier = pattern.quantum_state.memory_tier;
  data.relationships = pattern.relationships;
  data.data = pattern.data;
  return data;
}

} // namespace pattern
} // namespace sep
