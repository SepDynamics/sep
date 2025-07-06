#pragma once

#include <cstddef>
#include "quantum/quantum_manifold_optimizer.h"

namespace sep::quantum::manifold {

struct MemoryConfig {
    float      adaptive_threshold_rate{0.0f};
    float      hamiltonian_coupling{0.0f};
    float      predictive_horizon_ms{0.0f};
    std::size_t pattern_cache_size{0};
};

struct QuantumConfig {
    int   manifold_dimensions{0};
    float coherence_modulation_factor{0.0f};
    float rupture_detection_sensitivity{0.0f};
    int   qfh_hierarchy_depth{0};
};

extern MemoryConfig memory;
extern QuantumConfig quantum;
extern ::sep::config::CudaConfig cuda;
extern ::sep::config::APIConfig api;

} // namespace sep::quantum::manifold

