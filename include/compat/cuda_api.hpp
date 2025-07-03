#pragma once

// Include C API definitions first
#include "api/bridge.h"  // For SEP_API
#include "core/common.h"  // For sep::SEPResult
#include <cuda_runtime.h>  // For CUDA types
#include <cstddef>  // For size_t
#include <cstdint>  // For fixed-width integers

#ifdef __cplusplus
extern "C" {
#endif

// Core CUDA operations
SEP_API sep::SEPResult sep_cuda_init(int device_id);
SEP_API sep::SEPResult sep_cuda_cleanup(void);

// Batch processing operations
SEP_API sep::SEPResult sep_cuda_process_batch(
    const std::uint32_t* probe_indices,
    const std::uint32_t* expectations,
    std::uint32_t num_probes,
    std::uint32_t* bitfield,
    std::uint32_t* correction_indices,
    std::uint32_t* correction_count
);

SEP_API sep::SEPResult sep_cuda_process_symmetry(
    const std::uint64_t* chunks,
    std::uint32_t num_chunks,
    std::uint32_t* collapse_indices,
    std::uint32_t* collapse_counts
);

// Memory management functions
SEP_API cudaError_t sep_cuda_allocate_managed(void** ptr, size_t size);
SEP_API cudaError_t sep_cuda_deallocate(void* ptr);
SEP_API cudaError_t sep_cuda_memcpy_async(
    void* dst,
    const void* src,
    size_t count,
    cudaMemcpyKind kind,
    void* stream
);

#ifdef __cplusplus
}

// C++ specific includes and definitions
#include "quantum/data.hpp"

namespace sep::cuda {

// Memory allocation/deallocation
cudaError_t allocateManaged(void** ptr, size_t size);
cudaError_t deallocate(void* ptr);

// Async memory copy operations
cudaError_t cudaMemcpyAsync(void* dst, const void* src, size_t count,
                           cudaMemcpyKind kind, cudaStream_t stream);

// Overload taking raw stream pointer for backward compatibility
cudaError_t cudaMemcpyAsync(void* dst, const void* src, size_t count,
                           cudaMemcpyKind kind, void* stream);

// CUDA kernel launch functions
cudaError_t launchQBSAKernel(const std::uint32_t *d_probe_indices,
                           const std::uint32_t *d_expectations, std::uint32_t num_probes,
                           std::uint32_t *d_bitfield, std::uint32_t *d_corrections,
                           std::uint32_t *d_correction_count, cudaStream_t stream);

cudaError_t launchQSHKernel(const std::uint64_t *d_chunks,
                           std::uint32_t num_chunks,
                           std::uint32_t *d_collapse_indices,
                           std::uint32_t *d_collapse_counts,
                           cudaStream_t stream);

// Pattern processing functions
cudaError_t launch_pattern_processing(pattern::PatternData* patterns,
                                    pattern::PatternData* results,
                                    const pattern::PatternConfig& config,
                                    size_t pattern_count,
                                    const pattern::PatternData* previous_patterns,
                                    cudaStream_t stream);

} // namespace sep::cuda
#endif // __cplusplus
