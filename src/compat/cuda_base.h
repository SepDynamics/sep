#ifndef SEP_COMPAT_CUDA_BASE_H
#define SEP_COMPAT_CUDA_BASE_H

// This is the base header that contains only the most fundamental type definitions
// with no actual implementations or includes to avoid circular dependencies

#include <stddef.h>  // For size_t

// Configuration macro if not already defined
#ifndef SEP_ENGINE_HAS_CUDA
#define SEP_ENGINE_HAS_CUDA 0
#endif

namespace sep {
namespace cuda {

// Basic type definitions - no CUDA headers included here
#if SEP_ENGINE_HAS_CUDA
// Forward declarations only, without including CUDA headers
struct CUevent_st;
struct CUstream_st;
struct cudaArray;
struct cudaMipmappedArray;
struct cudaGraphicsResource;
struct textureReference;
struct surfaceReference;
struct cudaFuncAttributes;

using cudaError_t = int;
using cudaStream_t = CUstream_st*;
using cudaEvent_t = CUevent_st*;
using cudaArray_t = cudaArray*;
using cudaArray_const_t = const cudaArray*;
using cudaGraphicsResource_t = cudaGraphicsResource*;
using cudaMipmappedArray_t = cudaMipmappedArray*;
using cudaMipmappedArray_const_t = const cudaMipmappedArray*;
using cudaTextureObject_t = long long unsigned int;
using cudaSurfaceObject_t = long long unsigned int;
#else
// Stub types when CUDA is not available
using cudaError_t = int;
using cudaStream_t = void*;
using cudaEvent_t = void*;
using cudaArray_t = void*;
using cudaArray_const_t = void*;
using cudaGraphicsResource_t = void*;
using cudaMipmappedArray_t = void*;
using cudaMipmappedArray_const_t = void*;
using cudaTextureObject_t = unsigned long long;
using cudaSurfaceObject_t = unsigned long long;
#endif

// NOTE: cudaMemcpyKind is now defined in cuda_types.h to avoid redefinition

// Device properties structure
struct cudaDeviceProp {
    char name[256];
    int major;
    int minor;
    size_t totalGlobalMem;
    size_t sharedMemPerBlock;
    int maxThreadsPerBlock;
    int maxThreadsPerMultiProcessor;
    int multiProcessorCount;
    int warpSize;
    int maxThreadsDim[3];
    int maxGridSize[3];
    int clockRate;
    size_t totalConstMem;
    int deviceOverlap;
    int kernelExecTimeoutEnabled;
    int integrated;
    int canMapHostMemory;
    int unifiedAddressing;
    int concurrentKernels;
};

}  // namespace cuda
}  // namespace sep

#endif  // SEP_COMPAT_CUDA_BASE_H