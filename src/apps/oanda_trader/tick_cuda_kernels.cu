#include "tick_cuda_kernels.cuh"
#include <cuda_runtime.h>
#include <device_launch_parameters.h>
#include <cub/cub.cuh>
#include <iostream>

namespace sep::apps::cuda {

// CUDA kernel for calculating rolling window statistics
__global__ void calculateRollingWindowsKernel(
    const TickData* ticks,
    size_t tick_count,
    const WindowSpec* window_specs,
    size_t window_count,
    WindowResult* results,
    uint64_t* window_timestamps) {
    
    int window_idx = blockIdx.x * blockDim.x + threadIdx.x;
    
    if (window_idx >= window_count) return;
    
    const WindowSpec& spec = window_specs[window_idx];
    WindowResult& result = results[window_idx];
    
    // Initialize result
    result.mean_price = 0.0;
    result.volatility = 0.0;
    result.price_change = 0.0;
    result.pip_change = 0.0;
    result.tick_count = 0;
    result.window_start = spec.start_time;
    result.window_end = spec.end_time;
    
    // Find ticks within this window
    double price_sum = 0.0;
    double first_price = 0.0;
    double last_price = 0.0;
    bool first_set = false;
    size_t valid_ticks = 0;
    
    for (size_t i = 0; i < tick_count; ++i) {
        const TickData& tick = ticks[i];
        
        if (tick.timestamp >= spec.start_time && tick.timestamp <= spec.end_time) {
            price_sum += tick.price;
            valid_ticks++;
            
            if (!first_set) {
                first_price = tick.price;
                first_set = true;
            }
            last_price = tick.price;
        }
    }
    
    result.tick_count = valid_ticks;
    
    if (valid_ticks > 0) {
        result.mean_price = price_sum / valid_ticks;
        
        // Calculate volatility (standard deviation)
        double variance_sum = 0.0;
        for (size_t i = 0; i < tick_count; ++i) {
            const TickData& tick = ticks[i];
            
            if (tick.timestamp >= spec.start_time && tick.timestamp <= spec.end_time) {
                double diff = tick.price - result.mean_price;
                variance_sum += diff * diff;
            }
        }
        
        result.volatility = sqrt(variance_sum / valid_ticks);
        result.price_change = last_price - first_price;
        result.pip_change = result.price_change * 10000.0; // Convert to pips
    }
}

// Optimized kernel using shared memory for better performance
__global__ void calculateRollingWindowsOptimized(
    const TickData* ticks,
    size_t tick_count,
    const WindowSpec* window_specs,
    size_t window_count,
    WindowResult* results,
    uint64_t current_time,
    uint64_t hourly_window_ns,
    uint64_t daily_window_ns) {
    
    __shared__ double shared_prices[CUDA_BLOCK_SIZE];
    __shared__ uint64_t shared_timestamps[CUDA_BLOCK_SIZE];
    
    int window_idx = blockIdx.x * blockDim.x + threadIdx.x;
    int thread_id = threadIdx.x;
    
    if (window_idx >= window_count) return;
    
    // Determine window type and calculate bounds
    bool is_hourly = (window_idx % 2 == 0);
    uint64_t window_start = current_time - (is_hourly ? hourly_window_ns : daily_window_ns);
    uint64_t window_end = current_time;
    
    WindowResult& result = results[window_idx];
    result.window_start = window_start;
    result.window_end = window_end;
    result.tick_count = 0;
    result.mean_price = 0.0;
    result.volatility = 0.0;
    
    // Process ticks in chunks using shared memory
    double price_sum = 0.0;
    double first_price = 0.0;
    double last_price = 0.0;
    bool first_set = false;
    size_t valid_ticks = 0;
    
    for (size_t chunk_start = 0; chunk_start < tick_count; chunk_start += CUDA_BLOCK_SIZE) {
        // Load chunk into shared memory
        size_t load_idx = chunk_start + thread_id;
        if (load_idx < tick_count) {
            shared_prices[thread_id] = ticks[load_idx].price;
            shared_timestamps[thread_id] = ticks[load_idx].timestamp;
        } else {
            shared_timestamps[thread_id] = 0; // Invalid timestamp
        }
        
        __syncthreads();
        
        // Process shared memory data (only thread 0 for each window)
        if (thread_id == 0) {
            for (int i = 0; i < CUDA_BLOCK_SIZE && (chunk_start + i) < tick_count; ++i) {
                if (shared_timestamps[i] >= window_start && shared_timestamps[i] <= window_end) {
                    price_sum += shared_prices[i];
                    valid_ticks++;
                    
                    if (!first_set) {
                        first_price = shared_prices[i];
                        first_set = true;
                    }
                    last_price = shared_prices[i];
                }
            }
        }
        
        __syncthreads();
    }
    
    // Finalize results (only thread 0)
    if (thread_id == 0) {
        result.tick_count = valid_ticks;
        
        if (valid_ticks > 0) {
            result.mean_price = price_sum / valid_ticks;
            result.price_change = last_price - first_price;
            result.pip_change = result.price_change * 10000.0;
            
            // Calculate volatility in second pass
            double variance_sum = 0.0;
            for (size_t chunk_start = 0; chunk_start < tick_count; chunk_start += CUDA_BLOCK_SIZE) {
                for (size_t i = chunk_start; i < min(chunk_start + CUDA_BLOCK_SIZE, tick_count); ++i) {
                    if (ticks[i].timestamp >= window_start && ticks[i].timestamp <= window_end) {
                        double diff = ticks[i].price - result.mean_price;
                        variance_sum += diff * diff;
                    }
                }
            }
            result.volatility = sqrt(variance_sum / valid_ticks);
        }
    }
}

// Multi-timeframe parallel processing kernel
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
    uint64_t daily_window_ns) {
    
    int idx = blockIdx.x * blockDim.x + threadIdx.x;
    
    if (idx >= result_count) return;
    
    // Calculate hourly window
    uint64_t hourly_end = base_time - (idx * hourly_step_ns);
    uint64_t hourly_start = hourly_end - hourly_window_ns;
    
    WindowResult& hourly_result = hourly_results[idx];
    calculateWindowStats(ticks, tick_count, hourly_start, hourly_end, hourly_result);
    
    // Calculate daily window
    uint64_t daily_end = base_time - (idx * daily_step_ns);
    uint64_t daily_start = daily_end - daily_window_ns;
    
    WindowResult& daily_result = daily_results[idx];
    calculateWindowStats(ticks, tick_count, daily_start, daily_end, daily_result);
}

__device__ void calculateWindowStats(
    const TickData* ticks,
    size_t tick_count,
    uint64_t window_start,
    uint64_t window_end,
    WindowResult& result) {
    
    result.window_start = window_start;
    result.window_end = window_end;
    result.tick_count = 0;
    
    double price_sum = 0.0;
    double first_price = 0.0;
    double last_price = 0.0;
    bool first_set = false;
    size_t valid_ticks = 0;
    
    // First pass: count, sum, find first/last
    for (size_t i = 0; i < tick_count; ++i) {
        const TickData& tick = ticks[i];
        
        if (tick.timestamp >= window_start && tick.timestamp <= window_end) {
            price_sum += tick.price;
            valid_ticks++;
            
            if (!first_set) {
                first_price = tick.price;
                first_set = true;
            }
            last_price = tick.price;
        }
    }
    
    result.tick_count = valid_ticks;
    
    if (valid_ticks > 0) {
        result.mean_price = price_sum / valid_ticks;
        result.price_change = last_price - first_price;
        result.pip_change = result.price_change * 10000.0;
        
        // Second pass: calculate variance
        double variance_sum = 0.0;
        for (size_t i = 0; i < tick_count; ++i) {
            const TickData& tick = ticks[i];
            
            if (tick.timestamp >= window_start && tick.timestamp <= window_end) {
                double diff = tick.price - result.mean_price;
                variance_sum += diff * diff;
            }
        }
        
        result.volatility = sqrt(variance_sum / valid_ticks);
    } else {
        result.mean_price = 0.0;
        result.volatility = 0.0;
        result.price_change = 0.0;
        result.pip_change = 0.0;
    }
}

// Host function implementations
cudaError_t initializeCudaDevice(CudaContext& context, int device_id) {
    cudaError_t error = cudaSuccess;

    error = cudaSetDevice(device_id);
    if (error != cudaSuccess) {
        std::cerr << "[CUDA] Error setting device: " << cudaGetErrorString(error) << std::endl;
        return error;
    }
    
    // Get device properties
    error = cudaGetDeviceProperties(&context.device_props, device_id);
    if (error != cudaSuccess) {
        std::cerr << "[CUDA] Error getting device properties: " << cudaGetErrorString(error) << std::endl;
        return error;
    }
    
    std::cout << "[CUDA] Device: " << context.device_props.name << std::endl;
    std::cout << "[CUDA] Compute Capability: " << context.device_props.major << "." << context.device_props.minor << std::endl;
    std::cout << "[CUDA] Global Memory: " << context.device_props.totalGlobalMem / (1024*1024) << " MB" << std::endl;
    std::cout << "[CUDA] Max Threads per Block: " << context.device_props.maxThreadsPerBlock << std::endl;
    
    // Allocate device memory
    size_t max_ticks = MAX_TICK_HISTORY;
    size_t max_windows = MAX_CALCULATION_WINDOWS;
    
    error = cudaMalloc(&context.d_ticks, max_ticks * sizeof(TickData));
    if (error != cudaSuccess) {
        std::cerr << "[CUDA] Error allocating tick data: " << cudaGetErrorString(error) << std::endl;
        return error;
    }
    
    error = cudaMalloc(&context.d_hourly_results, max_windows * sizeof(WindowResult));
    if (error != cudaSuccess) {
        std::cerr << "[CUDA] Error allocating hourly results: " << cudaGetErrorString(error) << std::endl;
        return error;
    }
    
    error = cudaMalloc(&context.d_daily_results, max_windows * sizeof(WindowResult));
    if (error != cudaSuccess) {
        std::cerr << "[CUDA] Error allocating daily results: " << cudaGetErrorString(error) << std::endl;
        return error;
    }
    
    error = cudaMalloc(&context.d_window_specs, max_windows * sizeof(WindowSpec));
    if (error != cudaSuccess) {
        std::cerr << "[CUDA] Error allocating window specs: " << cudaGetErrorString(error) << std::endl;
        return error;
    }
    
    // Create CUDA streams for async operations
    error = cudaStreamCreate(&context.stream);
    if (error != cudaSuccess) {
        std::cerr << "[CUDA] Error creating stream: " << cudaGetErrorString(error) << std::endl;
        return error;
    }
    
    context.initialized = true;
    std::cout << "[CUDA] Initialization complete!" << std::endl;
    
    return cudaSuccess;
}

cudaError_t cleanupCudaDevice(CudaContext& context) {
    cudaError_t error = cudaSuccess;
    
    if (context.d_ticks) {
        cudaFree(context.d_ticks);
        context.d_ticks = nullptr;
    }
    
    if (context.d_hourly_results) {
        cudaFree(context.d_hourly_results);
        context.d_hourly_results = nullptr;
    }
    
    if (context.d_daily_results) {
        cudaFree(context.d_daily_results);
        context.d_daily_results = nullptr;
    }
    
    if (context.d_window_specs) {
        cudaFree(context.d_window_specs);
        context.d_window_specs = nullptr;
    }
    
    if (context.stream) {
        cudaStreamDestroy(context.stream);
        context.stream = nullptr;
    }
    
    context.initialized = false;
    return error;
}

cudaError_t calculateWindowsCuda(
    CudaContext& context,
    const std::vector<TickData>& host_ticks,
    std::vector<WindowResult>& hourly_results,
    std::vector<WindowResult>& daily_results,
    uint64_t current_time,
    uint64_t hourly_window_ns,
    uint64_t daily_window_ns) {
    
    if (!context.initialized) {
        return cudaErrorNotReady;
    }
    
    cudaError_t error = cudaSuccess;
    
    // Copy tick data to device
    size_t tick_count = host_ticks.size();
    if (tick_count > MAX_TICK_HISTORY) {
        tick_count = MAX_TICK_HISTORY;
    }
    
    error = cudaMemcpyAsync(context.d_ticks, host_ticks.data(), 
                           tick_count * sizeof(TickData), 
                           cudaMemcpyHostToDevice, context.stream);
    if (error != cudaSuccess) {
        std::cerr << "[CUDA] Error copying ticks to device: " << cudaGetErrorString(error) << std::endl;
        return error;
    }
    
    // Calculate grid and block dimensions
    size_t result_count = std::min(hourly_results.size(), daily_results.size());
    int block_size = CUDA_BLOCK_SIZE;
    int grid_size = (result_count + block_size - 1) / block_size;
    
    // Launch multi-timeframe kernel
    calculateMultiTimeframeWindows<<<grid_size, block_size, 0, context.stream>>>(
        context.d_ticks,
        tick_count,
        context.d_hourly_results,
        context.d_daily_results,
        result_count,
        current_time,
        HOURLY_STEP_NS,
        DAILY_STEP_NS,
        hourly_window_ns,
        daily_window_ns
    );
    
    error = cudaGetLastError();
    if (error != cudaSuccess) {
        std::cerr << "[CUDA] Kernel launch error: " << cudaGetErrorString(error) << std::endl;
        return error;
    }
    
    // Copy results back to host
    error = cudaMemcpyAsync(hourly_results.data(), context.d_hourly_results,
                           result_count * sizeof(WindowResult),
                           cudaMemcpyDeviceToHost, context.stream);
    if (error != cudaSuccess) {
        std::cerr << "[CUDA] Error copying hourly results: " << cudaGetErrorString(error) << std::endl;
        return error;
    }
    
    error = cudaMemcpyAsync(daily_results.data(), context.d_daily_results,
                           result_count * sizeof(WindowResult),
                           cudaMemcpyDeviceToHost, context.stream);
    if (error != cudaSuccess) {
        std::cerr << "[CUDA] Error copying daily results: " << cudaGetErrorString(error) << std::endl;
        return error;
    }
    
    // Synchronize stream
    error = cudaStreamSynchronize(context.stream);
    if (error != cudaSuccess) {
        std::cerr << "[CUDA] Stream sync error: " << cudaGetErrorString(error) << std::endl;
        return error;
    }
    
    return cudaSuccess;
}

cudaError_t calculateForwardWindowsCuda(
    CudaContext& context,
    const std::vector<TickData>& ticks,
    std::vector<ForwardWindowResult>& results,
    uint64_t window_size_ns) {
    
    if (!context.initialized) {
        return cudaErrorNotReady;
    }
    
    // Clear and resize results based on tick data windows
    results.clear();
    size_t num_windows = std::max(1UL, ticks.size() / 100); // Create windows from tick data
    results.resize(num_windows);
    
    // Calculate forward window metrics based on bitspace math
    for (size_t i = 0; i < num_windows && i < ticks.size(); ++i) {
        ForwardWindowResult& result = results[i];
        
        // Calculate coherence - measure of pattern consistency
        double price_variance = 0.0;
        double mean_price = 0.0;
        size_t window_end = std::min(i + 100, ticks.size());
        size_t window_size = window_end - i;
        
        if (window_size > 1) {
            // Calculate mean price in window
            for (size_t j = i; j < window_end; ++j) {
                mean_price += ticks[j].price;
            }
            mean_price /= window_size;
            
            // Calculate variance for coherence
            for (size_t j = i; j < window_end; ++j) {
                double diff = ticks[j].price - mean_price;
                price_variance += diff * diff;
            }
            price_variance /= window_size;
            
            // Coherence: inverse of normalized variance (higher coherence = lower variance)
            result.coherence = 1.0f / (1.0f + static_cast<float>(price_variance * 10000.0));
            
            // Stability: based on price change consistency
            double total_change = 0.0;
            int direction_changes = 0;
            for (size_t j = i + 1; j < window_end; ++j) {
                double change = ticks[j].price - ticks[j-1].price;
                total_change += std::abs(change);
                if (j > i + 1) {
                    double prev_change = ticks[j-1].price - ticks[j-2].price;
                    if ((change > 0 && prev_change < 0) || (change < 0 && prev_change > 0)) {
                        direction_changes++;
                    }
                }
            }
            result.stability = 1.0f - (static_cast<float>(direction_changes) / static_cast<float>(window_size - 1));
            
            // Entropy: measure of randomness in price movements
            result.entropy = static_cast<float>(total_change / (window_size * mean_price));
            
            // Confidence: based on trajectory similarity (simplified)
            result.confidence = result.coherence * result.stability * (1.0f - result.entropy);
            
            // Count ruptures and flips based on price movements
            result.rupture_count = 0;
            result.flip_count = direction_changes;
            
            // Check for significant price ruptures (>2% moves)
            for (size_t j = i + 1; j < window_end; ++j) {
                double price_change = std::abs((ticks[j].price - ticks[j-1].price) / ticks[j-1].price);
                if (price_change > 0.02) { // 2% threshold
                    result.rupture_count++;
                }
            }
        } else {
            // Default values for insufficient data
            result.coherence = 0.5f;
            result.stability = 0.5f;
            result.entropy = 0.1f;
            result.confidence = 0.25f;
            result.rupture_count = 0;
            result.flip_count = 0;
        }
    }
    
    return cudaSuccess;
}

} // namespace sep::apps::cuda
