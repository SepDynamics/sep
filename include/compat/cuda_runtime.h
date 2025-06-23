// Include standard headers needed for types
#include <stddef.h>  // For size_t

// Define SEP_CUDA_AVAILABLE if not already defined
#ifndef SEP_CUDA_AVAILABLE
#define SEP_CUDA_AVAILABLE 0
#endif

// Include necessary headers
#include <cstring> // For std::memcpy

// Forward declare types to avoid circular dependency with cuda_impl.h
#if !SEP_CUDA_AVAILABLE
namespace cuda_stub_constants {
typedef struct CUstream_st* cudaStream_t;
typedef struct CUevent_st* cudaEvent_t;

// Define cudaMemcpyKind enum
typedef enum {
    cudaMemcpyHostToHost = 0,
    cudaMemcpyHostToDevice = 1,
    cudaMemcpyDeviceToHost = 2,
    cudaMemcpyDeviceToDevice = 3,
    cudaMemcpyDefault = 4
} cudaMemcpyKind;
}
#endif

// We don't need to define these types here since they're already defined in cuda_impl.h
// This avoids the redefinition errors

// When CUDA is not available, define global functions for CUDA operations
#if !SEP_CUDA_AVAILABLE

// Import the CUDA types from cuda_impl.h
using cuda_stub_constants::cudaMemcpyKind;
using cuda_stub_constants::cudaMemcpyHostToHost;
using cuda_stub_constants::cudaMemcpyHostToDevice;
using cuda_stub_constants::cudaMemcpyDeviceToHost;
using cuda_stub_constants::cudaMemcpyDeviceToDevice;
using cuda_stub_constants::cudaMemcpyDefault;

namespace sep {
namespace cuda {
// Define cudaError_t enum
typedef enum {
    cudaSuccess = 0,
    cudaErrorMemoryAllocation = 1,
    cudaErrorInitializationError = 2,
    cudaErrorInvalidDevice = 3,
    cudaErrorDeviceUninitialized = 4,
    cudaErrorInvalidValue = 5,
    cudaErrorNotReady = 6,
    cudaErrorSetOnActiveProcess = 7,
    cudaErrorStreamCaptureUnsupported = 8,
    cudaErrorInvalidMemcpyDirection = 9,
    cudaErrorInvalidResourceHandle = 10
} cudaError_t;
}  // namespace cuda
}


// Only declare the _ptsz variants of CUDA functions
// These are the only ones that should be unique to this header
#if !SEP_CUDA_AVAILABLE

#ifdef __cplusplus
extern "C" {
#endif


#ifdef __cplusplus
}
#endif

#endif // !SEP_CUDA_AVAILABLE

// Define missing CUDA function aliases if needed
#if !defined(__CUDA_RUNTIME_H__) && !SEP_CUDA_AVAILABLE
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
#endif // !defined(__CUDA_RUNTIME_H__) && !SEP_CUDA_AVAILABLE

#endif // SEP_COMPAT_CUDA_RUNTIME_H
