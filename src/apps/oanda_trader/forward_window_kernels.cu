#include <cuda_runtime.h>
#include <device_launch_parameters.h>

#include "forward_window_kernels.cuh"

namespace sep::apps::cuda {

// Forward-looking window calculation kernel
__global__ void forwardWindowKernel(
    const TickData* ticks,
    size_t tick_count,
    ForwardWindowResult* results,
    uint64_t window_size_ns
) {
    const unsigned int idx = blockIdx.x * blockDim.x + threadIdx.x;

    if (idx >= tick_count) {
        return;
    }

    const uint64_t window_start_time = ticks[idx].timestamp;
    const uint64_t window_end_time = window_start_time + window_size_ns;



    // Damping function (exponential decay)
    const double lambda = 0.1;
    double damped_price = 0.0;
    double total_weight = 0.0;

    for (size_t i = idx; i < tick_count; ++i) {
        if (ticks[i].timestamp >= window_end_time) {
            break;
        }
        const double t = (ticks[i].timestamp - window_start_time) / 1e9; // time in seconds
        const double weight = exp(-lambda * t);
        damped_price += ticks[i].price * weight;
        total_weight += weight;
    }

    double mean = 0.0;
    if (total_weight > 0) {
        mean = damped_price / total_weight;
    } 
    results[idx].mean_price = mean;

    // Calculate coherence, stability, and confidence
    float coherence = 0.0f;
    float stability = 0.0f;
    float confidence = 0.0f;

    if (total_weight > 0) {
        // Coherence: 1 - variance / mean
        double variance = 0.0;
        for (size_t i = idx; i < tick_count; ++i) {
            if (ticks[i].timestamp >= window_end_time) {
                break;
            }
            const double t = (ticks[i].timestamp - window_start_time) / 1e9; // time in seconds
            const double weight = exp(-lambda * t);
            variance += weight * (ticks[i].price - mean) * (ticks[i].price - mean);
        }
        variance /= total_weight;
        coherence = 1.0f - static_cast<float>(variance / mean);

        // Stability: 1 - stddev / mean
        stability = 1.0f - static_cast<float>(sqrt(variance) / mean);

        // Confidence: 1 - (max - min) / mean
        double min_price = ticks[idx].price;
        double max_price = ticks[idx].price;
        for (size_t i = idx + 1; i < tick_count; ++i) {
            if (ticks[i].timestamp >= window_end_time) {
                break;
            }
            if (ticks[i].price < min_price) {
                min_price = ticks[i].price;
            }
            if (ticks[i].price > max_price) {
                max_price = ticks[i].price;
            }
        }
        confidence = 1.0f - static_cast<float>((max_price - min_price) / mean);
    }

    results[idx].coherence = coherence;
    results[idx].stability = stability;
    results[idx].confidence = confidence;
}

cudaError_t calculateForwardWindowsCuda(
    CudaContext& context,
    const std::vector<TickData>& host_ticks,
    std::vector<ForwardWindowResult>& host_results,
    uint64_t window_size_ns
) {
    if (!context.initialized) {
        return cudaErrorNotReady;
    }

    const size_t tick_count = host_ticks.size();
    host_results.resize(tick_count);

    // Allocate device memory for results
    ForwardWindowResult* d_results = nullptr;
    cudaMalloc(&d_results, tick_count * sizeof(ForwardWindowResult));

    // Copy ticks to device
    cudaMemcpy(context.d_ticks, host_ticks.data(), tick_count * sizeof(TickData), cudaMemcpyHostToDevice);

    // Launch kernel
    const dim3 blockSize(CUDA_BLOCK_SIZE);
    const dim3 gridSize((tick_count + CUDA_BLOCK_SIZE - 1) / CUDA_BLOCK_SIZE);
    forwardWindowKernel<<<gridSize, blockSize, 0, context.stream>>>(
        context.d_ticks,
        tick_count,
        d_results,
        window_size_ns
    );

    // Copy results back to host
    cudaMemcpy(host_results.data(), d_results, tick_count * sizeof(ForwardWindowResult), cudaMemcpyDeviceToHost);

    // Free device memory
    cudaFree(d_results);

    return cudaGetLastError();
}

} // namespace sep::apps::cuda
