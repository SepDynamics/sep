#include "compat/core.h"

#include <stdexcept>

#include "compat/cuda_common.h"
#include "compat/kernels.h"
#include "compat/macros.h"  // for SEP_CUDA_CHECK_NOTHROW
#include "compat/stream.h"  // for Stream::create()
#include "compat/cuda_helpers.h"

namespace sep::cuda {

CudaCore::CudaCore() = default;

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

StreamPtr CudaCore::createStream(sep::StreamFlags flags) {
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
                           DeviceMemory<std::uint32_t>& correction_count, Stream& stream) {
  cudaError_t result =
      launchQBSAKernel(probe_indices.get(), expectations.get(), num_probes, bitfield.get(),
                       corrections.get(), correction_count.get(), static_cast<cudaStream_t>(stream.handle()));
  return {result == cudaSuccess ? Status::Success : Status::Error,
          result != cudaSuccess ? cudaGetErrorString(result) : "", "",
          result == cudaSuccess ? sep::SEPResult::SUCCESS : sep::SEPResult::CUDA_ERROR};
}

Error CudaCore::launchQSH(const DeviceMemory<std::uint64_t>& chunks, std::uint32_t num_chunks,
                          DeviceMemory<std::uint32_t>& collapse_indices,
                          DeviceMemory<std::uint32_t>& collapse_counts, Stream& stream) {
  cudaError_t result = launchQSHKernel(chunks.get(), num_chunks, collapse_indices.get(),
                                       collapse_counts.get(), static_cast<cudaStream_t>(stream.handle()));
  return {result == cudaSuccess ? Status::Success : Status::Error,
          result != cudaSuccess ? cudaGetErrorString(result) : "", "",
          result == cudaSuccess ? sep::SEPResult::SUCCESS : sep::SEPResult::CUDA_ERROR};
}

Error CudaCore::launchSimilarity(const DeviceMemory<float>& similarity,
                                 const DeviceMemory<float>& emb_a, const DeviceMemory<float>& emb_b,
                                 std::uint32_t embedding_size, Stream& stream) {
  cudaError_t result = launchSimilarityKernel(const_cast<float*>(similarity.get()), emb_a.get(),
                                             emb_b.get(), embedding_size, static_cast<cudaStream_t>(stream.handle()));
  return {result == cudaSuccess ? Status::Success : Status::Error,
          result != cudaSuccess ? cudaGetErrorString(result) : "", "",
          result == cudaSuccess ? sep::SEPResult::SUCCESS : sep::SEPResult::CUDA_ERROR};
}

Error CudaCore::launchBlend(DeviceMemory<float>& output, const DeviceMemory<float>& embeddings,
                            const DeviceMemory<float>& weights, std::uint32_t num_contexts,
                            std::uint32_t embedding_size, Stream& stream) {
  cudaError_t result = launchBlendKernel(output.get(), embeddings.get(), weights.get(),
                                        num_contexts, embedding_size, static_cast<cudaStream_t>(stream.handle()));
  return {result == cudaSuccess ? Status::Success : Status::Error,
          result != cudaSuccess ? cudaGetErrorString(result) : "", "",
          result == cudaSuccess ? sep::SEPResult::SUCCESS : sep::SEPResult::CUDA_ERROR};
}

}  // namespace sep::cuda
