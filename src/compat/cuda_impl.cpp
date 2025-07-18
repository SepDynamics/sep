/*
 * Copyright (c) 2025 SEP Engine Contributors
 *
 * Implementation of CUDA functions in the sep::cuda namespace
 */

#include <cuda.h>
#include <cuda_runtime.h>

#include "compat/core.h"
#include "compat/cuda_unified.h"
#include "core/common.h"
#include "core/error_handler.h"

using sep::Error;
using sep::shim::string;

namespace sep::cuda
{

    Error CudaCore::initialize(int device)
    {
        // Basic CUDA device initialization
        cudaError_t cuda_err = cudaSetDevice(device);
        if (cuda_err != cudaSuccess)
        {
            return Error(Status::Error, string(::cudaGetErrorString(cuda_err)),
                         string("CudaCore::initialize"), SEPResult::CUDA_ERROR);
        }
        
        // Initialize device properties
        Error err = queryDeviceProperties();
        if (err.code != SEPResult::SUCCESS)
        {
            return err;
        }
        
        initialized_ = true;
        current_device_ = device;
        
        return Error(cuda::Status::Success, string("CUDA initialized successfully"),
                    string("CudaCore::initialize"), SEPResult::SUCCESS);
    }

    Error CudaCore::setDevice(int device)
    {
        cudaError_t cuda_err = cudaSetDevice(device);
        if (cuda_err != cudaSuccess)
        {
            return Error(Status::Error, string(::cudaGetErrorString(cuda_err)),
                         string("CudaCore::setDevice"), SEPResult::CUDA_ERROR);
        }
        current_device_ = device;
        return Error(cuda::Status::Success, string("Device set successfully"),
                    string("CudaCore::setDevice"), SEPResult::SUCCESS);
    }

    int CudaCore::getDeviceCount() const
    {
        int count = 0;
        cudaError_t cuda_err = cudaGetDeviceCount(&count);
        if (cuda_err != cudaSuccess)
        {
            return 0;
        }
        return count;
    }

    Error CudaCore::getDeviceProperties(cudaDeviceProp& props, int device) const
    {
        cudaError_t cuda_err = cudaGetDeviceProperties(&props, device);
        if (cuda_err != cudaSuccess)
        {
            return Error(Status::Error, string(::cudaGetErrorString(cuda_err)),
                         string("CudaCore::getDeviceProperties"), SEPResult::CUDA_ERROR);
        }
        return Error(cuda::Status::Success, string("Device properties retrieved"),
                    string("CudaCore::getDeviceProperties"), SEPResult::SUCCESS);
    }

    Error CudaCore::getMemoryInfo(size_t& free, size_t& total) const
    {
        cudaError_t cuda_err = cudaMemGetInfo(&free, &total);
        if (cuda_err != cudaSuccess)
        {
            return Error(Status::Error, string(::cudaGetErrorString(cuda_err)),
                         string("CudaCore::getMemoryInfo"), SEPResult::CUDA_ERROR);
        }
        return Error(cuda::Status::Success, string("Memory info retrieved"),
                    string("CudaCore::getMemoryInfo"), SEPResult::SUCCESS);
    }

    Error CudaCore::getLastError() const
    {
        cudaError_t cuda_err = cudaGetLastError();
        if (cuda_err != cudaSuccess)
        {
            return Error(Status::Error, string(::cudaGetErrorString(cuda_err)),
                         string("CudaCore::getLastError"), SEPResult::CUDA_ERROR);
        }
        return Error(cuda::Status::Success, string("No error"),
                    string("CudaCore::getLastError"), SEPResult::SUCCESS);
    }

    std::string CudaCore::getErrorString(cudaError_t error) const
    {
        return std::string(::cudaGetErrorString(error));
    }

    Error CudaCore::queryDeviceProperties()
    {
        int device_count = getDeviceCount();
        if (device_count == 0)
        {
            return Error(Status::Error, string("No CUDA devices found"),
                         string("CudaCore::queryDeviceProperties"), SEPResult::CUDA_ERROR);
        }

        device_properties_.clear();
        device_properties_.reserve(device_count);

        for (int i = 0; i < device_count; ++i)
        {
            cudaDeviceProp props;
            cudaError_t cuda_err = cudaGetDeviceProperties(&props, i);
            if (cuda_err != cudaSuccess)
            {
                return Error(Status::Error, string(::cudaGetErrorString(cuda_err)),
                             string("CudaCore::queryDeviceProperties"), SEPResult::CUDA_ERROR);
            }
            device_properties_.push_back(props);
        }

        return Error(cuda::Status::Success, string("Device properties queried"),
                    string("CudaCore::queryDeviceProperties"), SEPResult::SUCCESS);
    }

    CudaMetrics CudaCore::getMetrics() const
    {
        return current_metrics_;
    }

    Error CudaCore::updateMetrics()
    {
        size_t free_mem, total_mem;
        Error err = getMemoryInfo(free_mem, total_mem);
        if (err.code != SEPResult::SUCCESS)
        {
            return err;
        }

        current_metrics_.total_memory = total_mem;
        current_metrics_.used_memory = total_mem - free_mem;
        current_metrics_.memory_utilization = 
            (total_mem > 0) ? static_cast<float>(current_metrics_.used_memory) / total_mem : 0.0f;

        // GPU utilization would require NVML or similar API
        // For now, we'll leave it at 0
        current_metrics_.gpu_utilization = 0.0f;

        return Error(Status::Success, string("Metrics updated"),
                     string("CudaCore::updateMetrics"), SEPResult::SUCCESS);
    }

}  // namespace sep::cuda
