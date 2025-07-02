#ifndef SEP_COMPAT_CUDA_RUNTIME_H
#define SEP_COMPAT_CUDA_RUNTIME_H

// Always use the lightweight runtime shim
#include <stddef.h>
#include <string.h>

// Forward declare CUDA types in global scope
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

#ifdef __cplusplus
namespace sep {
namespace cuda {

using ::cudaError_t;
using ::cudaStream_t;
using ::cudaEvent_t;
using ::cudaMemcpyKind;

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
cudaError_t cudaStreamWaitEvent(cudaStream_t stream, cudaEvent_t event, unsigned int flags);
cudaError_t cudaEventRecord(cudaEvent_t event, cudaStream_t stream);
cudaError_t cudaEventCreateWithFlags(cudaEvent_t* event, unsigned int flags);
cudaError_t cudaEventCreate(cudaEvent_t* event);
cudaError_t cudaEventDestroy(cudaEvent_t event);
cudaError_t cudaEventSynchronize(cudaEvent_t event);
cudaError_t cudaEventElapsedTime(float* ms, cudaEvent_t start, cudaEvent_t end);
cudaError_t cudaMalloc(void** ptr, size_t size);
cudaError_t cudaFree(void* ptr);
cudaError_t cudaMallocHost(void** ptr, size_t size);
cudaError_t cudaMallocManaged(void** ptr, size_t size);
cudaError_t cudaMemcpy(void* dst, const void* src, size_t size, int kind);
cudaError_t cudaMemcpyAsync(void* dst, const void* src, size_t size, int kind, cudaStream_t stream);
cudaError_t cudaMemGetInfo(size_t* free, size_t* total);
cudaError_t cudaStreamAttachMemAsync(cudaStream_t stream, void* ptr, size_t size, unsigned int flags);

}  // namespace cuda
}  // namespace sep

#if defined(__cplusplus)
using namespace sep::cuda; // Provide global aliases
#endif

#endif // __cplusplus

#endif // SEP_COMPAT_CUDA_RUNTIME_H
