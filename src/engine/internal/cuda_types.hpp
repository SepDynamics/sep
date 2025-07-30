#ifndef SEP_ENGINE_CUDA_TYPES_HPP
#define SEP_ENGINE_CUDA_TYPES_HPP

#include <cuda_runtime.h>
#include <string>
#include <vector>
#include <memory>
#include "engine/internal/error_handler.h"

namespace sep::cuda {

struct CudaMetrics {
    size_t total_memory = 0;
    size_t used_memory = 0;
    float memory_utilization = 0.0f;
    float gpu_utilization = 0.0f;
};

class CudaCore {
public:
    struct Impl;
    CudaCore();
    ~CudaCore();
    static CudaCore& instance();

    Error initialize(int device_id);
    bool is_initialized() const;
    Error setDevice(int device);
    int getDeviceCount() const;
    Error getDeviceProperties(cudaDeviceProp& props, int device) const;
    Error getMemoryInfo(size_t& free, size_t& total) const;
    Error getLastError() const;
    std::string getErrorString(cudaError_t error) const;
    CudaMetrics getMetrics() const;
    Error updateMetrics();

private:
    std::unique_ptr<Impl> impl_;
};

}

#endif // SEP_ENGINE_CUDA_TYPES_HPP
