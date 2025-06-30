#pragma once

#include <cstddef>
#include <cstdint>
#include "compat/cuda_runtime.h"

namespace sep::cuda {

// Async memory copy operations
cudaError_t cudaMemcpyAsync(void* dst, const void* src, size_t count,
                           cudaMemcpyKind kind, cudaStream_t stream);

cudaError_t cudaMemcpyAsync(void* dst, const void* src, size_t count,
                           int kind, void* stream);

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

int sep_cuda_init(int device_id);
int sep_cuda_cleanup(void);

int sep_cuda_process_batch(const std::uint32_t* probe_indices, const std::uint32_t* expectations,
                          std::uint32_t num_probes, std::uint32_t* bitfield, std::uint32_t* correction_indices,
                          std::uint32_t* correction_count);

int sep_cuda_process_symmetry(const std::uint64_t* chunks, std::uint32_t num_chunks,
                            std::uint32_t* collapse_indices, std::uint32_t* collapse_counts);

} // extern "C"