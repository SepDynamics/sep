#pragma once

#include <cstddef>
#include <cstdint>
#include <cuda_runtime.h>
#include "core/common.h"  // for sep::SEPResult

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

} // namespace sep::cuda

// C API
extern "C" {

sep::SEPResult sep_cuda_init(int device_id);
sep::SEPResult sep_cuda_cleanup(void);

sep::SEPResult sep_cuda_process_batch(const std::uint32_t* probe_indices, const std::uint32_t* expectations,
                          std::uint32_t num_probes, std::uint32_t* bitfield, std::uint32_t* correction_indices,
                          std::uint32_t* correction_count);

sep::SEPResult sep_cuda_process_symmetry(const std::uint64_t* chunks, std::uint32_t num_chunks,
                            std::uint32_t* collapse_indices, std::uint32_t* collapse_counts);

} // extern "C"