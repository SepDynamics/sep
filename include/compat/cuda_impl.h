#ifndef SEP_CUDA_IMPL_H
#define SEP_CUDA_IMPL_H
#pragma once

#include <stddef.h>  // For size_t
#include <stdio.h>   // For fprintf
#include <stdlib.h>  // For malloc/free
#include <string.h>  // For strcpy, memcpy, memset
#include "compat/cuda_defs.h"

#if SEP_CUDA_AVAILABLE
#include <cuda_runtime.h>
#include "compat/cuda_helpers.h"

#if SEP_CUDA_AVAILABLE
#define SEP_HD __host__ __device__
#else
#define SEP_HD

// CUDA function implementations when CUDA is not available
inline const char* cudaGetErrorString(cudaError_t error) {
    return (error == cudaSuccess) ? "success" : "CUDA not available";
}

inline cudaError_t cudaSetDevice(int device) {
    (void)device;
    return cudaSuccess;
}


// Additional CUDA function stubs for missing functions
inline cudaError_t cudaEventCreate(cudaEvent_t* event) {
#if SEP_CUDA_AVAILABLE
    return ::cudaEventCreate(event);
#else
    if (event) {
        *event = nullptr;
    }
    return cudaSuccess;
#endif
}

inline cudaError_t cudaEventDestroy(cudaEvent_t event) {
#if SEP_CUDA_AVAILABLE
    return ::cudaEventDestroy(event);
#else
    (void)event;
    return cudaSuccess;
#endif
}

inline cudaError_t cudaEventRecord(cudaEvent_t event, cudaStream_t stream) {
#if SEP_CUDA_AVAILABLE
    return ::cudaEventRecord(event, stream);
#else
    (void)event;
    (void)stream;
    return cudaSuccess;
#endif
}

inline cudaError_t cudaEventSynchronize(cudaEvent_t event) {
#if SEP_CUDA_AVAILABLE
    return ::cudaEventSynchronize(event);
#else
    (void)event;
    return cudaSuccess;
#endif
}

inline cudaError_t cudaEventElapsedTime(float* outMilliseconds, cudaEvent_t start, cudaEvent_t end) {
#if SEP_CUDA_AVAILABLE
    return ::cudaEventElapsedTime(outMilliseconds, start, end);
#else
    if (outMilliseconds) {
        *outMilliseconds = 0.0F;
    }
    (void)start;
    (void)end;
    return cudaSuccess;
#endif
}

inline cudaError_t cudaStreamCreate(cudaStream_t* stream) {
#if SEP_CUDA_AVAILABLE
    return ::cudaStreamCreate(stream);
#else
    if (stream) {
        *stream = nullptr;
    }
    return cudaSuccess;
#endif
}

inline cudaError_t cudaStreamCreateWithFlags(cudaStream_t* stream, unsigned int flags) {
#if SEP_CUDA_AVAILABLE
    return ::cudaStreamCreateWithFlags(stream, flags);
#else
    if (stream) {
        *stream = nullptr;
    }
    (void)flags; // Unused in stub implementation
    return cudaSuccess;
#endif
}

inline cudaError_t cudaStreamDestroy(cudaStream_t stream) {
#if SEP_CUDA_AVAILABLE
    return ::cudaStreamDestroy(stream);
#else
    (void)stream;
    return cudaSuccess;
#endif
}


// cudaStreamSynchronize_ptsz is already defined as a macro in cuda_runtime.h

// Simplified memory info struct to avoid parameter similarity issues
struct CudaMemoryInfo {
    size_t free;
    size_t total;
};

// Provide fake memory statistics for compatibility with code that queries
// available GPU memory - using a struct to avoid similar parameters
inline CudaMemoryInfo getCudaMemoryInfo() {
    return {512 * 1024 * 1024, 1024 * 1024 * 1024};
}

// Wrapper for the standard cudaMemGetInfo to avoid linter warnings
inline cudaError_t cudaMemGetInfo(size_t* free, size_t* total) {
#if SEP_CUDA_AVAILABLE
    return ::cudaMemGetInfo(free, total);
#else
    CudaMemoryInfo info = getCudaMemoryInfo();
    if (free)
        *free = info.free;
    if (total)
        *total = info.total;
    return cudaSuccess;
#endif
}
extern "C" {

// Memory management functions
cudaError_t cudaMemset(void* devPtr, int value, size_t count);
cudaError_t cudaMallocManaged(void** ptr, size_t size);

// Memory copy functions
cudaError_t cudaMemcpy(void* dst, const void* src, size_t count, cudaMemcpyKind kind);
cudaError_t cudaMemcpyAsync(void* dst, const void* src, size_t count, cudaMemcpyKind kind, cudaStream_t stream);

} // extern "C"

#endif 

// Synchronize a stream with a recorded event. The stub ignores both arguments
// and immediately returns success.
inline cudaError_t cudaStreamWaitEvent(cudaStream_t stream, cudaEvent_t event) {
#if SEP_CUDA_AVAILABLE
    return ::cudaStreamWaitEvent(stream, event, 0);
#else
    (void)stream;
    (void)event;
    return cudaSuccess;
#endif
}

// Add cudaStreamAttachMemAsync function
inline cudaError_t cudaStreamAttachMemAsync(cudaStream_t stream, void* devPtr, size_t length, unsigned int flags) {
#if SEP_CUDA_AVAILABLE
    return ::cudaStreamAttachMemAsync(stream, devPtr, length, flags);
#else
    (void)stream;
    (void)devPtr;
    (void)length;
    (void)flags;
    return cudaSuccess;
#endif
}

// Add cudaStreamAttachMemAsync_ptsz function
inline cudaError_t cudaStreamAttachMemAsync_ptsz(cudaStream_t stream, void* devPtr, size_t length, unsigned int flags) {
#if SEP_CUDA_AVAILABLE
    return ::cudaStreamAttachMemAsync_ptsz(stream, devPtr, length, flags);
#else
    (void)stream;
    (void)devPtr;
    (void)length;
    (void)flags;
    return cudaSuccess;
#endif
}

// Add cudaStreamSynchronize_ptsz function
inline cudaError_t cudaStreamSynchronize_ptsz(cudaStream_t stream) {
#if SEP_CUDA_AVAILABLE
    return ::cudaStreamSynchronize_ptsz(stream);
#else
    (void)stream;
    return cudaSuccess;
#endif
}

// Structure to hold memory copy parameters to avoid similar adjacent parameters
struct CudaMemcpyParams {
    void* destination;
    const void* source;
    size_t sizeInBytes;
    cudaMemcpyKind direction;
    cudaStream_t stream;
};

// Helper function for memory copies to avoid parameter similarity issues
inline cudaError_t performCudaMemcpyAsync(const CudaMemcpyParams& params) {
#if SEP_CUDA_AVAILABLE
    return ::cudaMemcpyAsync(params.destination, params.source, params.sizeInBytes, params.direction, params.stream);
#else
    if (params.destination && params.source) {
        memcpy(params.destination, params.source, params.sizeInBytes);
    }
    (void)params.direction;
    (void)params.stream;
    return cudaSuccess;
#endif
}

// Asynchronous memory copy stub using the helper function
inline cudaError_t cudaMemcpyAsync(void* dst, const void* src, size_t count, cudaMemcpyKind kind, cudaStream_t stream) {
    return performCudaMemcpyAsync({dst, src, count, kind, stream});
}

#endif // !SEP_CUDA_AVAILABLE
