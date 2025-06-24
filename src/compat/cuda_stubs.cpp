#if !SEP_CUDA_AVAILABLE
#include "compat/cuda_runtime.h"
#include <cstring>
#include <cstdlib>

namespace sep { namespace cuda {

cudaError_t cudaMallocManaged(void** ptr, size_t size) {
    if (!ptr) return cudaErrorInvalidValue;
    *ptr = std::malloc(size);
    return *ptr ? cudaSuccess : cudaErrorMemoryAllocation;
}

cudaError_t cudaFree(void* ptr) {
    std::free(ptr);
    return cudaSuccess;
}

cudaError_t cudaMemcpy(void* dst, const void* src, size_t size, int kind) {
    (void)kind;
    if (dst && src) std::memcpy(dst, src, size);
    return cudaSuccess;
}

cudaError_t cudaMemcpyAsync(void* dst, const void* src, size_t size, int kind, cudaStream_t stream) {
    (void)stream;
    return cudaMemcpy(dst, src, size, kind);
}

} } // namespace sep::cuda
#endif
