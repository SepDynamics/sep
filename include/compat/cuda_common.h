#ifndef CUDA_COMMON_H
#define CUDA_COMMON_H

// Include CUDA runtime or provide shims first
#if SEP_CUDA_AVAILABLE
#include <cuda_runtime.h>
#else
#include "compat/cuda_runtime.h"  // Contains shim definitions
#endif

// GLM configuration after CUDA runtime
#ifndef GLM_FORCE_CUDA
#define GLM_FORCE_CUDA
#endif

#ifndef GLM_CUDA_VERSION_CHECK
#define GLM_CUDA_VERSION_CHECK 90  // Require CUDA 9.0+
#endif

// Only set GLM compiler if not already defined
#ifndef GLM_COMPILER
#define GLM_COMPILER GLM_COMPILER_GCC
#endif

#ifndef GLM_FORCE_CXX17
#define GLM_FORCE_CXX17
#endif

#include "compat/cuda_defs.h"

namespace sep::cuda {

// Forward declarations of helper functions
void logCudaError(const char* operation, cudaError_t error);

}  // namespace sep::cuda

#endif  // CUDA_COMMON_H
