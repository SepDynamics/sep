#ifndef SEP_CUDA_CORE_H
#define SEP_CUDA_CORE_H

#include <vector>
#include "compat/cuda_common.h"
#include "compat/cuda_helpers.h"
#include "compat/types.h"
#include "compat/memory.h"
#include "compat/stream.h"

namespace sep::cuda {

// Add metrics structure
struct CudaMetrics {
  float gpu_utilization{0.0f};
  float memory_utilization{0.0f};
  size_t total_memory{0};
  size_t used_memory{0};
  uint32_t active_streams{0};
  uint32_t active_kernels{0};
};

class CudaCore {
 public:
  static CudaCore& instance() {
    static CudaCore instance;
    return instance;
  }

  // Initialization
  Error initialize(int device_id = 0);
  bool is_initialized() const { return initialized_; }

  // Device management
  Error setDevice(int device);
  int getDeviceCount() const;
  Error getDeviceProperties(cudaDeviceProp& props, int device) const;

  // Stream management
  StreamPtr createStream(::sep::StreamFlags flags = ::sep::StreamFlags::Default);
  Error destroyStream(cudaStream_t stream);
  Error synchronizeStream(cudaStream_t stream);

  // Memory management
  template <typename T>
  DeviceMemory<T> allocateMemory(size_t count) {
    return DeviceMemory<T>(count);
  }

  Error getMemoryInfo(size_t& free, size_t& total) const;
  Error getLastError() const;
  std::string getErrorString(cudaError_t error) const;

  // Metrics
  CudaMetrics getMetrics() const;
  Error updateMetrics();

  // Kernel launches
  Error launchQBSA(const DeviceMemory<std::uint32_t>& probe_indices,
                   const DeviceMemory<std::uint32_t>& expectations, std::uint32_t num_probes,
                   DeviceMemory<std::uint32_t>& bitfield, DeviceMemory<std::uint32_t>& corrections,
                   DeviceMemory<std::uint32_t>& correction_count, Stream& stream);

  Error launchQSH(const DeviceMemory<std::uint64_t>& chunks, std::uint32_t num_chunks,
                  DeviceMemory<std::uint32_t>& collapse_indices,
                  DeviceMemory<std::uint32_t>& collapse_counts, Stream& stream);

  Error launchSimilarity(const DeviceMemory<float>& similarity, const DeviceMemory<float>& emb_a,
                         const DeviceMemory<float>& emb_b, std::uint32_t embedding_size,
                         Stream& stream);

 Error launchBlend(DeviceMemory<float>& output, const DeviceMemory<float>& embeddings,
                    const DeviceMemory<float>& weights, std::uint32_t num_contexts,
                    std::uint32_t embedding_size, Stream& stream);

 private:
  CudaCore() : initialized_(false), current_device_(-1) {}
  ~CudaCore() = default;  // Private destructor

  CudaCore(const CudaCore&) = delete;
  CudaCore& operator=(const CudaCore&) = delete;

  bool initialized_{false};
  int current_device_{-1};
  std::vector<cudaDeviceProp> device_properties_;
  CudaMetrics current_metrics_;

  Error initializeDevice(int device);
  Error queryDeviceProperties();
};


}  // namespace sep::cuda

#endif  // SEP_CUDA_CORE_H
