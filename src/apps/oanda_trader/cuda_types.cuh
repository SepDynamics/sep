#pragma once

#include <cuda_runtime.h>
#include <cstdint>

namespace sep::apps::cuda {

// Data structures for CUDA kernels
struct TickData {
    double price;
    double bid;
    double ask;
    uint64_t timestamp; // nanoseconds since epoch
    double volume;
};

struct WindowSpec {
    uint64_t start_time;
    uint64_t end_time;
    int window_type; // 0 = hourly, 1 = daily
};

struct WindowResult {
    double mean_price;
    double volatility;
    double price_change;
    double pip_change;
    size_t tick_count;
    uint64_t window_start;
    uint64_t window_end;
};

struct ForwardWindowResult {
    double mean_price;
    float coherence;
    float stability;
    float confidence;
    float entropy;
    int rupture_count;
    int flip_count;
};

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