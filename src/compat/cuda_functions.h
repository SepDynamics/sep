#ifndef SEP_COMPAT_CUDA_FUNCTIONS_H
#define SEP_COMPAT_CUDA_FUNCTIONS_H

#include <cstddef>  // For size_t
#include <cstring>  // For std::memcpy and std::memset

#include "compat/cuda_types.h"
#include "compat/cuda_constants.h"

namespace sep {
namespace cuda {

// Function declarations - these are implemented either by calling real CUDA
// functions or as stubs, depending on SEP_ENGINE_HAS_CUDA

// Device management
cudaError_t cudaSetDevice(int device);
cudaError_t cudaGetDevice(int* device);
cudaError_t cudaGetDeviceCount(int* count);
cudaError_t cudaDeviceSynchronize();
cudaError_t cudaDeviceReset();

// Error handling
cudaError_t cudaGetLastError();
const char* cudaGetErrorString(cudaError_t error);

// Stream operations
cudaError_t cudaStreamCreate(cudaStream_t* stream);
cudaError_t cudaStreamCreateWithFlags(cudaStream_t* stream, unsigned int flags);
cudaError_t cudaStreamDestroy(cudaStream_t stream);
cudaError_t cudaStreamSynchronize(cudaStream_t stream);
cudaError_t cudaStreamWaitEvent(cudaStream_t stream, cudaEvent_t event, unsigned int flags);

// Event operations
cudaError_t cudaEventCreate(cudaEvent_t* event);
cudaError_t cudaEventCreateWithFlags(cudaEvent_t* event, unsigned int flags);
cudaError_t cudaEventDestroy(cudaEvent_t event);
cudaError_t cudaEventRecord(cudaEvent_t event, cudaStream_t stream);
cudaError_t cudaEventSynchronize(cudaEvent_t event);
cudaError_t cudaEventElapsedTime(float* ms, cudaEvent_t start, cudaEvent_t end);

// Memory operations
cudaError_t cudaMalloc(void** ptr, size_t size);
cudaError_t cudaFree(void* ptr);
cudaError_t cudaMallocHost(void** ptr, size_t size);
cudaError_t cudaFreeHost(void* ptr);
cudaError_t cudaMemcpy(void* dst, const void* src, size_t count, cudaMemcpyKind kind);
cudaError_t cudaMemcpyAsync(void* dst, const void* src, size_t count, cudaMemcpyKind kind, cudaStream_t stream = 0);
cudaError_t cudaMemset(void* devPtr, int value, size_t count);
cudaError_t cudaMemsetAsync(void* devPtr, int value, size_t count, cudaStream_t stream);
cudaError_t cudaMemGetInfo(size_t* free, size_t* total);

// Inline implementation of simple wrapper functions
#if SEP_ENGINE_HAS_CUDA
// When CUDA is available, these inline functions directly call the real CUDA functions

inline cudaError_t cudaStreamDestroy(cudaStream_t stream) {
    return ::cudaStreamDestroy(stream);
}

inline cudaError_t cudaMemcpy(void* dst, const void* src, size_t count, cudaMemcpyKind kind) {
    return ::cudaMemcpy(dst, src, count, kind);
}

inline cudaError_t cudaMemcpyAsync(void* dst, const void* src, size_t count, cudaMemcpyKind kind, cudaStream_t stream) {
    return ::cudaMemcpyAsync(dst, src, count, kind, stream);
}

inline cudaError_t cudaMemset(void* devPtr, int value, size_t count) {
    return ::cudaMemset(devPtr, value, count);
}

#else
// When CUDA is not available, these are stub implementations that do nothing

inline cudaError_t cudaStreamDestroy(cudaStream_t) {
    return cudaSuccess;
}

inline cudaError_t cudaMemcpy(void* dst, const void* src, size_t count, cudaMemcpyKind) {
    if (dst && src && count > 0) {
        std::memcpy(dst, src, count);
    }
    return cudaSuccess;
}

inline cudaError_t cudaMemcpyAsync(void* dst, const void* src, size_t count, cudaMemcpyKind, cudaStream_t) {
    if (dst && src && count > 0) {
        std::memcpy(dst, src, count);
    }
    return cudaSuccess;
}

inline cudaError_t cudaMemset(void* devPtr, int value, size_t count) {
    if (devPtr && count > 0) {
        std::memset(devPtr, value, count);
    }
    return cudaSuccess;
}

#endif

} // namespace cuda
} // namespace sep

#endif // SEP_COMPAT_CUDA_FUNCTIONS_H