#pragma once

#include "core/types.h"

namespace sep::quantum::manifold {
inline ::sep::config::MemoryThresholdConfig memory{
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

inline ::sep::config::QuantumThresholdConfig quantum{
  .ltm_coherence_threshold = 0.9f,
  .mtm_coherence_threshold = 0.6f,
  .stability_threshold = 0.8f
};

inline ::sep::config::CudaConfig cuda{
  .use_gpu = true,
  .max_memory_mb = 8192,
  .batch_size = 1024,
  .gpu_memory_limit = 0.9f,
  .enable_profiling = false
};

inline ::sep::config::APIConfig api{
  .max_connections = 1000,
  .timeout_ms = 5000,
  .host = "127.0.0.1",
  .port = 8080,
  .threads = 4,
  .keep_alive_timeout_ms = 15000,
  .log_level = "info",
  .enable_metrics = true,
  .max_batch_size = 1024
};
} // namespace sep::quantum::manifold

