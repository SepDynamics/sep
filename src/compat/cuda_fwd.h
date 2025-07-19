#ifndef SEP_COMPAT_CUDA_FWD_H
#define SEP_COMPAT_CUDA_FWD_H

// Forward declarations of CUDA types to break circular dependencies
// This header should be included first in both cuda_runtime.h and cuda_types.h

// Configuration macro - respect SEP_USE_CUDA from CMake
#ifndef SEP_ENGINE_HAS_CUDA
  #ifdef SEP_USE_CUDA
    #define SEP_ENGINE_HAS_CUDA SEP_USE_CUDA
  #else
    #define SEP_ENGINE_HAS_CUDA 0
  #endif
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
#if __has_include(<cuda_runtime.h>)
    #include <cuda_runtime.h>
    #include <cuda_runtime_api.h>
#else
    // Fallback if CUDA headers not found in system paths
    #include "/usr/local/cuda/include/cuda_runtime.h"
    #include "/usr/local/cuda/include/cuda_runtime_api.h"
#endif

// Use CUDA's actual values
#ifndef SEP_CUDA_SUCCESS
#define SEP_CUDA_SUCCESS cudaSuccess
#endif
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