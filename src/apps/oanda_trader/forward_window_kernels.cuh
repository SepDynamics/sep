#pragma once

#include <cuda_runtime.h>
#include <vector>
#include <cstdint>

#include "tick_cuda_kernels.cuh"

namespace sep::apps::cuda {

// Data structure for forward-looking window results
struct ForwardWindowResult {
    double mean_price;
    float coherence;
    float stability;
    float confidence;
};

// CUDA kernel declaration
__global__ void forwardWindowKernel(
    const TickData* ticks,
    size_t tick_count,
    ForwardWindowResult* results,
    uint64_t window_size_ns
);

// Host function declaration
cudaError_t calculateForwardWindowsCuda(
    CudaContext& context,
    const std::vector<TickData>& host_ticks,
    std::vector<ForwardWindowResult>& host_results,
    uint64_t window_size_ns
);

} // namespace sep::apps::cuda
