#ifndef SEP_COMPAT_CUDA_RUNTIME_H
#define SEP_COMPAT_CUDA_RUNTIME_H

// Include standard headers needed for types
#include <stddef.h>  // For size_t

// Define SEP_CUDA_AVAILABLE if not already defined
#ifndef SEP_CUDA_AVAILABLE
#define SEP_CUDA_AVAILABLE 0
#endif

// Include necessary headers
#include <string.h> // For memcpy

#if SEP_CUDA_AVAILABLE
// When CUDA is available, include the real CUDA runtime
#include "cuda_runtime.h"
#else
// When CUDA is not available, define stub types and functions

#ifdef __cplusplus
namespace sep {
namespace cuda {

// Basic CUDA types
typedef int cudaError_t;
typedef int CUresult;
typedef void* cudaStream_t;
typedef void* cudaEvent_t;

// Define cudaMemcpyKind enum
typedef enum {
    cudaMemcpyHostToHost = 0,
    cudaMemcpyHostToDevice = 1,
    cudaMemcpyDeviceToHost = 2,
    cudaMemcpyDeviceToDevice = 3,
    cudaMemcpyDefault = 4
} cudaMemcpyKind;

// Error codes
static const cudaError_t cudaSuccess = 0;
static const cudaError_t cudaErrorMemoryAllocation = 2;
static const cudaError_t cudaErrorInitializationError = 3;
static const cudaError_t cudaErrorInvalidDevice = 10;
static const cudaError_t cudaErrorDeviceUninitialized = 37;
static const cudaError_t cudaErrorInvalidValue = 11;
static const cudaError_t cudaErrorNotReady = 34;
static const cudaError_t cudaErrorSetOnActiveProcess = 711;
static const cudaError_t cudaErrorStreamCaptureUnsupported = 900;
static const cudaError_t cudaErrorInvalidMemcpyDirection = 21;
static const cudaError_t cudaErrorInvalidResourceHandle = 400;

// Stream creation flags
static const unsigned int cudaStreamDefault = 0x00;
static const unsigned int cudaStreamNonBlocking = 0x01;

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

// Function declarations
cudaError_t cudaSetDevice(int device);
cudaError_t cudaGetDeviceCount(int* count);
cudaError_t cudaGetDeviceProperties(cudaDeviceProp* prop, int device);
cudaError_t cudaGetLastError(void);
const char* cudaGetErrorString(cudaError_t error);
cudaError_t cudaStreamCreate(cudaStream_t* stream);
cudaError_t cudaStreamCreateWithFlags(cudaStream_t* stream, unsigned int flags);
cudaError_t cudaStreamDestroy(cudaStream_t stream);
cudaError_t cudaStreamSynchronize(cudaStream_t stream);
cudaError_t cudaMalloc(void** ptr, size_t size);
cudaError_t cudaFree(void* ptr);
cudaError_t cudaMallocHost(void** ptr, size_t size);
cudaError_t cudaMallocManaged(void** ptr, size_t size);
cudaError_t cudaMemcpy(void* dst, const void* src, size_t size, int kind);
cudaError_t cudaMemcpyAsync(void* dst, const void* src, size_t size, int kind, cudaStream_t stream);
cudaError_t cudaMemGetInfo(size_t* free, size_t* total);

}  // namespace cuda
}  // namespace sep

// Make the sep::cuda types available in global namespace for compatibility
using sep::cuda::cudaError_t;
using sep::cuda::cudaStream_t;
using sep::cuda::cudaEvent_t;
using sep::cuda::cudaMemcpyKind;
using sep::cuda::cudaDeviceProp;
using sep::cuda::cudaMemcpyHostToHost;
using sep::cuda::cudaMemcpyHostToDevice;
using sep::cuda::cudaMemcpyDeviceToHost;
using sep::cuda::cudaMemcpyDeviceToDevice;
using sep::cuda::cudaMemcpyDefault;
using sep::cuda::cudaSuccess;
using sep::cuda::cudaErrorMemoryAllocation;
using sep::cuda::cudaErrorInitializationError;
using sep::cuda::cudaErrorInvalidDevice;
using sep::cuda::cudaErrorDeviceUninitialized;
using sep::cuda::cudaErrorInvalidValue;
using sep::cuda::cudaErrorNotReady;
using sep::cuda::cudaErrorSetOnActiveProcess;
using sep::cuda::cudaErrorStreamCaptureUnsupported;
using sep::cuda::cudaErrorInvalidMemcpyDirection;
using sep::cuda::cudaErrorInvalidResourceHandle;
using sep::cuda::cudaStreamDefault;
using sep::cuda::cudaStreamNonBlocking;

#endif // __cplusplus
#endif // !SEP_CUDA_AVAILABLE

#endif // SEP_COMPAT_CUDA_RUNTIME_H
