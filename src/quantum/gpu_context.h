#pragma once

#ifdef __CUDACC__
#include <cuda_runtime.h>
#else
// Forward declarations when not compiling with CUDA
typedef struct CUstream_st* cudaStream_t;
#endif

namespace sep::quantum {

struct GPUContext {
    int device_id{0};
    cudaStream_t default_stream{nullptr};
    int block_size{256};  // Conservative default from device props
    bool initialized{false};

    GPUContext() = default;
    ~GPUContext() {
#ifdef __CUDACC__
        if (initialized && default_stream) {
            cudaStreamDestroy(default_stream);
        }
#endif
    }

    // Prevent copying of CUDA resources
    GPUContext(const GPUContext&) = delete;
    GPUContext& operator=(const GPUContext&) = delete;
};

} // namespace sep::quantum
