#ifndef SEP_COMPAT_CUDA_TYPES_H
#define SEP_COMPAT_CUDA_TYPES_H

#include <cstddef>  // For size_t
#include "compat/cuda_config.h"
#include "compat/cuda_fwd.h"  // Include forward declarations first

namespace sep {
namespace cuda {

// Import types from global namespace
using ::cudaError_t;
using ::cudaStream_t;
using ::cudaEvent_t;
using ::cudaMemcpyKind;

#if SEP_ENGINE_HAS_CUDA
// When CUDA is available, use the actual types
using ::cudaDeviceProp;
#else
// For non-CUDA builds, define a placeholder
struct cudaDeviceProp
{
    char name[256];
    size_t totalGlobalMem;
    size_t sharedMemPerBlock;
    int regsPerBlock;
    int warpSize;
    size_t memPitch;
    int maxThreadsPerBlock;
    int maxThreadsDim[3];
    int maxGridSize[3];
    int clockRate;
    size_t totalConstMem;
    int major;
    int minor;
    size_t textureAlignment;
    int deviceOverlap;
    int multiProcessorCount;
    int kernelExecTimeoutEnabled;
    int integrated;
    int canMapHostMemory;
    int computeMode;
};
#endif

// Error code definitions (consistent regardless of CUDA availability)
enum ErrorCode
{
    Success = SEP_CUDA_SUCCESS,
    InvalidValue = 1,
    OutOfMemory = 2,
    NotInitialized = 3,
    Deinitialized = 4,
    DeviceNotLicensed = 10,
    MemoryAllocation = SEP_CUDA_ERROR_MEMORY_ALLOCATION,
    InvalidDevice = 101
};

}  // namespace cuda
} // namespace sep

#endif // SEP_COMPAT_CUDA_TYPES_H