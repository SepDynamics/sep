#include <cuda_runtime.h>

#include <cstddef>
#include <cstdint>
#include <memory>
#include <mutex>

#include "common.h"
#include "kernels.h"
#include "raii.h"

extern "C" {

// Global state
namespace {
using StreamPtr = std::unique_ptr<sep::cuda::StreamRAII>;
static std::mutex g_stream_mutex;
}  // namespace

static StreamPtr g_stream;
static bool g_initialized = false;

sep::SEPResult sep_cuda_init(int device_id)
{
    std::lock_guard<std::mutex> lock(g_stream_mutex);
    if (g_initialized) {
        return sep::SEPResult::SUCCESS;
    }

    if (device_id >= 0) {
        cudaSetDevice(device_id);
    }

    g_stream = std::make_unique<sep::cuda::StreamRAII>();
    if (!g_stream || !g_stream->valid()) {
        g_stream.reset();
        return sep::SEPResult::UNKNOWN_ERROR;
    }

    g_initialized = true;
    return sep::SEPResult::SUCCESS;
}

sep::SEPResult sep_cuda_cleanup(void)
{
    if (!g_initialized) {
        return sep::SEPResult::SUCCESS;
    }

    std::lock_guard<std::mutex> lock(g_stream_mutex);
    g_stream.reset();
    g_initialized = false;
    return sep::SEPResult::SUCCESS;
}

sep::SEPResult sep_cuda_process_batch(const std::uint32_t* probe_indices,
                                      const std::uint32_t* expectations, std::uint32_t num_probes,
                                      std::uint32_t* bitfield, std::uint32_t* correction_indices,
                                      std::uint32_t* correction_count)
{
    std::lock_guard<std::mutex> lock(g_stream_mutex);
    if (!g_initialized || !g_stream || !g_stream->valid()) {
        return sep::SEPResult::UNKNOWN_ERROR;
    }

    const size_t probe_size = num_probes * sizeof(std::uint32_t);
    const size_t bitfield_size = 1024 * sizeof(std::uint32_t);
    const size_t corrections_size = 1024 * sizeof(std::uint32_t);
    const size_t count_size = sizeof(std::uint32_t);

    sep::cuda::DeviceBufferRAII<std::uint32_t> d_probe_indices(num_probes);
    sep::cuda::DeviceBufferRAII<std::uint32_t> d_expectations(num_probes);
    sep::cuda::DeviceBufferRAII<std::uint32_t> d_bitfield(1024);
    sep::cuda::DeviceBufferRAII<std::uint32_t> d_corrections(1024);
    sep::cuda::DeviceBufferRAII<std::uint32_t> d_correction_count(1);

    if (!d_probe_indices.valid() || !d_expectations.valid() || !d_bitfield.valid() || !d_corrections.valid() ||
        !d_correction_count.valid()) {
        return sep::SEPResult::UNKNOWN_ERROR;
    }

    cudaStreamSynchronize(g_stream->get());
    cudaMemcpyAsync(d_probe_indices.get(), probe_indices, probe_size, cudaMemcpyHostToDevice, g_stream->get());
    cudaStreamSynchronize(g_stream->get());
    cudaMemcpyAsync(d_expectations.get(), expectations, probe_size, cudaMemcpyHostToDevice, g_stream->get());
    cudaStreamSynchronize(g_stream->get());
    cudaMemsetAsync(d_correction_count.get(), 0, count_size, g_stream->get());
    cudaStreamSynchronize(g_stream->get());

    launchQBSAKernel(d_probe_indices.get(), d_expectations.get(), num_probes,
                     d_bitfield.get(), d_corrections.get(), d_correction_count.get(),
                     g_stream->get());

    cudaMemcpyAsync(bitfield, d_bitfield.get(), bitfield_size, cudaMemcpyDeviceToHost, g_stream->get());
    cudaMemcpyAsync(correction_indices, d_corrections.get(), corrections_size, cudaMemcpyDeviceToHost,
                                  g_stream->get());
    cudaMemcpyAsync(correction_count, d_correction_count.get(), count_size, cudaMemcpyDeviceToHost,
                                  g_stream->get());
    cudaStreamSynchronize(g_stream->get());

    return sep::SEPResult::SUCCESS;
}

sep::SEPResult sep_cuda_process_symmetry(const std::uint64_t* chunks, std::uint32_t num_chunks,
                                         std::uint32_t* collapse_indices,
                                         std::uint32_t* collapse_counts)
{
    std::lock_guard<std::mutex> lock(g_stream_mutex);
    if (!g_initialized || !g_stream || !g_stream->valid()) {
        return sep::SEPResult::UNKNOWN_ERROR;
    }

    const size_t chunks_size = num_chunks * sizeof(std::uint64_t);
    const size_t indices_size = num_chunks * 32 * sizeof(std::uint32_t);
    const size_t counts_size = num_chunks * sizeof(std::uint32_t);

    sep::cuda::DeviceBufferRAII<std::uint64_t> d_chunks(num_chunks);
    sep::cuda::DeviceBufferRAII<std::uint32_t> d_collapse_indices(num_chunks * 32);
    sep::cuda::DeviceBufferRAII<std::uint32_t> d_collapse_counts(num_chunks);

    if (!d_chunks.valid() || !d_collapse_indices.valid() || !d_collapse_counts.valid()) {
        return sep::SEPResult::UNKNOWN_ERROR;
    }

    cudaStreamSynchronize(g_stream->get());
    cudaMemcpyAsync(d_chunks.get(), chunks, chunks_size, cudaMemcpyHostToDevice, g_stream->get());
    cudaStreamSynchronize(g_stream->get());

    launchQSHKernel(d_chunks.get(), num_chunks, d_collapse_indices.get(),
                    d_collapse_counts.get(), g_stream->get());

    cudaMemcpyAsync(collapse_indices, d_collapse_indices.get(), indices_size, cudaMemcpyDeviceToHost,
                                   g_stream->get());
    cudaMemcpyAsync(collapse_counts, d_collapse_counts.get(), counts_size, cudaMemcpyDeviceToHost,
                                   g_stream->get());
    cudaStreamSynchronize(g_stream->get());

    return sep::SEPResult::SUCCESS;
}

cudaError_t sep_cuda_allocate_managed(void** ptr, size_t size) {
    return cudaMallocManaged(ptr, size);
}

cudaError_t sep_cuda_deallocate(void* ptr) {
    return cudaFree(ptr);
}

cudaError_t sep_cuda_memcpy_async(void* dst, const void* src, size_t count,
                                 cudaMemcpyKind kind, void* stream) {
   return cudaMemcpyAsync(dst, src, count, kind, reinterpret_cast<cudaStream_t>(stream));
}

} // extern "C"
