/*
 * Copyright (c) 2025 SEP Engine Contributors
 *
 * Custom wrapper for HIP runtime to avoid conflicts with CUDA types
 */

#ifndef HIP_RUNTIME_WRAPPER_H
#define HIP_RUNTIME_WRAPPER_H

#include <cuda_runtime.h>

// Define HIP types for external use
typedef void* hipStream_t;
typedef void* hipEvent_t;
typedef sep::cuda::cudaError_t hipError_t;

// Define HIP types as CUDA types in the sep::cuda namespace
namespace sep {
namespace cuda {

// Stream functions
cudaError_t cudaStreamCreateWithFlags(void** stream, unsigned int flags);
cudaError_t cudaStreamDestroy(void* stream);
cudaError_t cudaStreamSynchronize(void* stream);
cudaError_t cudaStreamWaitEvent(void* stream, void* event, unsigned int flags);
cudaError_t cudaStreamAttachMemAsync(void* stream, void* devPtr, size_t length, unsigned int flags);

// Event functions
cudaError_t cudaEventCreate(void** event);
cudaError_t cudaEventDestroy(void* event);
cudaError_t cudaEventSynchronize(void* event);
cudaError_t cudaEventRecord(void* event, void* stream);

// Memory management functions
cudaError_t cudaMalloc(void** devPtr, size_t size);
cudaError_t cudaFree(void* devPtr);
cudaError_t cudaMallocManaged(void** devPtr, size_t size, unsigned int flags);

// Error handling
const char* cudaGetErrorString(int error);

// Define HIP constants
constexpr unsigned int hipHostMallocDefault = 0;
constexpr unsigned int hipHostMallocMapped = 1;
constexpr unsigned int hipHostMallocWriteCombined = 2;

} // namespace cuda
} // namespace sep

// Forward declarations for HIP functions
extern "C" {
    hipError_t hipGetDeviceProperties(sep::cuda::cudaDeviceProp* prop, int device);
    hipError_t hipGetDevicePropertiesR0600(sep::cuda::cudaDeviceProp* prop, int device);
    hipError_t hipHostMalloc(void** ptr, size_t size, unsigned int flags);
    
    // Additional HIP function declarations needed by hip_compat.cpp
    hipError_t hipStreamCreateWithFlags(hipStream_t* stream, unsigned int flags);
    hipError_t hipStreamDestroy(hipStream_t stream);
    hipError_t hipStreamSynchronize(hipStream_t stream);
    hipError_t hipStreamWaitEvent(hipStream_t stream, hipEvent_t event, unsigned int flags);
    hipError_t hipEventCreate(hipEvent_t* event);
    hipError_t hipEventDestroy(hipEvent_t event);
    hipError_t hipEventSynchronize(hipEvent_t event);
    hipError_t hipEventRecord(hipEvent_t event, hipStream_t stream);
    hipError_t hipMalloc(void** devPtr, size_t size);
    hipError_t hipFree(void* devPtr);
    const char* hipGetErrorString(hipError_t error);
}

// Define HIP constants
#define hipSuccess sep::cuda::cudaSuccess
#define hipErrorNotReady sep::cuda::cudaErrorNotReady

#endif // HIP_RUNTIME_WRAPPER_H