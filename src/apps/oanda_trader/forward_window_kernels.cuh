#pragma once

#include <cuda_runtime.h>
#include <vector>
#include <cstdint>
#include "cuda_types.cuh"

namespace sep::apps::cuda {

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
