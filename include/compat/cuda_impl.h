#ifndef SEP_CUDA_IMPL_H
#define SEP_CUDA_IMPL_H
#pragma once

// Added to ensure consistent macro definitions
#include <stddef.h>  // For size_t
#include <stdio.h>   // For fprintf
#include <stdlib.h>  // For malloc/free
#include <string.h>  // For strcpy, memcpy, memset

// Include our CUDA runtime header which handles both real CUDA and stub implementations
#include "compat/cuda_runtime.h"

#if SEP_CUDA_AVAILABLE
// When CUDA is available, include the real CUDA runtime
#include "/usr/local/cuda-12.9/include/cuda_runtime.h"
#define SEP_HD __host__ __device__
#else
#define SEP_HD
#endif

#if !SEP_CUDA_AVAILABLE
// Additional implementation details for CUDA stubs
// Use the cudaDeviceProp from cuda_runtime.h
using cudaDeviceProp = sep::cuda::cudaDeviceProp;

constexpr size_t STUB_GLOBAL_MEM = 1024 * 1024 * 1024;  // 1GB
constexpr int STUB_MAJOR_VERSION = 3;
constexpr int STUB_MINOR_VERSION = 0;
constexpr int STUB_MP_COUNT = 8;
constexpr int STUB_MAX_THREADS_PER_BLOCK = 1024;
constexpr int STUB_WARP_SIZE = 32;
constexpr size_t STUB_SHARED_MEM_PER_BLOCK = 49152;
constexpr int STUB_REGS_PER_BLOCK = 65536;
constexpr size_t STUB_MEM_PITCH = 2147483647;
constexpr int STUB_MAX_THREADS_DIM_0 = 1024;
constexpr int STUB_MAX_THREADS_DIM_1 = 1024;
constexpr int STUB_MAX_THREADS_DIM_2 = 64;
constexpr int STUB_MAX_GRID_SIZE_0 = 65535;
constexpr int STUB_MAX_GRID_SIZE_1 = 65535;
constexpr int STUB_MAX_GRID_SIZE_2 = 65535;
constexpr size_t STUB_TOTAL_CONST_MEM = 65536;
constexpr int STUB_CLOCK_RATE = 1000000;
constexpr size_t STUB_TEXTURE_ALIGNMENT = 512;
constexpr int STUB_DEVICE_OVERLAP = 1;
constexpr int STUB_KERNEL_EXEC_TIMEOUT_ENABLED = 0;
constexpr int STUB_INTEGRATED = 0;
constexpr int STUB_CAN_MAP_HOST_MEMORY = 1;
constexpr int STUB_COMPUTE_MODE = 0;
constexpr int STUB_MAX_TEXTURE_1D = 65536;
constexpr int STUB_MAX_TEXTURE_2D_0 = 65536;
constexpr int STUB_MAX_TEXTURE_2D_1 = 65536;
constexpr int STUB_MAX_TEXTURE_3D_0 = 4096;
constexpr int STUB_MAX_TEXTURE_3D_1 = 4096;
constexpr int STUB_MAX_TEXTURE_3D_2 = 4096;
constexpr int STUB_MAX_TEXTURE_1D_LAYERED_0 = 16384;
constexpr int STUB_MAX_TEXTURE_1D_LAYERED_1 = 2048;
constexpr int STUB_MAX_TEXTURE_2D_LAYERED_0 = 16384;
constexpr int STUB_MAX_TEXTURE_2D_LAYERED_1 = 16384;
constexpr int STUB_MAX_TEXTURE_2D_LAYERED_2 = 2048;
constexpr size_t STUB_SURFACE_ALIGNMENT = 512;
constexpr int STUB_CONCURRENT_KERNELS = 1;
constexpr int STUB_ECC_ENABLED = 0;
constexpr int STUB_PCI_BUS_ID = 0;
constexpr int STUB_PCI_DEVICE_ID = 0;
constexpr int STUB_PCI_DOMAIN_ID = 0;
constexpr int STUB_TCC_DRIVER = 0;
constexpr int STUB_ASYNC_ENGINE_COUNT = 2;
constexpr int STUB_UNIFIED_ADDRESSING = 1;
constexpr int STUB_MEMORY_CLOCK_RATE = 2000000;
constexpr int STUB_MEMORY_BUS_WIDTH = 256;
constexpr size_t STUB_L2_CACHE_SIZE = 1048576;
constexpr int STUB_MAX_THREADS_PER_MULTI_PROCESSOR = 2048;
// Return a human readable error message. Always indicates failure is due to the
// runtime stubs when CUDA is unavailable.
inline const char* cudaGetErrorString(cudaError_t error) {
#if SEP_CUDA_AVAILABLE
    return ::cudaGetErrorString(error);
#else
    return (error == cudaSuccess) ? "success" : "CUDA not available";
#endif
}
namespace cuda_stub_constants {
// Select a CUDA device. This stub ignores the requested device and simply
// reports success so that host builds can proceed without errors.
inline cudaError_t cudaSetDevice(int device) {
#if SEP_CUDA_AVAILABLE
    return ::cudaSetDevice(device);
#else
    (void)device;
    return cudaSuccess;
#endif
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
}  // namespace cuda_stub_constants

// Retrieve the last CUDA error. Since the stub never fails, this always
// returns `cudaSuccess`.
inline cudaError_t cudaGetLastError() {
#if SEP_CUDA_AVAILABLE
    return ::cudaGetLastError();
#else
    return cudaSuccess;
#endif
}

// Wrapper function to avoid similar parameter types
inline cudaError_t cudaFillMemory(void* devicePtr, size_t numBytes, int value) {
    if (!devicePtr) {
        return cudaErrorInvalidValue;
    }
    memset(devicePtr, value, numBytes);
    return sep::cuda::cudaSuccess;
}

// Initialize a memory region with the given value - wrapper around memset
inline cudaError_t cudaMemset(void* devPtr, int value, size_t count) {
#if SEP_CUDA_AVAILABLE
    return ::cudaMemset(devPtr, value, count);
#else
    return cudaFillMemory(devPtr, count, value);
#endif
}

// Allocate managed memory using host malloc when CUDA is unavailable
inline cudaError_t cudaMallocManaged(void** ptr, size_t size) {
#if SEP_CUDA_AVAILABLE
    return ::cudaMallocManaged(ptr, size);
#else
    if (!ptr) {
        return sep::cuda::cudaErrorInvalidValue;
    }
    *ptr = malloc(size);
    return (*ptr) ? sep::cuda::cudaSuccess : sep::cuda::cudaErrorMemoryAllocation;
#endif
}
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
// End of the implementation section
#endif  // !SEP_CUDA_AVAILABLE

#endif  // SEP_CUDA_IMPL_H
