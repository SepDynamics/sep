#pragma once

#include "core/types.h"

namespace sep::quantum::manifold {
// Default configuration values used across the engine. Definitions live in
// `src/quantum/manifold_config.cpp` to avoid duplication across translation
// units.

extern ::sep::config::MemoryThresholdConfig memory;
extern ::sep::config::QuantumThresholdConfig quantum;
extern ::sep::config::CudaConfig cuda;
extern ::sep::config::APIConfig api;
}


