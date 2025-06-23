#ifndef CUDA_COMMON_H
#define CUDA_COMMON_H

// Include CUDA runtime when available, otherwise use stub definitions
#include "compat/macros.h"
#if SEP_CUDA_AVAILABLE
#include "compat/cuda_runtime.h"
#else
#include "compat/cuda_impl.h"
#endif

// Forward declaration for compat/cuda_helpers.h functionality
// These are defined in compat/cuda_helpers.h
namespace sep::cuda {
#if SEP_CUDA_AVAILABLE
void logCudaError(const char* operation, cudaError_t error);
#else
// The stub implementation reuses sep::cuda::cudaError_t directly rather than
// relying on the internal cuda_stub_constants namespace.  This avoids build
// failures when the stub constants header does not provide a matching type.
void logCudaError(const char* operation, cudaError_t error);
#endif
}  // namespace sep::cuda


#endif  // CUDA_COMMON_H
