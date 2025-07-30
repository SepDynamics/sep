#include "quantum/manifold_config.h"

#include "engine/internal/types.h"

namespace sep::quantum::manifold {

    ::sep::memory::MemoryThresholdConfig memory{.stm_size = 1 << 20,
                                                .mtm_size = 4 << 20,
                                                .ltm_size = 16 << 20,
                                                .promote_stm_to_mtm = 0.7f,
                                                .promote_mtm_to_ltm = 0.9f,
                                                .demote_threshold = 0.3f,
                                                .fragmentation_threshold = 0.3f,
                                                .use_unified_memory = true,
                                                .enable_compression = true,
                                                .stm_to_mtm_min_gen = 5};

    ::sep::QuantumThresholdConfig quantum{.ltm_coherence_threshold = 0.9f,
                                          .mtm_coherence_threshold = 0.6f,
                                          .stability_threshold = 0.8f};

    ::sep::config::CudaConfig cuda{.use_gpu = true,
                                   .max_memory_mb = 8192,
                                   .batch_size = 1024,
                                   .gpu_memory_limit = 0.9f,
                                   .enable_profiling = false};

} // namespace sep::quantum::manifold
