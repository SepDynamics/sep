#ifndef SEP_COMPAT_CUDA_FWD_H
#define SEP_COMPAT_CUDA_FWD_H

// Forward declarations of CUDA types to break circular dependencies
// This header should be included first in both cuda_runtime.h and cuda_types.h

// Configuration macro if not already defined
#ifndef SEP_ENGINE_HAS_CUDA
#define SEP_ENGINE_HAS_CUDA 1
#endif

// Only define types if CUDA runtime is not available
#if !SEP_ENGINE_HAS_CUDA

// Global namespace forward declarations of CUDA types for non-CUDA builds
typedef int cudaError_t;
typedef void* cudaStream_t;
typedef void* cudaEvent_t;
struct cudaDeviceProp;
typedef int cufftHandle;

// Define cudaMemcpyKind enum for non-CUDA builds
enum cudaMemcpyKind
{
    cudaMemcpyHostToHost = 0,
    cudaMemcpyHostToDevice = 1,
    cudaMemcpyDeviceToHost = 2,
    cudaMemcpyDeviceToDevice = 3,
    cudaMemcpyDefault = 4
};

// Define error constants as enum values to avoid macro conflicts
enum {
    SEP_CUDA_SUCCESS = 0,
    SEP_CUDA_ERROR_MEMORY_ALLOCATION = 2
};

#else
// When CUDA is available, include the actual CUDA headers
#include <driver_types.h>
#include <cuda_runtime_api.h>

// Use CUDA's actual values
#define SEP_CUDA_SUCCESS cudaSuccess
#define SEP_CUDA_ERROR_MEMORY_ALLOCATION cudaErrorMemoryAllocation
#endif

// Define stream flags that were missing
#ifndef SEP_cudaStreamNonBlocking
#define SEP_cudaStreamNonBlocking 0x01
#endif

#ifndef SEP_cudaStreamDefault
#define SEP_cudaStreamDefault 0x00
#endif

#endif // SEP_COMPAT_CUDA_FWD_H