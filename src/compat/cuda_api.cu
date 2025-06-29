#include "compat/cuda_common.h"

#include "compat/cuda_unified_fix.h"

// CUDA runtime must come first
#ifndef SEP_CUDACC_DISABLE_EXCEPTION_SPEC_CHECKS
#define SEP_CUDACC_DISABLE_EXCEPTION_SPEC_CHECKS 1
#endif
#include "compat/cuda_common.h"
#include "compat/cuda_helpers.h"
#include "compat/cuda_runtime.h"  // for sep::cuda::cudaMemcpyAsync

// Standard library includes - only for host compilation
#if !defined(__CUDACC__)
#include <cstddef>
#include <cstdint>
#include <memory>
#include <utility>
#endif

// Project includes
#include "compat/constants.h"
#include "compat/cuda_wrapper.h"
#include "compat/kernels.h"
#include "compat/macros.h"

// Add missing includes
#include "compat/raii.h"
#include "compat/types.h"
#include "compat/kernels.h"


extern "C" {
#if !defined(__CUDACC__)

// Global state
namespace {
using StreamPtr = std::unique_ptr<sep::cuda::StreamRAII>;
}  // namespace

static StreamPtr g_stream;
static bool g_initialized = false;

int sep_cuda_init(int device_id) {
    if (g_initialized) {
        return 0;
    }

    if (device_id >= 0) {
        sep::cuda::CudaWrapper::getInstance().setDevice(device_id);
    }

    g_stream = std::make_unique<sep::cuda::StreamRAII>();
    if (!g_stream || !g_stream->valid()) {
        g_stream.reset();
        return static_cast<int>(sep::ErrorCode::GeneralError);
    }

    g_initialized = true;
    return 0;
}

int sep_cuda_cleanup(void) {
    if (!g_initialized) {
        return 0;
    }

    g_stream.reset();
    g_initialized = false;
    return 0;
}

int sep_cuda_process_batch(const std::uint32_t* probe_indices, const std::uint32_t* expectations,
                           std::uint32_t num_probes, std::uint32_t* bitfield, std::uint32_t* correction_indices,
                           std::uint32_t* correction_count) {
    if (!g_initialized) {
        return static_cast<int>(sep::ErrorCode::GeneralError);
    }

    // Get constants
    const uint32_t bitfield_words = constants::get_bitfield_words();
    const uint32_t max_block_size = constants::get_max_block_size();
    
    // Pre-calculate all sizes
    const size_t probe_size = num_probes * sizeof(std::uint32_t);
    const size_t bitfield_size = bitfield_words * sizeof(std::uint32_t);
    const size_t corrections_size = max_block_size * sizeof(std::uint32_t);
    const size_t count_size = sizeof(std::uint32_t);

    // Device buffers managed via RAII
    sep::cuda::DeviceBufferRAII<std::uint32_t> d_probe_indices(num_probes);
    sep::cuda::DeviceBufferRAII<std::uint32_t> d_expectations(num_probes);
    sep::cuda::DeviceBufferRAII<std::uint32_t> d_bitfield(bitfield_words);
    sep::cuda::DeviceBufferRAII<std::uint32_t> d_corrections(max_block_size);
    sep::cuda::DeviceBufferRAII<std::uint32_t> d_correction_count(1);

    if (!d_probe_indices.valid() || !d_expectations.valid() || !d_bitfield.valid() || !d_corrections.valid() ||
        !d_correction_count.valid()) {
        return static_cast<int>(sep::ErrorCode::GeneralError);
    }

    try {
        // Ensure proper stream synchronization before memory operations
        CUDA_CHECK(cudaStreamSynchronize(g_stream->get()));

        // Memory transfers with error checking
        CUDA_CHECK(
            sep::cuda::cudaMemcpyAsync(d_probe_indices.get(), probe_indices, probe_size, cudaMemcpyHostToDevice, g_stream->get()));
        CUDA_CHECK(cudaStreamSynchronize(g_stream->get()));

        CUDA_CHECK(
            sep::cuda::cudaMemcpyAsync(d_expectations.get(), expectations, probe_size, cudaMemcpyHostToDevice, g_stream->get()));
        CUDA_CHECK(cudaStreamSynchronize(g_stream->get()));

        CUDA_CHECK(cudaMemsetAsync(d_correction_count.get(), 0, count_size, g_stream->get()));
        CUDA_CHECK(cudaStreamSynchronize(g_stream->get()));

        CUDA_CHECK(sep::cuda::launchQBSAKernel(d_probe_indices.get(), d_expectations.get(), num_probes,
                                               d_bitfield.get(), d_corrections.get(), d_correction_count.get(),
                                               g_stream->get()));

        CUDA_CHECK(sep::cuda::cudaMemcpyAsync(bitfield, d_bitfield.get(), bitfield_size, cudaMemcpyDeviceToHost, g_stream->get()));

        CUDA_CHECK(sep::cuda::cudaMemcpyAsync(correction_indices, d_corrections.get(), corrections_size, cudaMemcpyDeviceToHost,
                                   g_stream->get()));

        CUDA_CHECK(sep::cuda::cudaMemcpyAsync(correction_count, d_correction_count.get(), count_size, cudaMemcpyDeviceToHost,
                                   g_stream->get()));

        CUDA_CHECK(cudaStreamSynchronize(g_stream->get()));
    } catch (const sep::CudaException&) {
        return static_cast<int>(sep::ErrorCode::GeneralError);
    }

    return 0;
}

int sep_cuda_process_symmetry(const std::uint64_t* chunks, std::uint32_t num_chunks, std::uint32_t* collapse_indices,
                              std::uint32_t* collapse_counts) {
    if (!g_initialized) {
        return static_cast<int>(sep::ErrorCode::GeneralError);
    }

    // Get constants
    const uint32_t symmetry_pairs = constants::get_symmetry_pairs();

    // Pre-calculate sizes
    const size_t chunks_size = num_chunks * sizeof(std::uint64_t);
    const size_t indices_size = num_chunks * symmetry_pairs * sizeof(std::uint32_t);
    const size_t counts_size = num_chunks * sizeof(std::uint32_t);

    // Device buffers managed via RAII
    sep::cuda::DeviceBufferRAII<std::uint64_t> d_chunks(num_chunks);
    sep::cuda::DeviceBufferRAII<std::uint32_t> d_collapse_indices(num_chunks * symmetry_pairs);
    sep::cuda::DeviceBufferRAII<std::uint32_t> d_collapse_counts(num_chunks);

    if (!d_chunks.valid() || !d_collapse_indices.valid() || !d_collapse_counts.valid()) {
        return static_cast<int>(sep::ErrorCode::GeneralError);
    }

    try {
        // Ensure proper stream synchronization before memory operations
        CUDA_CHECK(cudaStreamSynchronize(g_stream->get()));

        // Memory transfer with error checking
        CUDA_CHECK(sep::cuda::cudaMemcpyAsync(d_chunks.get(), chunks, chunks_size, cudaMemcpyHostToDevice, g_stream->get()));
        CUDA_CHECK(cudaStreamSynchronize(g_stream->get()));

        CUDA_CHECK(sep::cuda::launchQSHKernel(d_chunks.get(), num_chunks, d_collapse_indices.get(),
                                              d_collapse_counts.get(), g_stream->get()));

        CUDA_CHECK(sep::cuda::cudaMemcpyAsync(collapse_indices, d_collapse_indices.get(), indices_size, cudaMemcpyDeviceToHost,
                                   g_stream->get()));

        CUDA_CHECK(sep::cuda::cudaMemcpyAsync(collapse_counts, d_collapse_counts.get(), counts_size, cudaMemcpyDeviceToHost,
                                   g_stream->get()));

        CUDA_CHECK(cudaStreamSynchronize(g_stream->get()));
    } catch (const sep::CudaException&) {
        return static_cast<int>(sep::ErrorCode::GeneralError);
    }

    return 0;
}

#endif
}  // extern "C"
