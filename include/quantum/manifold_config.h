#pragma once

#include <cstdint>

// Minimal configuration structures used only by the manifold testbed. These are
// intentionally standalone so they don't interfere with the engine's existing
// configuration system.

namespace sep::quantum::manifold {

struct MemoryConfig {
    float    adaptive_threshold_rate{0.02f};
    float    hamiltonian_coupling{0.42f};
    float    predictive_horizon_ms{100.0f};
    uint32_t pattern_cache_size{10000};
};

struct QuantumConfig {
    uint32_t manifold_dimensions{8};
    float    coherence_modulation_factor{0.707f};
    float    rupture_detection_sensitivity{0.3f};
    uint32_t qfh_hierarchy_depth{5};
};

struct CudaTuningConfig {
    uint32_t warp_tile_size{16};
    uint32_t coherence_block_size{256};
    uint32_t similarity_grid_dim{32};
    bool     enable_phase_modulation{true};
};

struct APICoherenceConfig {
    float    base_coherence{0.5f};
    float    context_weight{0.3f};
    float    state_weight{0.7f};
    uint32_t superposition_states{4};
};

extern MemoryConfig       memory;
extern QuantumConfig      quantum;
extern CudaTuningConfig   cuda;
extern APICoherenceConfig api;
}

