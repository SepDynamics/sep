#pragma once

#include "types.h"

namespace sep::quantum::manifold {
// Default configuration values used across the engine.
extern ::sep::MemoryThresholdConfig memory;
extern ::sep::QuantumThresholdConfig quantum;
extern ::sep::config::CudaConfig cuda;
extern ::sep::LogConfig api;
}


