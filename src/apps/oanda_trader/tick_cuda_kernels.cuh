#pragma once

#include <cuda_runtime.h>
#include <vector>
#include <cstdint>
#include "cuda_types.cuh"

namespace sep::apps::cuda {

// CUDA constants
constexpr int CUDA_BLOCK_SIZE = 256;
constexpr size_t MAX_TICK_HISTORY = 1000000;
constexpr size_t MAX_CALCULATION_WINDOWS = 10000;
constexpr uint64_t HOURLY_STEP_NS = 60000000000ULL; // 1 minute in nanoseconds
constexpr uint64_t DAILY_STEP_NS = 3600000000000ULL; // 1 hour in nanoseconds

// CUDA kernel declarations
__global__ void calculateRollingWindowsKernel(
    const TickData* ticks,
    size_t tick_count,
    const WindowSpec* window_specs,
    size_t window_count,
    WindowResult* results,
    uint64_t* window_timestamps);

__global__ void calculateRollingWindowsOptimized(
    const TickData* ticks,
    size_t tick_count,
    const WindowSpec* window_specs,
    size_t window_count,
    WindowResult* results,
    uint64_t current_time,
    uint64_t hourly_window_ns,
    uint64_t daily_window_ns);

__global__ void calculateMultiTimeframeWindows(
    const TickData* ticks,
    size_t tick_count,
    WindowResult* hourly_results,
    WindowResult* daily_results,
    size_t result_count,
    uint64_t base_time,
    uint64_t hourly_step_ns,
    uint64_t daily_step_ns,
    uint64_t hourly_window_ns,
    uint64_t daily_window_ns);

__device__ void calculateWindowStats(
    const TickData* ticks,
    size_t tick_count,
    uint64_t window_start,
    uint64_t window_end,
    WindowResult& result);

// Host function declarations
cudaError_t initializeCudaDevice(CudaContext& context, int device_id = 0);
cudaError_t cleanupCudaDevice(CudaContext& context);

cudaError_t calculateWindowsCuda(
    CudaContext& context,
    const std::vector<TickData>& host_ticks,
    std::vector<WindowResult>& hourly_results,
    std::vector<WindowResult>& daily_results,
    uint64_t current_time,
    uint64_t hourly_window_ns,
    uint64_t daily_window_ns);

cudaError_t calculateForwardWindowsCuda(
    CudaContext& context,
    const std::vector<TickData>& ticks,
    std::vector<ForwardWindowResult>& results,
    uint64_t window_size_ns);

} // namespace sep::apps::cuda
