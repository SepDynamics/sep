/*
 * Copyright (c) 2025 SEP Engine Contributors
 *
 * Implementation of CUDA functions in the sep::cuda namespace
 */

#include <cuda_runtime.h>

#include <string>
#include <vector>

#include "engine/internal/cuda_types.hpp"
#include "engine/internal/standard_includes.h"
#include "error_handler.h"
#include "standard_includes.h"

namespace sep::cuda {

struct CudaCore::Impl {
    bool initialized_ = false;
    int current_device_ = -1;
    std::vector<cudaDeviceProp> device_properties_;
    CudaMetrics current_metrics_;
};

CudaCore::CudaCore() : impl_(new Impl) {}
CudaCore::~CudaCore() = default;

CudaCore& CudaCore::instance() {
    static CudaCore inst;
    return inst;
}

Error CudaCore::initialize(int device_id) {
    cudaError_t err = ::cudaSetDevice(device_id);
    if (err != cudaSuccess) {
        return Error(sep::SEPResult::CUDA_ERROR, cudaGetErrorString(err));
    }

    int device_count = 0;
    err = ::cudaGetDeviceCount(&device_count);
    if (err != cudaSuccess) {
        return Error(sep::SEPResult::CUDA_ERROR, cudaGetErrorString(err));
    }

    impl_->device_properties_.resize(device_count);
    for (int i = 0; i < device_count; ++i) {
        err = ::cudaGetDeviceProperties(&impl_->device_properties_[i], i);
        if (err != cudaSuccess) {
            return Error(sep::SEPResult::CUDA_ERROR, cudaGetErrorString(err));
        }
    }

    impl_->initialized_ = true;
    impl_->current_device_ = device_id;
    return Error();
}

bool CudaCore::is_initialized() const {
    return impl_->initialized_;
}

Error CudaCore::setDevice(int device) {
    cudaError_t err = ::cudaSetDevice(device);
    if (err != cudaSuccess) {
        return Error(sep::SEPResult::CUDA_ERROR, cudaGetErrorString(err));
    }
    impl_->current_device_ = device;
    return Error();
}

int CudaCore::getDeviceCount() const {
    int count = 0;
    ::cudaGetDeviceCount(&count);
    return count;
}

Error CudaCore::getDeviceProperties(cudaDeviceProp& props, int device) const {
    cudaError_t err = ::cudaGetDeviceProperties(&props, device);
    if (err != cudaSuccess) {
        return Error(sep::SEPResult::CUDA_ERROR, cudaGetErrorString(err));
    }
    return Error();
}

Error CudaCore::getMemoryInfo(size_t& free, size_t& total) const {
    cudaError_t err = ::cudaMemGetInfo(&free, &total);
    if (err != cudaSuccess) {
        return Error(sep::SEPResult::CUDA_ERROR, cudaGetErrorString(err));
    }
    return Error();
}

Error CudaCore::getLastError() const {
    cudaError_t err = ::cudaGetLastError();
    if (err != cudaSuccess) {
        return Error(sep::SEPResult::CUDA_ERROR, cudaGetErrorString(err));
    }
    return Error();
}

std::string CudaCore::getErrorString(cudaError_t error) const { return ::cudaGetErrorString(error); }

CudaMetrics CudaCore::getMetrics() const {
    return impl_->current_metrics_;
}

Error CudaCore::updateMetrics() {
    size_t free_mem, total_mem;
    cudaError_t err = ::cudaMemGetInfo(&free_mem, &total_mem);
    if (err != cudaSuccess) {
        return Error(sep::SEPResult::CUDA_ERROR, cudaGetErrorString(err));
    }

    impl_->current_metrics_.total_memory = total_mem;
    impl_->current_metrics_.used_memory = total_mem - free_mem;
    impl_->current_metrics_.memory_utilization =
        (total_mem > 0) ? static_cast<float>(impl_->current_metrics_.used_memory) / total_mem : 0.0f;

    // GPU utilization would require NVML or similar API
    impl_->current_metrics_.gpu_utilization = 0.0f;

    return Error();
}

}  // namespace sep::cuda
