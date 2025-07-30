#include <cuda_runtime.h>

#include <cstdint>

#include "kernels.h"

namespace {

__device__ std::uint32_t derivativeCascade(std::uint64_t input, std::uint32_t cascade_depth) {
    std::uint32_t result = 0;
    for (std::uint32_t i = 0; i < cascade_depth; ++i) {
        std::uint32_t xor_result = static_cast<std::uint32_t>(input) ^ static_cast<std::uint32_t>(input >> 32);
        result ^= xor_result;
        input = static_cast<std::uint64_t>(xor_result) | (static_cast<std::uint64_t>(xor_result) << 32);
    }
    return result;
}

} // namespace

__global__ void qbsa_kernel(const std::uint32_t* d_probe_indices, const std::uint32_t* d_expectations, std::uint32_t num_probes,
                            std::uint32_t* d_bitfield, std::uint32_t* d_corrections, std::uint32_t* d_correction_count) {
    const std::uint32_t tid = blockIdx.x * blockDim.x + threadIdx.x;
    if (tid >= num_probes)
        return;

    const std::uint32_t bit_index = d_probe_indices[tid];
    const std::uint32_t expected = d_expectations[tid];

    const std::uint32_t word_idx = bit_index / 32;
    const std::uint32_t bit_pos = bit_index % 32;
    const std::uint32_t bit_mask = 1U << bit_pos;

    const std::uint32_t current = atomicOr(&d_bitfield[word_idx], 0);
    const std::uint32_t current_bit = (current & bit_mask) ? 1 : 0;

    if (current_bit != expected) {
        atomicXor(&d_bitfield[word_idx], bit_mask);
        const std::uint32_t correction_idx = atomicAdd(d_correction_count, 1);
        d_corrections[correction_idx] = bit_index;
    }
}

__global__ void qsh_kernel(const std::uint64_t* d_chunks, std::uint32_t num_chunks, std::uint32_t* d_collapse_indices,
                           std::uint32_t* d_collapse_counts) {
    const std::uint32_t tid = blockIdx.x * blockDim.x + threadIdx.x;
    if (tid >= num_chunks)
        return;

    const std::uint64_t chunk = d_chunks[tid];
    std::uint32_t collapse_count = 0;

    std::uint32_t cascade_result = derivativeCascade(chunk, 3);

    const std::uint64_t reversed = __brevll(chunk);
    const std::uint64_t diff = chunk ^ reversed;

    const std::uint32_t pairs = 32;
    const std::uint64_t pair_mask = (1ULL << pairs) - 1ULL;

    std::uint32_t mismatches = 0;
    std::uint32_t current_run = 0;
    std::uint32_t max_run = 0;
    for (std::uint32_t i = 0; i < pairs; ++i) {
        bool mis = (diff >> i) & 1ULL;
        if (mis) {
            mismatches++;
            current_run++;
            if (current_run > max_run)
                max_run = current_run;
        } else {
            current_run = 0;
        }
    }

    const float mismatch_ratio = static_cast<float>(mismatches) / static_cast<float>(pairs);

    float cascade_factor = static_cast<float>(__popc(cascade_result)) / 32.0f;
    float adjusted_threshold = 0.35f * (1.0f - 0.2f * cascade_factor);
    bool rupture = (mismatch_ratio > adjusted_threshold) && (max_run > 2);

    std::uint32_t match_mask = static_cast<std::uint32_t>(~diff & pair_mask);
    const std::uint32_t base_idx = tid * pairs;

    while (match_mask && collapse_count < pairs) {
        std::uint32_t i = __ffs(match_mask) - 1;
        d_collapse_indices[base_idx + collapse_count] = i;
        collapse_count++;
        match_mask &= match_mask - 1;
    }

    if (rupture && collapse_count < pairs) {
        const std::uint32_t base_idx = tid * pairs;
        d_collapse_indices[base_idx + collapse_count] = 0xFFFFFFFFU;
        collapse_count++;
    }

    d_collapse_counts[tid] = collapse_count;
}

__global__ void similarity_kernel(float* d_similarity, const float* d_emb_a, const float* d_emb_b,
                                  std::uint32_t embedding_size) {
    const std::uint32_t tid = blockIdx.x * blockDim.x + threadIdx.x;
    if (tid >= embedding_size)
        return;

    float dot_product = 0.0f;
    for (std::uint32_t i = 0; i < embedding_size; ++i) {
        dot_product += d_emb_a[i] * d_emb_b[i];
    }

    if (tid == 0) {
        *d_similarity = dot_product;
    }
}

__global__ void blend_kernel(float* d_output, const float* d_embeddings, const float* d_weights, std::uint32_t num_contexts,
                             std::uint32_t embedding_size) {
    const std::uint32_t tid = blockIdx.x * blockDim.x + threadIdx.x;
    if (tid >= embedding_size)
        return;

    float sum = 0.0f;
    for (std::uint32_t i = 0; i < num_contexts; ++i) {
        sum += d_embeddings[i * embedding_size + tid] * d_weights[i];
    }

    d_output[tid] = sum;
}
