#ifndef SEP_USE_CUDA
#include "compat/cuda_api.hpp"
#include "core/common.h"
#include "compat/constants.h"

extern "C" {

// CPU implementations used when CUDA is unavailable. Separating the logic
// allows the public API wrappers below to remain small and to potentially
// dispatch to real GPU kernels in the future.
namespace {
bool g_initialized = false;

static inline std::uint64_t bit_reverse64(std::uint64_t v) {
    std::uint64_t r = 0;
    for (int i = 0; i < 64; ++i) {
        r = (r << 1) | ((v >> i) & 1ULL);
    }
    return r;
}

sep::SEPResult cpu_process_batch(const std::uint32_t* probe_indices,
                                 const std::uint32_t* expectations,
                                 std::uint32_t num_probes,
                                 std::uint32_t* bitfield,
                                 std::uint32_t* correction_indices,
                                 std::uint32_t* correction_count) {
    std::uint32_t count = 0;
    for (std::uint32_t i = 0; i < num_probes; ++i) {
        std::uint32_t bit_index = probe_indices[i];
        std::uint32_t expected  = expectations[i];

        std::uint32_t word_idx = bit_index / 32u;
        std::uint32_t bit_pos  = bit_index % 32u;
        std::uint32_t mask     = 1u << bit_pos;
        std::uint32_t current  = (bitfield[word_idx] >> bit_pos) & 1u;

        if (current != expected) {
            bitfield[word_idx] ^= mask;
            correction_indices[count] = bit_index;
            ++count;
        }
    }

    *correction_count = count;
    return sep::SEPResult::SUCCESS;
}

sep::SEPResult cpu_process_symmetry(const std::uint64_t* chunks,
                                    std::uint32_t num_chunks,
                                    std::uint32_t* collapse_indices,
                                    std::uint32_t* collapse_counts) {
    constexpr std::uint32_t pairs = sep::cuda::constants::SYMMETRY_PAIRS;

    for (std::uint32_t c = 0; c < num_chunks; ++c) {
        std::uint64_t chunk      = chunks[c];
        std::uint64_t reversed   = bit_reverse64(chunk);
        std::uint64_t diff       = chunk ^ reversed;
        std::uint32_t match_mask = static_cast<std::uint32_t>(~diff) &
                                   ((1u << pairs) - 1u);

        std::uint32_t base  = c * pairs;
        std::uint32_t count = 0;
        while (match_mask && count < pairs) {
            std::uint32_t i = __builtin_ctz(match_mask);
            collapse_indices[base + count] = i;
            ++count;
            match_mask &= match_mask - 1u;
        }
        collapse_counts[c] = count;
    }

    return sep::SEPResult::SUCCESS;
}
}  // namespace

sep::SEPResult sep_cuda_init(int /*device_id*/) {
    g_initialized = true;
    return sep::SEPResult::SUCCESS;
}

sep::SEPResult sep_cuda_cleanup(void) {
    g_initialized = false;
    return sep::SEPResult::SUCCESS;
}

sep::SEPResult sep_cuda_process_batch(const std::uint32_t* probe_indices,
                                      const std::uint32_t* expectations,
                                      std::uint32_t num_probes,
                                      std::uint32_t* bitfield,
                                      std::uint32_t* correction_indices,
                                      std::uint32_t* correction_count) {
    if (!g_initialized || !probe_indices || !expectations || !bitfield ||
        !correction_indices || !correction_count) {
        return sep::SEPResult::INVALID_ARGUMENT;
    }

    return cpu_process_batch(probe_indices, expectations, num_probes, bitfield,
                              correction_indices, correction_count);
}

sep::SEPResult sep_cuda_process_symmetry(const std::uint64_t* chunks,
                                         std::uint32_t num_chunks,
                                         std::uint32_t* collapse_indices,
                                         std::uint32_t* collapse_counts) {
    if (!g_initialized || !chunks || !collapse_indices || !collapse_counts) {
        return sep::SEPResult::INVALID_ARGUMENT;
    }

    return cpu_process_symmetry(chunks, num_chunks, collapse_indices,
                                collapse_counts);
}

}  // extern "C"
#endif  // SEP_USE_CUDA
