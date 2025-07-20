#ifndef SEP_CUDA_CORE_H
#define SEP_CUDA_CORE_H

#include <cuda_runtime.h>

#include "error_handler.h"

namespace sep::cuda {

// Add metrics structure
struct CudaMetrics {
  float gpu_utilization{0.0f};
  float memory_utilization{0.0f};
  size_t total_memory{0};
  size_t used_memory{0};
};

class CudaCore {
 public:
  static CudaCore& instance();

  // Initialization
  Error initialize(int device_id = 0);
  bool is_initialized() const;

  // Device management
  Error setDevice(int device);
  int getDeviceCount() const;
  Error getDeviceProperties(cudaDeviceProp& props, int device) const;

  // Memory management
  Error getMemoryInfo(size_t& free, size_t& total) const;
  Error getLastError() const;
  shim::string getErrorString(cudaError_t error) const;

  // Metrics
  CudaMetrics getMetrics() const;
  Error updateMetrics();

 private:
  CudaCore();
  ~CudaCore();

  CudaCore(const CudaCore&) = delete;
  CudaCore& operator=(const CudaCore&) = delete;

  struct Impl;
  std::unique_ptr<Impl> impl_;
};

}  // namespace sep::cuda

#endif  // SEP_CUDA_CORE_H
