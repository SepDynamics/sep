#include "forward_window_kernels.cuh"
#include <cuda_runtime.h>
#include <vector>

namespace sep::apps::cuda {

__global__ void forwardWindowKernel(
    const TickData* ticks,
    size_t tick_count,
    ForwardWindowResult* results,
    uint64_t window_size_ns
) {
    // Kernel implementation would go here
}

cudaError_t calculateForwardWindowsCuda(
    CudaContext& context,
    const std::vector<TickData>& host_ticks,
    std::vector<ForwardWindowResult>& host_results,
    uint64_t window_size_ns
) {
    if (host_ticks.empty()) {
        return cudaSuccess;
    }

    TickData* device_ticks = nullptr;
    ForwardWindowResult* device_results = nullptr;
    size_t tick_size = host_ticks.size() * sizeof(TickData);
    size_t result_size = host_ticks.size() * sizeof(ForwardWindowResult);

    cudaError_t err = cudaMalloc(&device_ticks, tick_size);
    if (err != cudaSuccess) {
        return err;
    }

    err = cudaMalloc(&device_results, result_size);
    if (err != cudaSuccess) {
        cudaFree(device_ticks);
        return err;
    }

    err = cudaMemcpy(device_ticks, host_ticks.data(), tick_size, cudaMemcpyHostToDevice);
    if (err != cudaSuccess) {
        cudaFree(device_ticks);
        cudaFree(device_results);
        return err;
    }

    uint32_t threads_per_block = 256;
    uint32_t num_blocks = (host_ticks.size() + threads_per_block - 1) / threads_per_block;

    forwardWindowKernel<<<num_blocks, threads_per_block>>>(
        device_ticks,
        host_ticks.size(),
        device_results,
        window_size_ns
    );

    err = cudaGetLastError();
    if (err != cudaSuccess) {
        cudaFree(device_ticks);
        cudaFree(device_results);
        return err;
    }

    host_results.resize(host_ticks.size());
    err = cudaMemcpy(host_results.data(), device_results, result_size, cudaMemcpyDeviceToHost);

    cudaFree(device_ticks);
    cudaFree(device_results);

    return err;
}

}
