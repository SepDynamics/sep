#ifndef SEP_COMPAT_CUDA_CONFIG_H
#define SEP_COMPAT_CUDA_CONFIG_H

// Basic configuration flags for CUDA support
#ifndef SEP_ENGINE_HAS_CUDA
#define SEP_ENGINE_HAS_CUDA 0
#endif

// For backward compatibility
#ifndef SEP_CUDA_AVAILABLE
#define SEP_CUDA_AVAILABLE SEP_ENGINE_HAS_CUDA
#endif

// CUDA function attributes
#if defined(__CUDACC__)
#define SEP_HOST __host__
#define SEP_DEVICE __device__
#define SEP_HD __host__ __device__
#define SEP_CUDA_EXPORT __host__
#else
#define SEP_HOST
#define SEP_DEVICE
#define SEP_HD
#define SEP_CUDA_EXPORT
#endif

#endif // SEP_COMPAT_CUDA_CONFIG_H