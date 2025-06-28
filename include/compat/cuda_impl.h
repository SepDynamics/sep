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

// Report a single available device to satisfy callers that expect at least one
// GPU in the system.
inline cudaError_t cudaGetDeviceCount(int* count) {
#if SEP_CUDA_AVAILABLE
    return ::cudaGetDeviceCount(count);
#else
    if (count) {
        *count = 1;
    }
    return cudaSuccess;
#endif
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

inline cudaError_t cudaStreamSynchronize(cudaStream_t stream) {
#if SEP_CUDA_AVAILABLE
    return ::cudaStreamSynchronize(stream);
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

// Event management functions
cudaError_t cudaEventCreate(cudaEvent_t* event);
cudaError_t cudaEventDestroy(cudaEvent_t event);
cudaError_t cudaEventRecord(cudaEvent_t event, cudaStream_t stream);
cudaError_t cudaEventSynchronize(cudaEvent_t event);
cudaError_t cudaEventElapsedTime(float* ms, cudaEvent_t start, cudaEvent_t end);

// Stream management functions
cudaError_t cudaStreamCreate(cudaStream_t* stream);
cudaError_t cudaStreamDestroy(cudaStream_t stream);
cudaError_t cudaStreamSynchronize(cudaStream_t stream);

// Memory management functions
cudaError_t cudaMemset(void* devPtr, int value, size_t count);
cudaError_t cudaMallocManaged(void** ptr, size_t size);

// Device management functions
cudaError_t cudaGetLastError();
cudaError_t cudaGetDeviceCount(int* count);
cudaError_t cudaSetDevice(int device);
cudaError_t cudaGetDeviceProperties(cudaDeviceProp* prop, int device);

// Memory copy functions
cudaError_t cudaMemcpy(void* dst, const void* src, size_t count, cudaMemcpyKind kind);
cudaError_t cudaMemcpyAsync(void* dst, const void* src, size_t count, cudaMemcpyKind kind, cudaStream_t stream);

} // extern "C"

#endif // !SEP_CUDA_AVAILABLE
// Populate device properties with conservative values so callers relying on
// hardware characteristics can continue running without real GPU hardware.
inline cudaError_t cudaGetDeviceProperties(cudaDeviceProp *prop, int device) {
#if SEP_CUDA_AVAILABLE
    return ::cudaGetDeviceProperties(prop, device);
#else
    if (!prop) {
        return sep::cuda::cudaErrorInvalidValue;
    }
    // Only 1 stub device
    if (device < 0 || device >= 1) {
        return cudaErrorInvalidDevice;
    }
    
    if (prop) {
        strcpy(prop->name, "Stub GPU Device");
        prop->totalGlobalMem = 1024 * 1024 * 1024;  // 1GB
        prop->major = 3;
        prop->minor = 0;
        prop->multiProcessorCount = 8;
        prop->maxThreadsPerBlock = 1024;
        prop->warpSize = 32;
        prop->sharedMemPerBlock = 49152;
        prop->maxThreadsDim[0] = 1024;
        prop->maxThreadsDim[1] = 1024;
        prop->maxThreadsDim[2] = 64;
        prop->maxGridSize[0] = 65535;
        prop->maxGridSize[1] = 65535;
        prop->maxGridSize[2] = 65535;
        prop->totalConstMem = 65536;
        prop->clockRate = 1000000;
        prop->deviceOverlap = 1;
        prop->kernelExecTimeoutEnabled = 0;
        prop->integrated = 0;
        prop->canMapHostMemory = 1;
        prop->concurrentKernels = 1;
        prop->unifiedAddressing = 1;
        prop->maxThreadsPerMultiProcessor = 2048;
        
        // Note: We're not setting fields that don't exist in our simplified cudaDeviceProp
    }
    return cudaSuccess;
#endif
}
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
