#pragma once

#include "core/types.h"

namespace sep::quantum::manifold {
// Default configuration values shared across the engine. They are declared here
// and defined in the corresponding source file so that multiple translation
// units can reference the same instances.

extern ::sep::config::MemoryThresholdConfig memory;
extern ::sep::config::QuantumThresholdConfig quantum;
extern ::sep::config::CudaConfig cuda;
extern ::sep::config::APIConfig api;

} // namespace sep::quantum::manifold
