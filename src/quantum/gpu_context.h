#pragma once

#include "compat/cuda_unified.h"
#include "compat/cuda_fwd.h"

namespace sep::quantum {

struct GPUContext {
    int device_id{0};
    ::cudaStream_t default_stream{nullptr};  // Use global cuda namespace
    int block_size{256};  // Conservative default from device props
    bool initialized{false};

    GPUContext() = default;
    ~GPUContext() {
        if (initialized && default_stream) {
            ::cudaStreamDestroy(default_stream);  // Use global cuda namespace
        }
    }

    // Prevent copying of CUDA resources
    GPUContext(const GPUContext&) = delete;
    GPUContext& operator=(const GPUContext&) = delete;
};

} // namespace sep::quantum
