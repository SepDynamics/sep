#include "compat/core.h"

#include "compat/cuda_common.h"
#include "compat/kernels.h"
#include "compat/constants.h"
#include "compat/raii.h"

#include "core/common.h"  // defines sep::SEPResult

#include "compat/cuda_helpers.h"

namespace sep::cuda {

CudaCore& CudaCore::instance() {
    static CudaCore instance;
    return instance;
}

Error CudaCore::initialize(int device_id) {
  if (initialized_) {
    return {Status::Success, "", "", sep::SEPResult::SUCCESS};
  }

  Error error = initializeDevice(device_id);
  if (error.status != Status::Success) {
    return error;
  }

  error = queryDeviceProperties();
  if (error.status != Status::Success) {
    return error;
  }

  error = updateMetrics();
  if (error.status != Status::Success) {
    return error;
  }

  initialized_ = true;
  return {Status::Success, "", "", sep::SEPResult::SUCCESS};
}

Error CudaCore::setDevice(int device) {
  if (device < 0 || device >= getDeviceCount()) {
    return {Status::Error, "Invalid device ID", "", sep::SEPResult::INVALID_ARGUMENT};
  }

  CUDA_CHECK(cudaSetDevice(device));

  current_device_ = device;
  return {Status::Success, "", "", sep::SEPResult::SUCCESS};
}

int CudaCore::getDeviceCount() const {
  int count = 0;
  SEP_CUDA_CHECK_NOTHROW(cudaGetDeviceCount(&count));
  return count;
}

Error CudaCore::getDeviceProperties(cudaDeviceProp& props, int device) const {
  if (device < 0 || device >= getDeviceCount()) {
    return {Status::Error, "Invalid device ID", "", sep::SEPResult::INVALID_ARGUMENT};
  }

  CUDA_CHECK(cudaGetDeviceProperties(&props, device));

  return {Status::Success, "", "", sep::SEPResult::SUCCESS};
}

std::shared_ptr<Stream> CudaCore::createStream(sep::StreamFlags flags) {
  return Stream::create(flags);
}

Error CudaCore::destroyStream(cudaStream_t stream) {
  if (!stream) {
    return {Status::Success, "", "", sep::SEPResult::SUCCESS};
  }

  CUDA_CHECK(cudaStreamDestroy(stream));

  return {Status::Success, "", "", sep::SEPResult::SUCCESS};
}

Error CudaCore::synchronizeStream(cudaStream_t stream) {
  if (!stream) {
    return {Status::Success, "", "", sep::SEPResult::SUCCESS};
  }

  CUDA_CHECK(cudaStreamSynchronize(stream));

  return {Status::Success, "", "", sep::SEPResult::SUCCESS};
}

Error CudaCore::synchronizeStream(void* stream) {
  return synchronizeStream(reinterpret_cast<cudaStream_t>(stream));
}

cudaError_t launchQBSAKernel(const std::uint32_t* d_probe_indices,
                           const std::uint32_t* d_expectations, std::uint32_t num_probes,
                           std::uint32_t* d_bitfield, std::uint32_t* d_corrections,
                           std::uint32_t* d_correction_count, cudaStream_t stream) {
  if (!d_probe_indices || !d_expectations || !d_bitfield || !d_corrections || !d_correction_count) {
    return cudaErrorInvalidValue;
  }

  try {
    const uint32_t block_size = sep::cuda::constants::get_default_block_size();
    const uint32_t grid_size = (num_probes + block_size - 1) / block_size;

    detail::qbsa_kernel<<<grid_size, block_size, 0, stream>>>(
        d_probe_indices, d_expectations, num_probes, d_bitfield, d_corrections,
        d_correction_count);

    return cudaGetLastError();
  } catch (...) {
    return cudaErrorUnknown;
  }
}

cudaError_t launchQSHKernel(const std::uint64_t* d_chunks,
                          std::uint32_t num_chunks,
                          std::uint32_t* d_collapse_indices,
                          std::uint32_t* d_collapse_counts,
                          cudaStream_t stream) {
  if (!d_chunks || !d_collapse_indices || !d_collapse_counts) {
    return cudaErrorInvalidValue;
  }

  try {
    const uint32_t block_size = sep::cuda::constants::get_default_block_size();
    const uint32_t grid_size = (num_chunks + block_size - 1) / block_size;

    detail::qsh_kernel<<<grid_size, block_size, 0, stream>>>(
        d_chunks, num_chunks, d_collapse_indices, d_collapse_counts);

    return cudaGetLastError();
  } catch (...) {
    return cudaErrorUnknown;
  }
}

Error CudaCore::getMemoryInfo(size_t& free, size_t& total) const {
  CUDA_CHECK(cudaMemGetInfo(&free, &total));

  return {Status::Success, "", "", sep::SEPResult::SUCCESS};
}

Error CudaCore::getLastError() const {
  cudaError_t error = cudaGetLastError();
  return {error == cudaSuccess ? Status::Success : Status::Error,
          error != cudaSuccess ? cudaGetErrorString(error) : "", "",
          error == cudaSuccess ? sep::SEPResult::SUCCESS : sep::SEPResult::CUDA_ERROR};
}

std::string CudaCore::getErrorString(cudaError_t error) const { return cudaGetErrorString(error); }

CudaMetrics CudaCore::getMetrics() const { return current_metrics_; }

// Implementation removed - using the helper-based implementation below instead

Error CudaCore::updateMetrics() {
  size_t free_memory = 0;
  size_t total_memory = 0;

  Error error = getMemoryInfo(free_memory, total_memory);
  if (error.status != Status::Success) {
    return error;
  }

  current_metrics_.total_memory = total_memory;
  current_metrics_.used_memory = total_memory - free_memory;
  current_metrics_.memory_utilization =
      static_cast<float>(current_metrics_.used_memory) / total_memory;

  // Update other metrics if we have a valid device
  if (current_device_ >= 0 && current_device_ < static_cast<int>(device_properties_.size())) {
    // Note: Would need nvml for actual GPU utilization
    current_metrics_.gpu_utilization = 0.0f;
    current_metrics_.active_kernels = 0;
  }

  return {Status::Success, "", "", sep::SEPResult::SUCCESS};
}

Error CudaCore::initializeDevice(int device) {
  if (device < 0) {
    return {Status::Error, "Invalid device ID", "", sep::SEPResult::INVALID_ARGUMENT};
  }

  CUDA_CHECK(cudaSetDevice(device));

  current_device_ = device;
  return {Status::Success, "", "", sep::SEPResult::SUCCESS};
}

Error CudaCore::queryDeviceProperties() {
  int count = getDeviceCount();
  if (count <= 0) {
    return {Status::Error, "No CUDA devices found", "", sep::SEPResult::INVALID_ARGUMENT};
  }

  device_properties_.resize(count);
  for (int i = 0; i < count; ++i) {
    CUDA_CHECK(cudaGetDeviceProperties(&device_properties_[i], i));
  }

  return {Status::Success, "", "", sep::SEPResult::SUCCESS};
}

Error CudaCore::launchQBSA(const DeviceMemory<std::uint32_t>& probe_indices,
                           const DeviceMemory<std::uint32_t>& expectations,
                           std::uint32_t num_probes, DeviceMemory<std::uint32_t>& bitfield,
                           DeviceMemory<std::uint32_t>& corrections,
                           DeviceMemory<std::uint32_t>& correction_count, Stream& stream)
{ 
  cudaError_t result =
      launchQBSAKernel(probe_indices.get(), expectations.get(), num_probes, bitfield.get(),
                      corrections.get(), correction_count.get(), reinterpret_cast<cudaStream_t>(stream.handle()));
  return {result == cudaSuccess ? Status::Success : Status::Error,
          result != cudaSuccess ? cudaGetErrorString(result) : "", "",
          result == cudaSuccess ? sep::SEPResult::SUCCESS : sep::SEPResult::CUDA_ERROR};
}

Error CudaCore::launchQSH(const DeviceMemory<std::uint64_t>& chunks, std::uint32_t num_chunks,
                          DeviceMemory<std::uint32_t>& collapse_indices,
                          DeviceMemory<std::uint32_t>& collapse_counts, Stream& stream)
{ 
  cudaError_t result = launchQSHKernel(chunks.get(), num_chunks, collapse_indices.get(),
                                       collapse_counts.get(), reinterpret_cast<cudaStream_t>(stream.handle()));
  return {result == cudaSuccess ? Status::Success : Status::Error,
          result != cudaSuccess ? cudaGetErrorString(result) : "", "",
          result == cudaSuccess ? sep::SEPResult::SUCCESS : sep::SEPResult::CUDA_ERROR};
}

Error CudaCore::launchSimilarity(const DeviceMemory<float>& similarity,
                                 const DeviceMemory<float>& emb_a, const DeviceMemory<float>& emb_b,
                                 std::uint32_t embedding_size, Stream& stream)
{ 
  cudaError_t result = launchSimilarityKernel(const_cast<float*>(similarity.get()), emb_a.get(),
                                            emb_b.get(), embedding_size, reinterpret_cast<cudaStream_t>(stream.handle()));
  return {result == cudaSuccess ? Status::Success : Status::Error,
          result != cudaSuccess ? cudaGetErrorString(result) : "", "",
          result == cudaSuccess ? sep::SEPResult::SUCCESS : sep::SEPResult::CUDA_ERROR};
}

Error CudaCore::launchBlend(DeviceMemory<float>& output, const DeviceMemory<float>& embeddings,
                            const DeviceMemory<float>& weights, std::uint32_t num_contexts,
                            std::uint32_t embedding_size, Stream& stream)
{ 
  cudaError_t result = launchBlendKernel(output.get(), embeddings.get(), weights.get(),
                                       num_contexts, embedding_size, reinterpret_cast<cudaStream_t>(stream.handle()));
  return {result == cudaSuccess ? Status::Success : Status::Error,
          result != cudaSuccess ? cudaGetErrorString(result) : "", "",
          result == cudaSuccess ? sep::SEPResult::SUCCESS : sep::SEPResult::CUDA_ERROR};
}

}  // namespace sep::cuda
