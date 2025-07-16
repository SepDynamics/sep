#ifndef SEP_COMPAT_CUDA_RUNTIME_H
#define SEP_COMPAT_CUDA_RUNTIME_H

#include <stddef.h>  // For size_t

#if SEP_ENGINE_HAS_CUDA
#include <cuda_runtime_api.h>
#include <cuda_runtime.h>

// Map SEP-prefixed names to CUDA enums/constants/functions when CUDA is available
#define SEP_cudaSuccess cudaSuccess
#define SEP_cudaErrorInvalidValue cudaErrorInvalidValue
#define SEP_cudaErrorMemoryAllocation cudaErrorMemoryAllocation
#define SEP_cudaStreamDestroy cudaStreamDestroy
#define SEP_cudaGetErrorString cudaGetErrorString
#define SEP_cudaEventCreate cudaEventCreate
#define SEP_cudaEventDestroy cudaEventDestroy
#define SEP_cudaEventRecord cudaEventRecord
#define SEP_cudaEventSynchronize cudaEventSynchronize
#define SEP_cudaEventElapsedTime cudaEventElapsedTime
#define SEP_cudaMemGetInfo cudaMemGetInfo
#define SEP_cudaErrorInitializationError cudaErrorInitializationError
#define SEP_cudaErrorInvalidDevicePointer cudaErrorInvalidDevicePointer
#define SEP_cudaErrorInvalidMemcpyDirection cudaErrorInvalidMemcpyDirection
#define SEP_cudaErrorNoDevice cudaErrorNoDevice
#define SEP_cudaErrorInvalidDevice cudaErrorInvalidDevice
#define SEP_cudaErrorDeviceUninitialized cudaErrorDeviceUninitialized
#define SEP_cudaErrorDeviceAlreadyInUse cudaErrorDeviceAlreadyInUse
#define SEP_cudaErrorInvalidResourceHandle cudaErrorInvalidResourceHandle
#define SEP_cudaErrorNotReady cudaErrorNotReady
#define SEP_cudaErrorSetOnActiveProcess cudaErrorSetOnActiveProcess
#define SEP_cudaErrorStreamCaptureUnsupported cudaErrorStreamCaptureUnsupported
#define SEP_cudaStreamDefault cudaStreamDefault
#define SEP_cudaStreamNonBlocking cudaStreamNonBlocking

namespace sep {
namespace cuda {
// Alias CUDA types into our namespace
using ::cudaError_t;
using ::cudaStream_t;
using ::cudaEvent_t;
using ::cudaMemcpyKind;
using ::cudaSuccess;

// Inline function wrappers for namespace-safe CUDA function usage
inline cudaError_t SEP_cudaStreamDestroy(cudaStream_t stream) {
    return ::cudaStreamDestroy(stream);
}

inline cudaError_t SEP_cudaMemcpy(void* dst, const void* src, size_t size, cudaMemcpyKind kind) {
    return ::cudaMemcpy(dst, src, size, kind);
}

inline cudaError_t SEP_cudaMemcpyAsync(void* dst, const void* src, size_t size,
                                       cudaMemcpyKind kind, cudaStream_t stream) {
    return ::cudaMemcpyAsync(dst, src, size, kind, stream);
}
} // namespace cuda
} // namespace sep

// Make CUDA types available in global scope
using sep::cuda::cudaError_t;
using sep::cuda::cudaStream_t;
using sep::cuda::cudaEvent_t;
using sep::cuda::cudaMemcpyKind;
using sep::cuda::cudaSuccess;

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
#ifndef SEP_COMPAT_CUDA_RUNTIME_H
#define SEP_COMPAT_CUDA_RUNTIME_H

#include <cstddef>

#if SEP_ENGINE_HAS_CUDA
#include <cuda_runtime.h>
#include <cuda_runtime_api.h>
#endif

namespace sep {
namespace cuda {

#if SEP_ENGINE_HAS_CUDA
    // Use actual CUDA types when available
    using cudaError_t = ::cudaError_t;
    using cudaStream_t = ::cudaStream_t;
    using cudaEvent_t = ::cudaEvent_t;
    using cudaMemcpyKind = ::cudaMemcpyKind;

    // Map CUDA functions
    inline cudaError_t SEP_cudaStreamDestroy(cudaStream_t stream)
    {
        return ::cudaStreamDestroy(stream);
    }
    inline const char* SEP_cudaGetErrorString(cudaError_t error)
    {
        return ::cudaGetErrorString(error);
    }
    inline cudaError_t SEP_cudaEventCreate(cudaEvent_t* event) { return ::cudaEventCreate(event); }
    inline cudaError_t SEP_cudaEventDestroy(cudaEvent_t event) { return ::cudaEventDestroy(event); }
    inline cudaError_t SEP_cudaEventRecord(cudaEvent_t event, cudaStream_t stream)
    {
        return ::cudaEventRecord(event, stream);
    }
    inline cudaError_t SEP_cudaEventSynchronize(cudaEvent_t event)
    {
        return ::cudaEventSynchronize(event);
    }
    inline cudaError_t SEP_cudaEventElapsedTime(float* ms, cudaEvent_t start, cudaEvent_t end)
    {
        return ::cudaEventElapsedTime(ms, start, end);
    }
    inline cudaError_t SEP_cudaMemGetInfo(size_t* free, size_t* total)
    {
        return ::cudaMemGetInfo(free, total);
    }
    inline cudaError_t SEP_cudaMemcpy(void* dst, const void* src, size_t size, cudaMemcpyKind kind)
    {
        return ::cudaMemcpy(dst, src, size, kind);
    }
    inline cudaError_t SEP_cudaMemcpyAsync(void* dst, const void* src, size_t size,
                                           cudaMemcpyKind kind, cudaStream_t stream)
    {
        return ::cudaMemcpyAsync(dst, src, size, kind, stream);
    }

#else
    // Stub types when CUDA is not available
    using cudaError_t = int;
    using cudaStream_t = void*;
    using cudaEvent_t = void*;

    enum cudaMemcpyKind
    {
        cudaMemcpyHostToHost = 0,
        cudaMemcpyHostToDevice = 1,
        cudaMemcpyDeviceToHost = 2,
        cudaMemcpyDeviceToDevice = 3,
        cudaMemcpyDefault = 4
    };

    static constexpr cudaError_t cudaSuccess = 0;

    // Function stubs
    inline cudaError_t SEP_cudaStreamDestroy(cudaStream_t) { return cudaSuccess; }
    inline const char* SEP_cudaGetErrorString(cudaError_t) { return "CUDA not available"; }
    inline cudaError_t SEP_cudaEventCreate(cudaEvent_t*) { return cudaSuccess; }
    inline cudaError_t SEP_cudaEventDestroy(cudaEvent_t) { return cudaSuccess; }
    inline cudaError_t SEP_cudaEventRecord(cudaEvent_t, cudaStream_t) { return cudaSuccess; }
    inline cudaError_t SEP_cudaEventSynchronize(cudaEvent_t) { return cudaSuccess; }
    inline cudaError_t SEP_cudaEventElapsedTime(float*, cudaEvent_t, cudaEvent_t)
    {
        return cudaSuccess;
    }
    inline cudaError_t SEP_cudaMemGetInfo(size_t* free, size_t* total)
    {
        *free = 0;
        *total = 0;
        return cudaSuccess;
    }
    inline cudaError_t SEP_cudaMemcpy(void*, const void*, size_t, cudaMemcpyKind)
    {
        return cudaSuccess;
    }
    inline cudaError_t SEP_cudaMemcpyAsync(void*, const void*, size_t, cudaMemcpyKind, cudaStream_t)
    {
        return cudaSuccess;
    }
#endif

}  // namespace cuda
}  // namespace sep

#endif  // SEP_COMPAT_CUDA_RUNTIME_H

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

using sep::cuda::SEP_cudaEventBlockingSync;
using sep::cuda::SEP_cudaEventDefault;
using sep::cuda::SEP_cudaEventDisableTiming;
using sep::cuda::SEP_cudaEventInterprocess;
using sep::cuda::SEP_cudaHostAllocDefault;
using sep::cuda::SEP_cudaHostAllocMapped;
using sep::cuda::SEP_cudaHostAllocPortable;
using sep::cuda::SEP_cudaHostAllocWriteCombined;
using sep::cuda::SEP_cudaMemAttachGlobal;
using sep::cuda::SEP_cudaMemAttachHost;
using sep::cuda::SEP_cudaMemAttachSingle;

#ifdef __cplusplus
namespace sep {
namespace cuda {

// Function declarations in sep::cuda namespace
cudaError_t SEP_cudaSetDevice(int device);
cudaError_t SEP_cudaGetDevice(int* device);
cudaError_t SEP_cudaGetDeviceCount(int* count);
cudaError_t SEP_cudaGetDeviceProperties(cudaDeviceProp* prop, int device);
cudaError_t SEP_cudaDeviceSynchronize(void);
cudaError_t SEP_cudaDeviceReset(void);
cudaError_t SEP_cudaSetDeviceFlags(unsigned int flags);
cudaError_t SEP_cudaGetDeviceFlags(unsigned int* flags);
cudaError_t SEP_cudaDeviceSetCacheConfig(int cacheConfig);
cudaError_t SEP_cudaDeviceGetCacheConfig(int* cacheConfig);
cudaError_t SEP_cudaGetLastError(void);
const char* SEP_cudaGetErrorString(cudaError_t error);
cudaError_t SEP_cudaStreamCreate(cudaStream_t* stream);
cudaError_t SEP_cudaStreamCreateWithFlags(cudaStream_t* stream, unsigned int flags);
cudaError_t SEP_cudaStreamDestroy(cudaStream_t stream);
cudaError_t SEP_cudaStreamSynchronize(cudaStream_t stream);
cudaError_t SEP_cudaStreamWaitEvent(cudaStream_t stream, cudaEvent_t event, unsigned int flags);
cudaError_t SEP_cudaEventRecord(cudaEvent_t event, cudaStream_t stream);
cudaError_t SEP_cudaEventCreateWithFlags(cudaEvent_t* event, unsigned int flags);
cudaError_t SEP_cudaEventCreate(cudaEvent_t* event);
cudaError_t SEP_cudaEventDestroy(cudaEvent_t event);
cudaError_t SEP_cudaEventSynchronize(cudaEvent_t event);
cudaError_t SEP_cudaEventElapsedTime(float* ms, cudaEvent_t start, cudaEvent_t end);
cudaError_t SEP_cudaMalloc(void** ptr, size_t size);
cudaError_t SEP_cudaFree(void* ptr);
cudaError_t SEP_cudaMallocHost(void** ptr, size_t size);
cudaError_t SEP_cudaFreeHost(void* ptr);
cudaError_t SEP_cudaMallocManaged(void** ptr, size_t size,
                                  unsigned int flags = SEP_cudaMemAttachGlobal);
cudaError_t SEP_cudaHostAlloc(void** ptr, size_t size, unsigned int flags);
cudaError_t SEP_cudaMemGetInfo(size_t* free, size_t* total);
cudaError_t SEP_cudaStreamAttachMemAsync(cudaStream_t stream, void* ptr, size_t size,
                                         unsigned int flags);
cudaError_t SEP_cudaMemset(void* devPtr, int value, size_t count);
cudaError_t SEP_cudaMemsetAsync(void* devPtr, int value, size_t count, cudaStream_t stream);

} // namespace cuda
} // namespace sep

// Make functions available in global scope
using sep::cuda::SEP_cudaDeviceGetCacheConfig;
using sep::cuda::SEP_cudaDeviceReset;
using sep::cuda::SEP_cudaDeviceSetCacheConfig;
using sep::cuda::SEP_cudaDeviceSynchronize;
using sep::cuda::SEP_cudaEventCreate;
using sep::cuda::SEP_cudaEventCreateWithFlags;
using sep::cuda::SEP_cudaEventDestroy;
using sep::cuda::SEP_cudaEventElapsedTime;
using sep::cuda::SEP_cudaEventRecord;
using sep::cuda::SEP_cudaEventSynchronize;
using sep::cuda::SEP_cudaFree;
using sep::cuda::SEP_cudaFreeHost;
using sep::cuda::SEP_cudaGetDevice;
using sep::cuda::SEP_cudaGetDeviceCount;
using sep::cuda::SEP_cudaGetDeviceFlags;
using sep::cuda::SEP_cudaGetDeviceProperties;
using sep::cuda::SEP_cudaGetErrorString;
using sep::cuda::SEP_cudaGetLastError;
using sep::cuda::SEP_cudaHostAlloc;
using sep::cuda::SEP_cudaMalloc;
using sep::cuda::SEP_cudaMallocHost;
using sep::cuda::SEP_cudaMallocManaged;
using sep::cuda::SEP_cudaMemGetInfo;
using sep::cuda::SEP_cudaMemset;
using sep::cuda::SEP_cudaMemsetAsync;
using sep::cuda::SEP_cudaSetDevice;
using sep::cuda::SEP_cudaSetDeviceFlags;
using sep::cuda::SEP_cudaStreamAttachMemAsync;
using sep::cuda::SEP_cudaStreamCreate;
using sep::cuda::SEP_cudaStreamCreateWithFlags;
using sep::cuda::SEP_cudaStreamDestroy;
using sep::cuda::SEP_cudaStreamSynchronize;
using sep::cuda::SEP_cudaStreamWaitEvent;

#endif // __cplusplus
#endif // !SEP_CUDA_AVAILABLE
#endif // SEP_COMPAT_CUDA_RUNTIME_H
