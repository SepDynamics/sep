#pragma once

#include "macros.h"
#if SEP_CUDA_AVAILABLE
#include "compat/cuda_runtime.h"
#else

#include "types.h"

#include <cstddef>
#include <memory>
#include <string>
#include <vector>

namespace sep {
namespace cuda {

// Basic CUDA types
typedef int cudaError_t;
typedef int CUresult;
typedef void* cudaStream_t;


// Device properties structure - must be defined before function declarations that use it
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
extern "C" {
cudaError_t cudaSetDevice(int device);
cudaError_t cudaGetDeviceCount(int* count);
cudaError_t cudaGetDeviceProperties(cudaDeviceProp* prop, int device);
cudaError_t cudaGetLastError(void);
const char* cudaGetErrorString(cudaError_t error);
cudaError_t cudaStreamCreate(cudaStream_t* stream);
cudaError_t cudaStreamDestroy(cudaStream_t stream);
cudaError_t cudaStreamSynchronize(cudaStream_t stream);
cudaError_t cudaMalloc(void** ptr, size_t size);
cudaError_t cudaFree(void* ptr);
cudaError_t cudaMallocHost(void** ptr, size_t size);
cudaError_t cudaMallocManaged(void** ptr, size_t size);
cudaError_t cudaMemcpy(void* dst, const void* src, size_t size, int kind);
cudaError_t cudaMemcpyAsync(void* dst, const void* src, size_t size, int kind, cudaStream_t stream);
cudaError_t cudaMemGetInfo(size_t* free, size_t* total);
}

// Error codes
#ifndef __CUDA_RUNTIME_H__
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
#endif

}  // namespace cuda
}  // namespace sep

#endif  // SEP_CUDA_AVAILABLE
