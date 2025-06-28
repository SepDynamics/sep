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

// Forward declare CUDA types in global scope
#if !SEP_CUDA_AVAILABLE
typedef int cudaError_t;
typedef void* cudaStream_t;
typedef void* cudaEvent_t;

// Error codes in global scope
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

// Memory copy kinds in global scope
enum cudaMemcpyKind {
    cudaMemcpyHostToHost = 0,
    cudaMemcpyHostToDevice = 1,
    cudaMemcpyDeviceToHost = 2,
    cudaMemcpyDeviceToDevice = 3,
    cudaMemcpyDefault = 4
};

// Device properties structure in global scope
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

// Stream flags in global scope
constexpr unsigned int cudaStreamDefault = 0x00;
constexpr unsigned int cudaStreamNonBlocking = 0x01;
#endif

#if SEP_CUDA_AVAILABLE
// When CUDA is available, include the real CUDA runtime
#include <cuda_runtime.h>
#include "compat/cuda_helpers.h"
#else
// When CUDA is not available, define stub types and functions

#ifdef __cplusplus
namespace sep {
namespace cuda {

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

#endif // __cplusplus
#endif // !SEP_CUDA_AVAILABLE

#endif // SEP_COMPAT_CUDA_RUNTIME_H
