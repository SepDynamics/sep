#ifndef SEP_COMPAT_CUDA_WRAPPERS_H
#define SEP_COMPAT_CUDA_WRAPPERS_H

// Include proper headers for CUDA types and functions
#include <stddef.h>  // For size_t

// Configuration macro - must be defined by build system
#ifndef SEP_ENGINE_HAS_CUDA
#define SEP_ENGINE_HAS_CUDA 0
#endif

// Include real CUDA headers when available
#if SEP_ENGINE_HAS_CUDA
#include <cuda_runtime.h>
#include <driver_types.h>
#endif

namespace sep {
namespace cuda {

#if SEP_ENGINE_HAS_CUDA
// When CUDA is available, use the actual CUDA types
using ::cudaError_t;
using ::cudaStream_t;
using ::cudaEvent_t;
using ::cudaMemcpyKind;

// Import key CUDA constants
using ::cudaSuccess;
using ::cudaMemcpyHostToHost;
using ::cudaMemcpyHostToDevice;
using ::cudaMemcpyDeviceToHost;
using ::cudaMemcpyDeviceToDevice;
using ::cudaMemcpyDefault;

// Wrapper functions that call real CUDA functions
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

// Type definitions
typedef int cudaError_t;
typedef void* cudaStream_t;
typedef void* cudaEvent_t;

// Define error constants
static const cudaError_t cudaSuccess = 0;
static const cudaError_t cudaErrorNotReady = 34;

// Define memory copy directions
enum cudaMemcpyKind
{
    cudaMemcpyHostToHost = 0,
    cudaMemcpyHostToDevice = 1,
    cudaMemcpyDeviceToHost = 2,
    cudaMemcpyDeviceToDevice = 3,
    cudaMemcpyDefault = 4
};

// Stub function implementations
inline cudaError_t cudaStreamDestroy(cudaStream_t) { 
    return cudaSuccess; 
}

inline cudaError_t cudaMemcpy(void*, const void*, size_t, cudaMemcpyKind) {
    return cudaSuccess;
}

inline cudaError_t cudaMemcpyAsync(void*, const void*, size_t, cudaMemcpyKind, cudaStream_t = 0) {
    return cudaSuccess;
}
#endif

}  // namespace cuda
}  // namespace sep

#endif // SEP_COMPAT_CUDA_WRAPPERS_H