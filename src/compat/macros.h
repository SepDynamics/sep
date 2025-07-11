#pragma once

// -----------------------------------------------------------------------------
// CUDA availability detection
// -----------------------------------------------------------------------------
// Determine if the CUDA runtime headers are reachable and/or we are compiling
// device code.  Some headers previously duplicated this logic which led to
// inconsistent checks.

#ifndef SEP_CUDA_AVAILABLE
#if defined(__CUDACC__) || defined(SEP_USE_CUDA)
#define SEP_CUDA_AVAILABLE 1
#else
#define SEP_CUDA_AVAILABLE 0
#endif
#endif

// -----------------------------------------------------------------------------
// spdlog availability
// -----------------------------------------------------------------------------
#ifndef SEP_HAS_SPDLOG
#define SEP_HAS_SPDLOG 1
#endif

// -----------------------------------------------------------------------------
// Cycles renderer availability - always enabled
// -----------------------------------------------------------------------------
#ifndef SEP_HAS_CYCLES
#define SEP_HAS_CYCLES 1
#endif

// -----------------------------------------------------------------------------
// Audio integration availability - always enabled
// -----------------------------------------------------------------------------
#ifndef SEP_HAS_AUDIO
#define SEP_HAS_AUDIO 1
#endif

// Backwards compatibility for legacy macros used across the code base.
#ifndef SEP_HAS_CUDA_RUNTIME
#define SEP_HAS_CUDA_RUNTIME SEP_CUDA_AVAILABLE
#endif
#ifndef SEP_CUDA_HAS_RUNTIME
#define SEP_CUDA_HAS_RUNTIME SEP_CUDA_AVAILABLE
#endif

// CUDA macros for consistent host/device/global function qualifiers
// Function qualifiers - use these instead of CUDA's __host__, __device__, etc.

#if defined(__CUDACC__)
#ifndef SEP_HOST
#define SEP_HOST __host__
#endif
#ifndef SEP_DEVICE
#define SEP_DEVICE __device__
#endif
#ifndef SEP_GLOBAL
#define SEP_GLOBAL __global__
#endif
#ifndef SEP_SHARED
#define SEP_SHARED __shared__
#endif
#ifndef SEP_CONSTANT
#define SEP_CONSTANT __constant__
#endif

// Combined host/device function qualifier
#ifndef SEP_HD
#define SEP_HD __host__ __device__
#endif

// CUDA block size constants
#define MAX_BLOCK_SIZE 1024

// Inlining directive (forced inline for CUDA)
#ifndef SEP_FORCEINLINE
#define SEP_FORCEINLINE __forceinline__
#endif
#ifndef SEP_INLINE
#define SEP_INLINE SEP_FORCEINLINE
#endif
#else
#ifndef SEP_HOST
#define SEP_HOST
#endif
#ifndef SEP_DEVICE
#define SEP_DEVICE
#endif
#ifndef SEP_GLOBAL
#define SEP_GLOBAL
#endif
#ifndef SEP_SHARED
#define SEP_SHARED
#endif

// Combined host/device function qualifier
#ifndef SEP_HD
#define SEP_HD
#endif

// Inlining directive
#ifndef SEP_FORCEINLINE
#define SEP_FORCEINLINE inline
#endif
#ifndef SEP_INLINE
#define SEP_INLINE SEP_FORCEINLINE
#endif
#endif

// CUDA error checking macros
// Only define when the CUDA runtime is available so cudaGetErrorString can be
// referenced.  When compiling device code (__CUDACC__) these are also active.
#if SEP_CUDA_AVAILABLE
#if !defined(SEP_CUDA_CHECK)
#define SEP_CUDA_CHECK(call)                                                                                 \
    do {                                                                                                     \
        cudaError_t err = call;                                                                              \
        if (err != cudaSuccess) {                                                                            \
            (void)fprintf(stderr, "CUDA error at %s:%d: %s\n", __FILE__, __LINE__, cudaGetErrorString(err)); \
            exit(EXIT_FAILURE);                                                                              \
        }                                                                                                    \
    } while (0)
#endif

#if !defined(SEP_CUDA_CHECK_NOTHROW)
#define SEP_CUDA_CHECK_NOTHROW(call)                                                                         \
    do {                                                                                                     \
        cudaError_t err = call;                                                                              \
        if (err != cudaSuccess) {                                                                            \
            (void)fprintf(stderr, "CUDA error at %s:%d: %s\n", __FILE__, __LINE__, cudaGetErrorString(err)); \
        }                                                                                                    \
    } while (0)
#endif

// Thread indexing macros for CUDA kernels
#define SEP_THREAD_IDX (threadIdx.x + blockIdx.x * blockDim.x)
#define SEP_BLOCK_IDX blockIdx.x
#define SEP_THREAD_COUNT (blockDim.x * gridDim.x)

// Memory fence macros
#define SEP_MEMORY_FENCE __threadfence()
#define SEP_BLOCK_FENCE __syncthreads()
#else
#if !defined(SEP_CUDA_CHECK)
#define SEP_CUDA_CHECK(call) ((void)(call))
#endif
#if !defined(SEP_CUDA_CHECK_NOTHROW)
#define SEP_CUDA_CHECK_NOTHROW(call) ((void)(call))
#endif
#define SEP_THREAD_IDX 0
#define SEP_BLOCK_IDX 0
#define SEP_THREAD_COUNT 1
#define SEP_MEMORY_FENCE \
    do {                 \
    } while (0)
#define SEP_BLOCK_FENCE \
    do {                \
    } while (0)
#endif
