#pragma once

#include "compat/cuda_runtime.h"
#include "compat/cuda_wrappers.h"

namespace sep::quantum {

struct GPUContext {
    int device_id{0};
    sep::cuda::cudaStream_t default_stream{nullptr};
    int block_size{256};  // Conservative default from device props
    bool initialized{false};

    GPUContext() = default;
    ~GPUContext() {
        if (initialized && default_stream) {
            sep::cuda::SEP_cudaStreamDestroy(default_stream);
        }
    }

    // Prevent copying of CUDA resources
    GPUContext(const GPUContext&) = delete;
    GPUContext& operator=(const GPUContext&) = delete;
};

} // namespace sep::quantum
