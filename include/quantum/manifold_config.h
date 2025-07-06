#pragma once

#include "core/types.h"

namespace sep::quantum::manifold {

// Default configuration values used across the engine.
// They are declared here and defined in a single translation unit
// to avoid duplicate symbol errors during linking.

extern ::sep::config::MemoryThresholdConfig memory;
extern ::sep::config::QuantumThresholdConfig quantum;
extern ::sep::config::CudaConfig cuda;
extern ::sep::config::APIConfig api;

} // namespace sep::quantum::manifold
