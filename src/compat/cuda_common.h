#ifndef SEP_COMPAT_CUDA_COMMON_H
#define SEP_COMPAT_CUDA_COMMON_H

#if SEP_ENGINE_HAS_CUDA
// GLM configuration before CUDA includes
#ifndef GLM_FORCE_CUDA
#define GLM_FORCE_CUDA
#endif

#ifndef GLM_CUDA_VERSION_CHECK
#define GLM_CUDA_VERSION_CHECK 90  // Require CUDA 9.0+
#endif

#ifndef GLM_COMPILER
#define GLM_COMPILER GLM_COMPILER_GCC
#endif

#ifndef GLM_FORCE_CXX17
#define GLM_FORCE_CXX17
#endif
#endif // SEP_ENGINE_HAS_CUDA

#include "compat/cuda_runtime.h"  // Always rely on the shim header

// Include additional definitions after types are available
#include "compat/cuda_defs.h"

// Forward declarations of helper functions
namespace sep::cuda {
void logCudaError(const char* operation, cudaError_t error);
}  // namespace sep::cuda

#endif // SEP_COMPAT_CUDA_COMMON_H
