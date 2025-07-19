#ifndef SEP_COMPAT_CUDA_FUNCTIONS_H
#define SEP_COMPAT_CUDA_FUNCTIONS_H

#include "compat/cuda_base.h"

namespace sep {
namespace cuda {

// Function declarations
cudaError_t cudaSetDevice(int device);
const char* cudaGetErrorString(cudaError_t error);
cudaError_t cudaStreamCreate(cudaStream_t* stream);
cudaError_t cudaMemcpyAsync(void* dst, const void* src, size_t count, cudaMemcpyKind kind, cudaStream_t stream);

} // namespace cuda
} // namespace sep

#endif // SEP_COMPAT_CUDA_FUNCTIONS_H