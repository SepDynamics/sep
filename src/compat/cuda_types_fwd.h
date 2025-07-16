#ifndef SEP_COMPAT_CUDA_TYPES_FWD_H
#define SEP_COMPAT_CUDA_TYPES_FWD_H

#include <stddef.h> // For size_t

namespace sep {
namespace cuda {

// Forward declarations of CUDA types
using cudaError_t = int;
using cudaStream_t = void*;
using cudaEvent_t = void*;
using cudaArray_t = void*;
using cudaArray_const_t = void*;
using cudaGraphicsResource_t = void*;
using cudaMipmappedArray_t = void*;
using cudaMipmappedArray_const_t = void*;
using cudaTextureObject_t = void*;
using cudaSurfaceObject_t = void*;

// Memory copy kinds enum forward declaration
enum cudaMemcpyKind
{
    cudaMemcpyHostToHost = 0,
    cudaMemcpyHostToDevice = 1,
    cudaMemcpyDeviceToHost = 2,
    cudaMemcpyDeviceToDevice = 3,
    cudaMemcpyDefault = 4
};

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

// Error code constants
constexpr cudaError_t cudaSuccess = 0;
constexpr cudaError_t cudaErrorMemoryAllocation = 2;
constexpr cudaError_t cudaErrorInitializationError = 3;
constexpr cudaError_t cudaErrorInvalidDevice = 10;
constexpr cudaError_t cudaErrorInvalidValue = 11;
constexpr cudaError_t cudaErrorDeviceUninitialized = 37;
constexpr cudaError_t cudaErrorNotReady = 34;
constexpr cudaError_t cudaErrorSetOnActiveProcess = 711;
constexpr cudaError_t cudaErrorStreamCaptureUnsupported = 900;
constexpr cudaError_t cudaErrorInvalidMemcpyDirection = 21;
constexpr cudaError_t cudaErrorInvalidResourceHandle = 400;
constexpr cudaError_t cudaErrorOutOfMemory = 2;
constexpr cudaError_t cudaErrorInvalidDevicePointer = 17;
constexpr cudaError_t cudaErrorInvalidConfiguration = 9;
constexpr cudaError_t cudaErrorInvalidPitchValue = 12;
constexpr cudaError_t cudaErrorInvalidSymbol = 13;
constexpr cudaError_t cudaErrorDeviceAlreadyInUse = 54;
constexpr cudaError_t cudaErrorDeviceNotLicensed = 803;
constexpr cudaError_t cudaErrorDeviceUninitilialized = 37;
constexpr cudaError_t cudaErrorNoDevice = 100;

// Stream and memory flags
constexpr unsigned int SEP_cudaStreamDefault = 0x00;
constexpr unsigned int SEP_cudaStreamNonBlocking = 0x01;
constexpr unsigned int SEP_cudaMemAttachGlobal = 0x01;
constexpr unsigned int SEP_cudaMemAttachHost = 0x02;
constexpr unsigned int SEP_cudaMemAttachSingle = 0x04;
constexpr unsigned int SEP_cudaHostAllocDefault = 0x00;
constexpr unsigned int SEP_cudaHostAllocPortable = 0x01;
constexpr unsigned int SEP_cudaHostAllocMapped = 0x02;
constexpr unsigned int SEP_cudaHostAllocWriteCombined = 0x04;
constexpr unsigned int SEP_cudaEventDefault = 0x00;
constexpr unsigned int SEP_cudaEventBlockingSync = 0x01;
constexpr unsigned int SEP_cudaEventDisableTiming = 0x02;
constexpr unsigned int SEP_cudaEventInterprocess = 0x04;

}  // namespace cuda
}  // namespace sep

#endif // SEP_COMPAT_CUDA_TYPES_FWD_H