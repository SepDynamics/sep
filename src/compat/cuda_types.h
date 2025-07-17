#ifndef SEP_COMPAT_CUDA_TYPES_H
#define SEP_COMPAT_CUDA_TYPES_H

#include "compat/cuda_config.h"
#include "compat/cuda_fwd.h"  // Include forward declarations first

// Include CUDA headers outside namespace when available
#if SEP_ENGINE_HAS_CUDA
#include <cuda_runtime_api.h>
#include <cuda_runtime.h>
#endif

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
// Types are already defined in cuda_fwd.h
struct cudaDeviceProp
{
    char data[256];
};  // Simple placeholder struct
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