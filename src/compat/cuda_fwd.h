#ifndef SEP_COMPAT_CUDA_FWD_H
#define SEP_COMPAT_CUDA_FWD_H

// Forward declarations of CUDA types to break circular dependencies
// This header should be included first in both cuda_runtime.h and cuda_types.h

// Configuration macro if not already defined
#ifndef SEP_ENGINE_HAS_CUDA
#define SEP_ENGINE_HAS_CUDA 1
#endif

// Global namespace forward declarations of CUDA types
// These ensure CUDA types are properly defined in the global namespace
// regardless of whether we're using the actual CUDA runtime
#if !defined(SEP_CUDA_TYPES_FORWARD_DECLARED)
#define SEP_CUDA_TYPES_FORWARD_DECLARED
typedef int cudaError_t;
typedef void* cudaStream_t;
typedef void* cudaEvent_t;
struct cudaDeviceProp;

// Add cufftHandle declaration to avoid incomplete type issues
typedef int cufftHandle;

// Define constants in global namespace for non-CUDA builds
#define cudaSuccess 0
#define cudaErrorMemoryAllocation 2

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

// Include actual CUDA headers when available for proper type definitions
#if SEP_ENGINE_HAS_CUDA && !defined(SEP_COMPAT_CUDA_RUNTIME_INCLUDED)
#include <driver_types.h>
#endif

// Also define types in sep::cuda namespace for consistency
namespace sep {
namespace cuda {
    using ::cudaError_t;
    using ::cudaStream_t;
    using ::cudaEvent_t;
    using ::cudaMemcpyKind;
    using ::cufftHandle;
    
    // Define constants
    constexpr cudaError_t cudaSuccess = ::cudaSuccess;
    constexpr cudaError_t cudaErrorMemoryAllocation = ::cudaErrorMemoryAllocation;
}
}

#endif // SEP_COMPAT_CUDA_FWD_H