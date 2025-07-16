#ifndef SEP_COMPAT_CUDA_WRAPPERS_H
#define SEP_COMPAT_CUDA_WRAPPERS_H

// Include proper headers for CUDA types and functions
#include <stddef.h>  // For size_t

#if SEP_ENGINE_HAS_CUDA
#include <cuda_runtime.h>
#include <driver_types.h>
#endif

namespace sep {
namespace cuda {

#if SEP_ENGINE_HAS_CUDA
// When CUDA is available, provide proper inline functions
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
// Stub implementations when CUDA is not available
enum cudaMemcpyKind
{
    cudaMemcpyHostToHost = 0,
    cudaMemcpyHostToDevice = 1,
    cudaMemcpyDeviceToHost = 2,
    cudaMemcpyDeviceToDevice = 3,
    cudaMemcpyDefault = 4
};

inline cudaError_t cudaStreamDestroy(cudaStream_t) { 
    return 0; // cudaSuccess 
}

inline cudaError_t cudaMemcpy(void*, const void*, size_t, cudaMemcpyKind) {
    return 0; // cudaSuccess
}

inline cudaError_t cudaMemcpyAsync(void*, const void*, size_t, cudaMemcpyKind, cudaStream_t = 0) {
    return 0; // cudaSuccess
}
#endif

}  // namespace cuda
}  // namespace sep

#endif // SEP_COMPAT_CUDA_WRAPPERS_H