#include "quantum/manifold_config.h"
#include "core/types.h"

namespace sep::quantum::manifold {

::sep::workbench::MemoryThresholdConfig memory{
    .promote_stm_to_mtm = 0.7f,
    .promote_mtm_to_ltm = 0.9f,
    .demote_threshold = 0.3f,
    .fragmentation_threshold = 0.3f,
    .stm_size = 1 << 20,
    .mtm_size = 4 << 20,
    .ltm_size = 16 << 20,
    .stm_to_mtm_min_gen = 5,
    .mtm_to_ltm_min_gen = 100,
    .use_unified_memory = true,
    .enable_compression = true
};

::sep::workbench::QuantumThresholdConfig quantum{
    .ltm_coherence_threshold = 0.9f,
    .mtm_coherence_threshold = 0.6f,
    .stability_threshold = 0.8f
};

::sep::config::CudaConfig cuda{
    .use_gpu = true,
    .max_memory_mb = 8192,
    .batch_size = 1024,
    .gpu_memory_limit = 0.9f,
    .enable_profiling = false
};

} // namespace sep::quantum::manifold
