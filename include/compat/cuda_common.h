#ifndef CUDA_COMMON_H
#define CUDA_COMMON_H

#include "compat/macros.h"
#if SEP_CUDA_AVAILABLE
#include <cuda_runtime.h>
#include "compat/cuda_helpers.h"
#else
#include "compat/cuda_impl.h"
#endif

// Forward declaration for compat/cuda_helpers.h functionality
// These are defined in compat/cuda_helpers.h
namespace sep::cuda {
#if SEP_CUDA_AVAILABLE
void logCudaError(const char* operation, cudaError_t error);
#else
// The stub implementation shares the same cudaError_t type defined in
// compat/cuda_impl.h, so keep the interface consistent regardless of build
// mode. Using the fully qualified name from the stub namespace caused build
// failures because the type alias lives outside that namespace.
void logCudaError(const char* operation, cudaError_t error);
#endif
}  // namespace sep::cuda


#endif  // CUDA_COMMON_H
