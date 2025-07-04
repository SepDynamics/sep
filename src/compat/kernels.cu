#include "compat/kernels.h"
#include "compat/constants.h"
#include "compat/cuda_helpers.h"
#include <cstring> // For std::memcpy (used implicitly by atomic operations or by the device itself)
#include "compat/cuda_unified_fix.h"

#ifdef __CUDACC__
#include <cuda_runtime.h>
#endif

namespace sep::cuda {
namespace detail {

// Derivative cascade helper for QSH analysis
__device__ static uint32_t derivativeCascade(uint64_t input, uint32_t depth) {
    uint32_t result = 0;
    for (uint32_t i = 0; i < depth; ++i) {
        uint32_t low = static_cast<uint32_t>(input);
        uint32_t high = static_cast<uint32_t>(input >> 32);
        uint32_t xorv = low ^ high;
        result ^= xorv;
        input = static_cast<uint64_t>(xorv) | (static_cast<uint64_t>(xorv) << 32);
    }
    return result;
}

// --- QBSA Kernel -----------------------------------------------------------
__global__ void qbsa_kernel(const uint32_t* d_probe_indices,
                            const uint32_t* d_expectations,
                            uint32_t num_probes,
                            uint32_t* d_bitfield,
                            uint32_t* d_corrections,
                            uint32_t* d_correction_count) {
    const uint32_t tid = blockIdx.x * blockDim.x + threadIdx.x;
    if (tid >= num_probes) return;

    const uint32_t bit_index = d_probe_indices[tid];
    const uint32_t expected  = d_expectations[tid];

    const uint32_t word_idx  = bit_index / 32u;
    const uint32_t bit_pos   = bit_index % 32u;
    const uint32_t mask      = 1u << bit_pos;

    const uint32_t current = atomicOr(&d_bitfield[word_idx], 0);
    const uint32_t current_bit = (current & mask) ? 1u : 0u;

    if (current_bit != expected) {
        atomicXor(&d_bitfield[word_idx], mask);
        const uint32_t idx = atomicAdd(d_correction_count, 1u);
        d_corrections[idx] = bit_index;
    }
}

// --- QSH Kernel ------------------------------------------------------------
__global__ void qsh_kernel(const uint64_t* d_chunks,
                           uint32_t num_chunks,
                           uint32_t* d_collapse_indices,
                           uint32_t* d_collapse_counts) {
    const uint32_t tid = blockIdx.x * blockDim.x + threadIdx.x;
    if (tid >= num_chunks) return;

    const uint64_t chunk = d_chunks[tid];
    const uint64_t reversed = __brevll(chunk);
    const uint64_t diff = chunk ^ reversed;

    uint32_t count = 0;
    uint32_t mask = static_cast<uint32_t>(~diff & 0xFFFFFFFFull);
    const uint32_t base = tid * 32u;

    while (mask) {
        uint32_t i = __ffs(mask) - 1u;
        d_collapse_indices[base + count] = i;
        mask &= mask - 1u;
        ++count;
    }

    d_collapse_counts[tid] = count;
}

} // namespace detail

} // namespace sep::cuda
