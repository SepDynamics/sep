#ifndef SEP_COMPAT_CUDA_WRAPPERS_H
#define SEP_COMPAT_CUDA_WRAPPERS_H

// This header is deprecated - all CUDA wrapper functions are now in cuda_runtime.h
// Include cuda_runtime.h instead
#include "compat/cuda_runtime.h"

// Define additional error constants that might be needed
namespace sep {
namespace cuda {

#if !SEP_ENGINE_HAS_CUDA
// Define additional error constants needed by some code
static const cudaError_t cudaErrorNotReady = 34;
#endif

}  // namespace cuda
}  // namespace sep

#endif // SEP_COMPAT_CUDA_WRAPPERS_H