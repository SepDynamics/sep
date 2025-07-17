#ifndef SEP_COMPAT_CUDA_WRAPPERS_H
#define SEP_COMPAT_CUDA_WRAPPERS_H

// Include proper headers for CUDA types and functions
#include <stddef.h>  // For size_t
#include <cstring>  // For std::memcpy
#include "compat/cuda_types.h"  // Include our centralized CUDA type definitions

namespace sep {
namespace cuda {

// Wrapper functions for CUDA operations that provide consistent behavior regardless of CUDA availability
#if SEP_ENGINE_HAS_CUDA
// When CUDA is available, these wrapper functions call the real CUDA functions

// Memory operations wrappers
inline cudaError_t cudaStreamDestroy(cudaStream_t stream) {
    return ::cudaStreamDestroy(stream);
}

inline cudaError_t cudaMemcpy(void* dst, const void* src, size_t count,
                             cudaMemcpyKind kind) {
    return ::cudaMemcpy(dst, src, count, kind);
}

inline cudaError_t cudaMemcpyAsync(void* dst, const void* src, size_t count,
                                  cudaMemcpyKind kind, cudaStream_t stream = 0) {
    return ::cudaMemcpyAsync(dst, src, count, kind, stream);
}

#else
// Stub function implementations when CUDA is not available

// Define additional error constants needed by wrappers
static const cudaError_t cudaErrorNotReady = 34;

// Stub function implementations
inline cudaError_t cudaStreamDestroy(cudaStream_t) {
    return cudaSuccess;
}

inline cudaError_t cudaMemcpy(void* dst, const void* src, size_t count, cudaMemcpyKind) {
    // For non-CUDA builds, implement a basic memcpy for host memory
    if (dst && src && count > 0) {
        std::memcpy(dst, src, count);
    }
    return cudaSuccess;
}

inline cudaError_t cudaMemcpyAsync(void* dst, const void* src, size_t count,
                                  cudaMemcpyKind, cudaStream_t = 0) {
    // For non-CUDA builds, async just calls regular memcpy
    if (dst && src && count > 0) {
        std::memcpy(dst, src, count);
    }
    return cudaSuccess;
}
#endif

}  // namespace cuda
}  // namespace sep

#endif // SEP_COMPAT_CUDA_WRAPPERS_H