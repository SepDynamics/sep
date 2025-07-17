#ifndef SEP_COMPAT_CUDA_TYPES_FWD_H
#define SEP_COMPAT_CUDA_TYPES_FWD_H

#include <stddef.h>  // For size_t

// This header contains only forward declarations
// No implementation details, no includes of actual CUDA headers
// This breaks circular dependencies by providing only the type information
// needed by other headers

#ifdef __cplusplus
namespace sep {
namespace cuda {
#endif

// Forward declarations of CUDA types without including CUDA headers
typedef struct CUstream_st* cudaStream_t;
typedef struct CUevent_st* cudaEvent_t;
typedef int cudaError_t;

// Memory copy kinds enum - defined here only once
#ifndef CUDA_MEMCPY_KIND_DEFINED
#define CUDA_MEMCPY_KIND_DEFINED
enum cudaMemcpyKind {
    cudaMemcpyHostToHost = 0,
    cudaMemcpyHostToDevice = 1,
    cudaMemcpyDeviceToHost = 2,
    cudaMemcpyDeviceToDevice = 3,
    cudaMemcpyDefault = 4
};
#endif

// Simple device properties struct (reduced from full CUDA version)
struct cudaDeviceProp {
    char name[256];
    int major;
    int minor;
    size_t totalGlobalMem;
    size_t sharedMemPerBlock;
    int maxThreadsPerBlock;
    int multiProcessorCount;
    int warpSize;
    int maxThreadsDim[3];
    int maxGridSize[3];
    int unifiedAddressing;
};

// Constants
#ifndef SEP_cudaMemAttachGlobal
#define SEP_cudaMemAttachGlobal 1
#endif

// Success code
#ifndef cudaSuccess
#define cudaSuccess 0
#endif

#ifdef __cplusplus
} // namespace cuda
} // namespace sep
#endif

#endif // SEP_COMPAT_CUDA_TYPES_FWD_H