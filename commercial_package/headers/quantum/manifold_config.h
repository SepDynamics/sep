#pragma once

#include "engine/internal/config.h"
#include "memory/types.h"
#include "quantum/config.h"

namespace sep::quantum::manifold {
// Default configuration values used across the engine.
extern ::sep::memory::MemoryThresholdConfig memory;
extern ::sep::QuantumThresholdConfig quantum;
extern ::sep::config::CudaConfig cuda;
extern ::sep::config::LogConfig api;
}


