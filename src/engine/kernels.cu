#include <cuda_runtime.h>

#include "kernels.h"

// Forward declarations for kernels implemented in quantum_kernels.cu
__global__ void qbsa_kernel(const std::uint32_t* d_probe_indices,
                           const std::uint32_t* d_expectations,
                           std::uint32_t num_probes,
                           std::uint32_t* d_bitfield,
                           std::uint32_t* d_corrections,
                           std::uint32_t* d_correction_count);

__global__ void qsh_kernel(const std::uint64_t* d_chunks,
                          std::uint32_t num_chunks,
                          std::uint32_t* d_collapse_indices,
                          std::uint32_t* d_collapse_counts);

cudaError_t launchQBSAKernel(const std::uint32_t *d_probe_indices,
                           const std::uint32_t *d_expectations, std::uint32_t num_probes,
                           std::uint32_t *d_bitfield, std::uint32_t *d_corrections,
                           std::uint32_t *d_correction_count, cudaStream_t stream) {
    const std::uint32_t block_size = 256;
    const std::uint32_t grid_size = (num_probes + block_size - 1) / block_size;
    qbsa_kernel<<<grid_size, block_size, 0, stream>>>(d_probe_indices, d_expectations, num_probes, d_bitfield, d_corrections, d_correction_count);
    return cudaGetLastError();
}

cudaError_t launchQSHKernel(const std::uint64_t *d_chunks,
                            std::uint32_t num_chunks,
                            std::uint32_t *d_collapse_indices,
                            std::uint32_t *d_collapse_counts,
                            cudaStream_t stream) {
    const std::uint32_t block_size = 256;
    const std::uint32_t grid_size = (num_chunks + block_size - 1) / block_size;
    qsh_kernel<<<grid_size, block_size, 0, stream>>>(d_chunks, num_chunks, d_collapse_indices, d_collapse_counts);
    return cudaGetLastError();
}
