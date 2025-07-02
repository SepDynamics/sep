#include "compat/cuda_api.hpp"
#include "core/common.h"
#include "compat/constants.h"
#ifdef SEP_USE_CUDA
#include "compat/kernels.h"
#include "compat/raii.h"
#include "compat/cuda_helpers.h"
#endif

extern "C" {

// When CUDA is unavailable we fall back to CPU implementations. When
// SEP_USE_CUDA is defined we dispatch to the actual GPU kernels in
// kernels.cu via the launch helpers.
namespace {
bool g_initialized = false;
#ifdef SEP_USE_CUDA
using StreamPtr = std::unique_ptr<sep::cuda::StreamRAII>;
static StreamPtr g_stream;
#endif

static inline std::uint64_t bit_reverse64(std::uint64_t v) {
    std::uint64_t r = 0;
    for (int i = 0; i < 64; ++i) {
        r = (r << 1) | ((v >> i) & 1ULL);
    }
    return r;
}

#ifndef SEP_USE_CUDA
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
#endif // SEP_USE_CUDA
}  // namespace

sep::SEPResult sep_cuda_init(int device_id) {
#ifdef SEP_USE_CUDA
    if (device_id >= 0) {
        sep::cuda::CudaWrapper::getInstance().setDevice(device_id);
    }
    g_stream = std::make_unique<sep::cuda::StreamRAII>();
    if (!g_stream || !g_stream->valid()) {
        g_stream.reset();
        return sep::SEPResult::UNKNOWN_ERROR;
    }
#endif
    g_initialized = true;
    return sep::SEPResult::SUCCESS;
}

sep::SEPResult sep_cuda_cleanup(void) {
#ifdef SEP_USE_CUDA
    g_stream.reset();
#endif
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

#ifdef SEP_USE_CUDA
    const uint32_t bitfield_words = sep::cuda::constants::get_bitfield_words();
    const uint32_t max_block_size = sep::cuda::constants::get_max_block_size();

    const size_t probe_size = num_probes * sizeof(std::uint32_t);
    const size_t bitfield_size = bitfield_words * sizeof(std::uint32_t);
    const size_t corrections_size = max_block_size * sizeof(std::uint32_t);
    const size_t count_size = sizeof(std::uint32_t);

    sep::cuda::DeviceBufferRAII<std::uint32_t> d_probe_indices(num_probes);
    sep::cuda::DeviceBufferRAII<std::uint32_t> d_expectations(num_probes);
    sep::cuda::DeviceBufferRAII<std::uint32_t> d_bitfield(bitfield_words);
    sep::cuda::DeviceBufferRAII<std::uint32_t> d_corrections(max_block_size);
    sep::cuda::DeviceBufferRAII<std::uint32_t> d_correction_count(1);

    if (!d_probe_indices.valid() || !d_expectations.valid() ||
        !d_bitfield.valid() || !d_corrections.valid() ||
        !d_correction_count.valid()) {
        return sep::SEPResult::UNKNOWN_ERROR;
    }

    try {
        CUDA_CHECK(cudaStreamSynchronize(g_stream->get()));
        CUDA_CHECK(sep::cuda::cudaMemcpyAsync(d_probe_indices.get(), probe_indices,
                                              probe_size, cudaMemcpyHostToDevice,
                                              g_stream->get()));
        CUDA_CHECK(sep::cuda::cudaMemcpyAsync(d_expectations.get(), expectations,
                                              probe_size, cudaMemcpyHostToDevice,
                                              g_stream->get()));
        CUDA_CHECK(cudaMemsetAsync(d_correction_count.get(), 0, count_size,
                                   g_stream->get()));
        CUDA_CHECK(sep::cuda::launchQBSAKernel(d_probe_indices.get(),
                                               d_expectations.get(), num_probes,
                                               d_bitfield.get(),
                                               d_corrections.get(),
                                               d_correction_count.get(),
                                               g_stream->get()));
        CUDA_CHECK(sep::cuda::cudaMemcpyAsync(bitfield, d_bitfield.get(),
                                              bitfield_size,
                                              cudaMemcpyDeviceToHost,
                                              g_stream->get()));
        CUDA_CHECK(sep::cuda::cudaMemcpyAsync(correction_indices,
                                              d_corrections.get(),
                                              corrections_size,
                                              cudaMemcpyDeviceToHost,
                                              g_stream->get()));
        CUDA_CHECK(sep::cuda::cudaMemcpyAsync(correction_count,
                                              d_correction_count.get(), count_size,
                                              cudaMemcpyDeviceToHost,
                                              g_stream->get()));
        CUDA_CHECK(cudaStreamSynchronize(g_stream->get()));
    } catch (...) {
        return sep::SEPResult::UNKNOWN_ERROR;
    }

    return sep::SEPResult::SUCCESS;
#else
    return cpu_process_batch(probe_indices, expectations, num_probes, bitfield,
                              correction_indices, correction_count);
#endif
}

sep::SEPResult sep_cuda_process_symmetry(const std::uint64_t* chunks,
                                         std::uint32_t num_chunks,
                                         std::uint32_t* collapse_indices,
                                         std::uint32_t* collapse_counts) {
    if (!g_initialized || !chunks || !collapse_indices || !collapse_counts) {
        return sep::SEPResult::INVALID_ARGUMENT;
    }

#ifdef SEP_USE_CUDA
    const uint32_t pairs = sep::cuda::constants::get_symmetry_pairs();

    const size_t chunks_size = num_chunks * sizeof(std::uint64_t);
    const size_t indices_size = num_chunks * pairs * sizeof(std::uint32_t);
    const size_t counts_size = num_chunks * sizeof(std::uint32_t);

    sep::cuda::DeviceBufferRAII<std::uint64_t> d_chunks(num_chunks);
    sep::cuda::DeviceBufferRAII<std::uint32_t> d_collapse_indices(num_chunks * pairs);
    sep::cuda::DeviceBufferRAII<std::uint32_t> d_collapse_counts(num_chunks);

    if (!d_chunks.valid() || !d_collapse_indices.valid() || !d_collapse_counts.valid()) {
        return sep::SEPResult::UNKNOWN_ERROR;
    }

    try {
        CUDA_CHECK(cudaStreamSynchronize(g_stream->get()));
        CUDA_CHECK(sep::cuda::cudaMemcpyAsync(d_chunks.get(), chunks, chunks_size,
                                              cudaMemcpyHostToDevice,
                                              g_stream->get()));
        CUDA_CHECK(sep::cuda::launchQSHKernel(d_chunks.get(), num_chunks,
                                              d_collapse_indices.get(),
                                              d_collapse_counts.get(),
                                              g_stream->get()));
        CUDA_CHECK(sep::cuda::cudaMemcpyAsync(collapse_indices,
                                              d_collapse_indices.get(), indices_size,
                                              cudaMemcpyDeviceToHost,
                                              g_stream->get()));
        CUDA_CHECK(sep::cuda::cudaMemcpyAsync(collapse_counts,
                                              d_collapse_counts.get(), counts_size,
                                              cudaMemcpyDeviceToHost,
                                              g_stream->get()));
        CUDA_CHECK(cudaStreamSynchronize(g_stream->get()));
    } catch (...) {
        return sep::SEPResult::UNKNOWN_ERROR;
    }

    return sep::SEPResult::SUCCESS;
#else
    return cpu_process_symmetry(chunks, num_chunks, collapse_indices,
                                collapse_counts);
#endif
}

}  // extern "C"
