#include "kernels.h"

#include <cuda_runtime.h>

__global__ void qbsa_kernel(const std::uint32_t* d_probe_indices,
                            const std::uint32_t* d_expectations,
                            std::uint32_t num_probes,
                            std::uint32_t* d_bitfield,
                            std::uint32_t* d_corrections,
                            std::uint32_t* d_correction_count) {
    const std::uint32_t tid = blockIdx.x * blockDim.x + threadIdx.x;
    if (tid >= num_probes) return;

    const std::uint32_t bit_index = d_probe_indices[tid];
    const std::uint32_t expected  = d_expectations[tid];

    const std::uint32_t word_idx  = bit_index / 32u;
    const std::uint32_t bit_pos   = bit_index % 32u;
    const std::uint32_t mask      = 1u << bit_pos;

    const std::uint32_t current = atomicOr(&d_bitfield[word_idx], 0);
    const std::uint32_t current_bit = (current & mask) ? 1u : 0u;

    if (current_bit != expected) {
        atomicXor(&d_bitfield[word_idx], mask);
        const std::uint32_t idx = atomicAdd(d_correction_count, 1u);
        d_corrections[idx] = bit_index;
    }
}

__global__ void qsh_kernel(const std::uint64_t* d_chunks,
                           std::uint32_t num_chunks,
                           std::uint32_t* d_collapse_indices,
                           std::uint32_t* d_collapse_counts) {
    const std::uint32_t tid = blockIdx.x * blockDim.x + threadIdx.x;
    if (tid >= num_chunks) return;

    const std::uint64_t chunk = d_chunks[tid];
    const std::uint64_t reversed = __brevll(chunk);
    const std::uint64_t diff = chunk ^ reversed;

    std::uint32_t count = 0;
    std::uint32_t mask = static_cast<std::uint32_t>(~diff & 0xFFFFFFFFull);
    const std::uint32_t base = tid * 32u;

    while (mask) {
        std::uint32_t i = __ffs(mask) - 1u;
        d_collapse_indices[base + count] = i;
        mask &= mask - 1u;
        ++count;
    }

    d_collapse_counts[tid] = count;
}

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
