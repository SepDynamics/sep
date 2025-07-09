#ifndef SEP_COMPAT_CUDA_RUNTIME_H
#define SEP_COMPAT_CUDA_RUNTIME_H

#if SEP_ENGINE_HAS_CUDA
#include <cuda_runtime.h>

namespace sep {
namespace cuda {
// Alias CUDA types into our namespace
using ::cudaError_t;
using ::cudaStream_t;
using ::cudaEvent_t;
using ::cudaMemcpyKind;
using ::cudaSuccess;
using ::cudaStreamNonBlocking;
using ::cudaStreamDefault;
} // namespace cuda
} // namespace sep

// Make CUDA types available in global scope
using sep::cuda::cudaError_t;
using sep::cuda::cudaStream_t;
using sep::cuda::cudaEvent_t;
using sep::cuda::cudaMemcpyKind;
using sep::cuda::cudaSuccess;
using sep::cuda::cudaStreamNonBlocking;
using sep::cuda::cudaStreamDefault;

#else // !SEP_ENGINE_HAS_CUDA
// Basic includes for non-CUDA builds
#include <stddef.h>
#include <cstring>

// Basic CUDA types
namespace sep {
namespace cuda {
typedef int cudaError_t;
typedef void* cudaStream_t;
typedef void* cudaEvent_t;
typedef void* cudaArray_t;
typedef void* cudaArray_const_t;
typedef void* cudaGraphicsResource_t;
typedef void* cudaMipmappedArray_t;
typedef void* cudaMipmappedArray_const_t;
typedef void* cudaTextureObject_t;
typedef void* cudaSurfaceObject_t;
} // namespace cuda
} // namespace sep

// Make types available in global scope
using sep::cuda::cudaError_t;
using sep::cuda::cudaStream_t;
using sep::cuda::cudaEvent_t;
using sep::cuda::cudaArray_t;
using sep::cuda::cudaArray_const_t;
using sep::cuda::cudaGraphicsResource_t;
using sep::cuda::cudaMipmappedArray_t;
using sep::cuda::cudaMipmappedArray_const_t;
using sep::cuda::cudaTextureObject_t;
using sep::cuda::cudaSurfaceObject_t;

// Memory copy kinds
namespace sep {
namespace cuda {
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
} // namespace cuda
} // namespace sep

// Make enum available in global scope
using sep::cuda::cudaMemcpyKind;

namespace sep {
namespace cuda {

// Constants
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

constexpr unsigned int cudaStreamDefault = 0x00;
constexpr unsigned int cudaStreamNonBlocking = 0x01;
constexpr unsigned int cudaMemAttachGlobal = 0x01;
constexpr unsigned int cudaMemAttachHost = 0x02;
constexpr unsigned int cudaMemAttachSingle = 0x04;
constexpr unsigned int cudaHostAllocDefault = 0x00;
constexpr unsigned int cudaHostAllocPortable = 0x01;
constexpr unsigned int cudaHostAllocMapped = 0x02;
constexpr unsigned int cudaHostAllocWriteCombined = 0x04;
constexpr unsigned int cudaEventDefault = 0x00;
constexpr unsigned int cudaEventBlockingSync = 0x01;
constexpr unsigned int cudaEventDisableTiming = 0x02;
constexpr unsigned int cudaEventInterprocess = 0x04;

} // namespace cuda
} // namespace sep

// Make constants available in global scope
using sep::cuda::cudaSuccess;
using sep::cuda::cudaErrorMemoryAllocation;
using sep::cuda::cudaErrorInitializationError;
using sep::cuda::cudaErrorInvalidDevice;
using sep::cuda::cudaErrorInvalidValue;
using sep::cuda::cudaErrorDeviceUninitialized;
using sep::cuda::cudaErrorNotReady;
using sep::cuda::cudaErrorSetOnActiveProcess;
using sep::cuda::cudaErrorStreamCaptureUnsupported;
using sep::cuda::cudaErrorInvalidMemcpyDirection;
using sep::cuda::cudaErrorInvalidResourceHandle;
using sep::cuda::cudaErrorOutOfMemory;
using sep::cuda::cudaErrorInvalidDevicePointer;
using sep::cuda::cudaErrorInvalidConfiguration;
using sep::cuda::cudaErrorInvalidPitchValue;
using sep::cuda::cudaErrorInvalidSymbol;
using sep::cuda::cudaErrorDeviceAlreadyInUse;
using sep::cuda::cudaErrorDeviceNotLicensed;
using sep::cuda::cudaErrorDeviceUninitilialized;
using sep::cuda::cudaErrorNoDevice;

using sep::cuda::cudaStreamDefault;
using sep::cuda::cudaStreamNonBlocking;
using sep::cuda::cudaMemAttachGlobal;
using sep::cuda::cudaMemAttachHost;
using sep::cuda::cudaMemAttachSingle;
using sep::cuda::cudaHostAllocDefault;
using sep::cuda::cudaHostAllocPortable;
using sep::cuda::cudaHostAllocMapped;
using sep::cuda::cudaHostAllocWriteCombined;
using sep::cuda::cudaEventDefault;
using sep::cuda::cudaEventBlockingSync;
using sep::cuda::cudaEventDisableTiming;
using sep::cuda::cudaEventInterprocess;

#ifdef __cplusplus
namespace sep {
namespace cuda {

// Function declarations in sep::cuda namespace
cudaError_t cudaSetDevice(int device);
cudaError_t cudaGetDevice(int* device);
cudaError_t cudaGetDeviceCount(int* count);
cudaError_t cudaGetDeviceProperties(cudaDeviceProp* prop, int device);
cudaError_t cudaDeviceSynchronize(void);
cudaError_t cudaDeviceReset(void);
cudaError_t cudaSetDeviceFlags(unsigned int flags);
cudaError_t cudaGetDeviceFlags(unsigned int* flags);
cudaError_t cudaDeviceSetCacheConfig(int cacheConfig);
cudaError_t cudaDeviceGetCacheConfig(int* cacheConfig);
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
cudaError_t cudaFreeHost(void* ptr);
cudaError_t cudaMallocManaged(void** ptr, size_t size, unsigned int flags = cudaMemAttachGlobal);
cudaError_t cudaHostAlloc(void** ptr, size_t size, unsigned int flags);
cudaError_t cudaMemcpy(void* dst, const void* src, size_t size, cudaMemcpyKind kind);
cudaError_t cudaMemcpyAsync(void* dst, const void* src, size_t size, cudaMemcpyKind kind, cudaStream_t stream);
cudaError_t cudaMemGetInfo(size_t* free, size_t* total);
cudaError_t cudaStreamAttachMemAsync(cudaStream_t stream, void* ptr, size_t size, unsigned int flags);
cudaError_t cudaMemset(void* devPtr, int value, size_t count);
cudaError_t cudaMemsetAsync(void* devPtr, int value, size_t count, cudaStream_t stream);

} // namespace cuda
} // namespace sep

// Make functions available in global scope
using sep::cuda::cudaSetDevice;
using sep::cuda::cudaGetDevice;
using sep::cuda::cudaGetDeviceCount;
using sep::cuda::cudaGetDeviceProperties;
using sep::cuda::cudaDeviceSynchronize;
using sep::cuda::cudaDeviceReset;
using sep::cuda::cudaSetDeviceFlags;
using sep::cuda::cudaGetDeviceFlags;
using sep::cuda::cudaDeviceSetCacheConfig;
using sep::cuda::cudaDeviceGetCacheConfig;
using sep::cuda::cudaGetLastError;
using sep::cuda::cudaGetErrorString;
using sep::cuda::cudaStreamCreate;
using sep::cuda::cudaStreamCreateWithFlags;
using sep::cuda::cudaStreamDestroy;
using sep::cuda::cudaStreamSynchronize;
using sep::cuda::cudaStreamWaitEvent;
using sep::cuda::cudaEventRecord;
using sep::cuda::cudaEventCreateWithFlags;
using sep::cuda::cudaEventCreate;
using sep::cuda::cudaEventDestroy;
using sep::cuda::cudaEventSynchronize;
using sep::cuda::cudaEventElapsedTime;
using sep::cuda::cudaMalloc;
using sep::cuda::cudaFree;
using sep::cuda::cudaMallocHost;
using sep::cuda::cudaFreeHost;
using sep::cuda::cudaMallocManaged;
using sep::cuda::cudaHostAlloc;
using sep::cuda::cudaMemcpy;
using sep::cuda::cudaMemcpyAsync;
using sep::cuda::cudaMemGetInfo;
using sep::cuda::cudaStreamAttachMemAsync;
using sep::cuda::cudaMemset;
using sep::cuda::cudaMemsetAsync;

#endif // __cplusplus
#endif // !SEP_CUDA_AVAILABLE
#endif // SEP_COMPAT_CUDA_RUNTIME_H
