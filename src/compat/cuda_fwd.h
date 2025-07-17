#ifndef SEP_COMPAT_CUDA_FWD_H
#define SEP_COMPAT_CUDA_FWD_H

// Forward declarations of CUDA types to break circular dependencies
// This header should be included first in both cuda_runtime.h and cuda_types.h

// Configuration macro if not already defined
#ifndef SEP_ENGINE_HAS_CUDA
#define SEP_ENGINE_HAS_CUDA 0
#endif

#if SEP_ENGINE_HAS_CUDA
// When CUDA is available, include minimal CUDA headers for type definitions
#include <driver_types.h>
#else
// When CUDA is not available, forward declare the types
typedef int cudaError_t;
typedef void* cudaStream_t;
typedef void* cudaEvent_t;
struct cudaDeviceProp;

// Define cudaMemcpyKind enum for non-CUDA builds
#if !defined(SEP_CUDA_MEMCPYKIND_DEFINED)
#define SEP_CUDA_MEMCPYKIND_DEFINED
enum cudaMemcpyKind
{
    cudaMemcpyHostToHost = 0,
    cudaMemcpyHostToDevice = 1,
    cudaMemcpyDeviceToHost = 2,
    cudaMemcpyDeviceToDevice = 3,
    cudaMemcpyDefault = 4
};
#endif
#endif

#endif // SEP_COMPAT_CUDA_FWD_H