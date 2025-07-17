#ifndef SEP_COMPAT_CUDA_TYPES_H
#define SEP_COMPAT_CUDA_TYPES_H

#include "compat/cuda_config.h"

// Include CUDA headers outside namespace when available
#if SEP_ENGINE_HAS_CUDA
#include <cuda_runtime_api.h>
#include <cuda_runtime.h>
#endif

// Forward declarations to prevent redefinition conflicts
struct cudaDeviceProp;  // Forward declare for compatibility

namespace sep {
namespace cuda {

// Include the actual CUDA runtime when available
#if SEP_ENGINE_HAS_CUDA
// When CUDA is available, import the actual CUDA types
using ::cudaError_t;
using ::cudaStream_t;
using ::cudaEvent_t;
using ::cudaMemcpyKind;
using ::cudaDeviceProp;
// Use the CUDA-defined cudaSuccess
static const int cudaSuccess = ::cudaSuccess;

#else
// Define success constant for non-CUDA builds
static const int cudaSuccess = 0;
// When CUDA is not available, define compatible stub types
typedef int cudaError_t;
typedef void* cudaStream_t;
typedef void* cudaEvent_t;
struct cudaDeviceProp
{
    char data[256];
};  // Simple placeholder struct

// Stub enum to match CUDA's memory copy directions
// Using the same include guard as in cuda_types_fwd.h
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

// Error code definitions (consistent regardless of CUDA availability)
enum ErrorCode
{
    Success = cudaSuccess,
    InvalidValue = 1,
    OutOfMemory = 2,
    NotInitialized = 3,
    Deinitialized = 4,
    DeviceNotLicensed = 10,
    MemoryAllocation = 2,
    InvalidDevice = 101
};

}  // namespace cuda
} // namespace sep

#endif // SEP_COMPAT_CUDA_TYPES_H