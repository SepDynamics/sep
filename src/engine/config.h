#pragma once

#include "engine/shim.h"
#include "memory/types.h"
#include "quantum/types.h"
#include "types.h"

namespace sep {
namespace config {

    struct CudaConfig
    {
        bool use_gpu{true};
        int max_memory_mb{8192};
        int batch_size{1024};
        float gpu_memory_limit{0.9f};
        bool enable_profiling{false};
    };

    struct LogConfig
    {
        shim::string level;
        shim::string path;
    };

    struct AnalyticsConfig
    {
        bool enabled;
        shim::string endpoint;
    };

    struct SystemConfig
    {
        sep::memory::MemoryThresholdConfig memory;
        sep::QuantumThresholdConfig quantum;
        CudaConfig cuda;
        LogConfig logging;
        AnalyticsConfig analytics;
    };
}  // namespace config
}  // namespace sep