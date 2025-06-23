#ifndef SEP_COMPAT_CUDA_RUNTIME_H
#define SEP_COMPAT_CUDA_RUNTIME_H

// Avoid defining __noinline__ to prevent conflicts with the standard library

// Include C++ headers first, without CUDA interference
#include <cmath>
#include <type_traits>
#include <utility>

// Tell GCC we've already included math.h to prevent redefinitions
#define _GLIBCXX_MATH_H 1

// These prevent CUDA from trying to redefine constexpr math functions
#define __CUDACC_RTC__ 1
#define __MATH_FUNCTIONS_DECL__ inline
#define __MATH_FUNCTIONS_DEVICE_DECL__ inline

// This prevents functions like abs, sqrt, etc. from causing errors
#ifndef __CUDABE__
#define __CUDABE__
#endif

// Set CUDA architecture and PTX version before including runtime
#ifndef __CUDA_ARCH__
#define __CUDA_ARCH__ 520  // Minimum supported architecture
#endif

// Define PTX ISA version to ensure proper function variants are used
#ifndef __CUDA_ARCH_PTX__
#define __CUDA_ARCH_PTX__ 520
#endif

// Prevent CUDA from using _ptsz variants by forcing PTX ISA version
#ifndef __CUDACC_VER_MAJOR__
#define __CUDACC_VER_MAJOR__ 12
#endif

#ifndef __CUDACC_VER_MINOR__
#define __CUDACC_VER_MINOR__ 9
#endif

#ifndef __CUDACC_VER_BUILD__
#define __CUDACC_VER_BUILD__ 0
#endif

// Now include CUDA runtime with the protections in place
#include "compat/cuda_runtime.h"

// Allow the toolchain to define __noinline__ if needed

// Forward declare CUDA types if not using CUDA runtime
#if !defined(__CUDA_RUNTIME_H__)
struct CUstream_st;
struct CUevent_st;
typedef struct CUstream_st* cudaStream_t;
typedef struct CUevent_st* cudaEvent_t;
typedef enum cudaError {
    cudaSuccess = 0
} cudaError_t;
typedef enum cudaMemcpyKind {
    cudaMemcpyHostToHost = 0,
    cudaMemcpyHostToDevice = 1,
    cudaMemcpyDeviceToHost = 2,
    cudaMemcpyDeviceToDevice = 3
} cudaMemcpyKind;
#endif

// Declare CUDA functions with proper linkage
#ifdef __cplusplus
extern "C" {
#endif

// Stream management functions
cudaError_t cudaStreamSynchronize_ptsz(cudaStream_t stream);
cudaError_t cudaStreamWaitEvent_ptsz(cudaStream_t stream, cudaEvent_t event, unsigned int flags);
cudaError_t cudaStreamCreateWithFlags(cudaStream_t* stream, unsigned int flags);
cudaError_t cudaStreamDestroy(cudaStream_t stream);

// Event management functions
cudaError_t cudaEventCreate(cudaEvent_t* event);
cudaError_t cudaEventDestroy(cudaEvent_t event);
cudaError_t cudaEventRecord_ptsz(cudaEvent_t event, cudaStream_t stream);
cudaError_t cudaEventSynchronize(cudaEvent_t event);
cudaError_t cudaEventElapsedTime(float* ms, cudaEvent_t start, cudaEvent_t end);

// Memory management functions
cudaError_t cudaFree(void* ptr);
cudaError_t cudaMemGetInfo(size_t* free, size_t* total);
cudaError_t cudaMemcpyAsync(void* dst, const void* src, size_t count, enum cudaMemcpyKind kind, cudaStream_t stream);
cudaError_t cudaStreamAttachMemAsync_ptsz(cudaStream_t stream, void* devPtr, size_t length, unsigned int flags);

// Device management functions
cudaError_t cudaSetDevice(int device);
cudaError_t cudaGetDeviceCount(int* count);
cudaError_t cudaGetDeviceProperties(struct cudaDeviceProp* prop, int device);
cudaError_t cudaGetLastError(void);
const char* cudaGetErrorString(cudaError_t error);

#ifdef __cplusplus
}
#endif

// Declare cudaMallocManaged in a separate scope to avoid redefinition
#if !defined(cudaMallocManaged) && !defined(__CUDA_RUNTIME_H__)
#ifdef __cplusplus
extern "C" {
#endif
cudaError_t cudaMallocManaged(void** ptr, size_t size, unsigned int flags);
#ifdef __cplusplus
}
#endif
#endif

// Define missing CUDA function aliases if needed
#ifndef cudaStreamSynchronize
#define cudaStreamSynchronize cudaStreamSynchronize_ptsz
#endif

#ifndef cudaStreamWaitEvent
#define cudaStreamWaitEvent cudaStreamWaitEvent_ptsz
#endif

#ifndef cudaEventRecord
#define cudaEventRecord cudaEventRecord_ptsz
#endif

#ifndef cudaStreamAttachMemAsync
#define cudaStreamAttachMemAsync cudaStreamAttachMemAsync_ptsz
#endif

#endif // SEP_COMPAT_CUDA_RUNTIME_H
