#pragma once

#include "shim.h"

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
        std::string level;
        std::string path;
    };

    struct AnalyticsConfig
    {
        bool enabled;
        std::string endpoint;
    };
}  // namespace config
}  // namespace sep