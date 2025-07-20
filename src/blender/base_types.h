#pragma once

// Standard headers
#include "compat/shim.h"

// Include result definitions and math compatibility
#include "core/common.h"
#include "compat/math_common.h"

#include <glm/glm.hpp>
#include <cstdint>

// Now include other dependencies with GLM compatibility
#ifndef GLM_FORCE_NO_CTOR_INIT
#define GLM_FORCE_NO_CTOR_INIT
#endif
#ifndef GLM_FORCE_EXPLICIT_CTOR
#define GLM_FORCE_EXPLICIT_CTOR
#endif
#ifndef GLM_FORCE_SIZE_T_LENGTH
#define GLM_FORCE_SIZE_T_LENGTH
#endif
#ifndef GLM_FORCE_PURE
#define GLM_FORCE_PURE
#endif
#ifndef GLM_FORCE_INLINE
#define GLM_FORCE_INLINE
#endif
#ifndef GLM_ENABLE_EXPERIMENTAL
#define GLM_ENABLE_EXPERIMENTAL
#endif

namespace sep {

// Explicitly bring standard integer types into scope for CUDA
using uint32_t = std::uint32_t;
using uint64_t = std::uint64_t;
using int32_t = std::int32_t;
using int64_t = std::int64_t;

namespace pattern {

// Basic type definitions
using ObjectHandle = std::uint64_t;


// Pattern state tracking
struct PatternState {
  float coherence;     // Pattern stability (0-1)
  float entropy;       // Information content
  uint32_t generation; // Evolution generation
  uint32_t mutations;  // Mutation count
  uint32_t flags;      // State flags
};

// Pattern metrics
struct PatternMetrics {
  float avg_coherence;        // Average pattern stability
  float peak_entropy;         // Maximum entropy observed
  size_t active_patterns;     // Number of active patterns
  uint64_t updates_processed; // Total updates processed

  struct {
    float process_time;    // Processing time in ms
    float gpu_utilization; // GPU usage (0-1)
  } performance;

  struct {
    size_t mutations;                // Total mutations
    float stability;                 // Overall stability
    size_t mtm_candidates;           // Patterns eligible for MTM promotion
    size_t ltm_candidates;           // Patterns eligible for LTM promotion
    float avg_relationship_strength; // Average relationship strength between
                                     // patterns
    size_t relationship_count;       // Number of relationships above threshold
    size_t path_count;               // Number of evolutionary paths
    float avg_path_length;           // Average length of evolutionary paths
  } evolution;
};

// Region definition for batch processing
struct Region {
  float min_x, min_y, min_z;
  float max_x, max_y, max_z;

  bool contains(const float *vertex) const {
    return vertex[0] >= min_x && vertex[0] <= max_x && vertex[1] >= min_y &&
           vertex[1] <= max_y && vertex[2] >= min_z && vertex[2] <= max_z;
  }
};

// Resource types for monitoring
enum class ResourceType {
  GPU_MEMORY,
  HOST_MEMORY,
  COMPUTE_RESOURCES,
  STORAGE_RESOURCES
};

} // namespace pattern
} // namespace sep
