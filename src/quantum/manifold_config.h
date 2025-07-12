#pragma once

#include "core/types.h"

namespace sep::quantum::manifold {
// Default configuration values used across the engine.
extern ::sep::workbench::MemoryThresholdConfig memory;
extern ::sep::workbench::QuantumThresholdConfig quantum;
extern ::sep::config::CudaConfig cuda;
extern ::sep::workbench::LogConfig api;
}


