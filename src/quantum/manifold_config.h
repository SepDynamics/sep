#pragma once

#include "engine/types.h"

namespace sep::quantum::manifold {
// Default configuration values used across the engine.
extern ::sep::MemoryThresholdConfig memory;
extern ::sep::QuantumThresholdConfig quantum;
extern ::sep::config::CudaConfig cuda;
extern ::sep::LogConfig api;
}


