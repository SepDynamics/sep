#ifndef SEP_COMPAT_CUDA_UNIFIED_FIX_H
#define SEP_COMPAT_CUDA_UNIFIED_FIX_H

// Standard includes needed
#include <cstddef>  // For std::size_t
#include <cstring>  // For std::memcpy
#include <cerrno>   // For errno constants

// Configuration macro if not already defined
#ifndef SEP_ENGINE_HAS_CUDA
#define SEP_ENGINE_HAS_CUDA 0
#endif

// Include our headers first - this establishes the baseline types
#include "compat/cuda_types.h"

// *** CRITICAL: COMPATIBILITY SETTINGS FOR GCC 14 AND CUDA 12.9 ***
// These must be defined before any CUDA headers are included
#ifndef SEP_CUDACC_DISABLE_EXCEPTION_SPEC_CHECKS
#define SEP_CUDACC_DISABLE_EXCEPTION_SPEC_CHECKS 1
#endif

// Disable problematic fp8 and half precision conversions
#ifndef __CUDA_NO_FP8_CONVERSIONS__
#define __CUDA_NO_FP8_CONVERSIONS__ 1
#endif

#ifndef __CUDA_NO_HALF_OPERATORS__
#define __CUDA_NO_HALF_OPERATORS__ 1
#endif

#ifndef __CUDA_NO_HALF2_OPERATORS__
#define __CUDA_NO_HALF2_OPERATORS__ 1
#endif

#ifndef __CUDA_NO_BFLOAT16_CONVERSIONS__
#define __CUDA_NO_BFLOAT16_CONVERSIONS__ 1
#endif

// Define CUDA device/host function qualifiers
#if defined(__CUDACC__)
#ifndef SEP_HOST
#define SEP_HOST __host__
#endif
#ifndef SEP_DEVICE
#define SEP_DEVICE __device__
#endif
#ifndef SEP_HD
#define SEP_HD __host__ __device__
#endif
#else
#ifndef SEP_HOST
#define SEP_HOST
#endif
#ifndef SEP_DEVICE
#define SEP_DEVICE
#endif
#ifndef SEP_HD
#define SEP_HD
#endif
#endif

// Define float math constants
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// Include the real CUDA runtime only when compiling with CUDA compiler
#if defined(__CUDACC__) && SEP_ENGINE_HAS_CUDA
#include <cuda_runtime.h>
#endif

// Math function declarations and implementations only when needed
#ifdef __cplusplus
extern "C" {
// Forward declarations of math functions with noexcept to match system headers
SEP_HD double sinpi(double x) noexcept;
SEP_HD double cospi(double x) noexcept;
SEP_HD float sinpif(float x) noexcept;
SEP_HD float cospif(float x) noexcept;
}
#endif

namespace sep {
namespace cuda {

// Create a uniform interface for CUDA functions whether CUDA is available or not
#if SEP_ENGINE_HAS_CUDA
// Use the real CUDA functions when available
inline cudaError_t deviceSynchronize() {
    return ::cudaDeviceSynchronize();
}

inline cudaError_t memcpyToDevice(void* dst, const void* src, std::size_t size) {
    return ::cudaMemcpy(dst, src, size, cudaMemcpyHostToDevice);
}

inline cudaError_t memcpyFromDevice(void* dst, const void* src, std::size_t size) {
    return ::cudaMemcpy(dst, src, size, cudaMemcpyDeviceToHost);
}

#else
// Provide stub implementations when CUDA is not available
inline cudaError_t deviceSynchronize() {
    return (cudaError_t)SEP_CUDA_SUCCESS;
}

inline cudaError_t memcpyToDevice(void* dst, const void* src, std::size_t size) {
    if (dst && src && size > 0) {
        std::memcpy(dst, src, size);
    }
    return (cudaError_t)SEP_CUDA_SUCCESS;
}

inline cudaError_t memcpyFromDevice(void* dst, const void* src, std::size_t size) {
    if (dst && src && size > 0) {
        std::memcpy(dst, src, size);
    }
    return (cudaError_t)SEP_CUDA_SUCCESS;
}
#endif

} // namespace cuda
} // namespace sep

#endif // SEP_COMPAT_CUDA_UNIFIED_FIX_H
