#pragma once

#include "engine/internal/cuda_api.hpp"
#ifdef __CUDACC__
#include <cuda_runtime.h>
#endif

namespace sep::quantum {

struct GPUContext {
    int device_id{0};
#ifdef __CUDACC__
    cudaStream_t default_stream{nullptr};
#else
    void* default_stream{nullptr};
#endif
    int block_size{256};  // Conservative default from device props
    bool initialized{false};

    GPUContext() = default;
    ~GPUContext() {
        if (initialized && default_stream) {
            // Cleanup handled by SEP layer
        }
    }

    // Prevent copying of CUDA resources
    GPUContext(const GPUContext&) = delete;
    GPUContext& operator=(const GPUContext&) = delete;
};

} // namespace sep::quantum
