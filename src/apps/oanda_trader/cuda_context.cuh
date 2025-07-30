#pragma once

#include <cuda_runtime.h>
#include "tick_data.cuh"

namespace sep::apps::cuda {

struct ForwardWindowResult;
struct WindowResult;
struct WindowSpec;

// CUDA context for managing device memory and streams
struct CudaContext {
    bool initialized = false;
    cudaDeviceProp device_props;
    
    // Device memory pointers
    TickData* d_ticks = nullptr;
    WindowResult* d_hourly_results = nullptr;
    WindowResult* d_daily_results = nullptr;
    WindowSpec* d_window_specs = nullptr;
    
    // CUDA stream for async operations
    cudaStream_t stream = nullptr;
};

} // namespace sep::apps::cuda