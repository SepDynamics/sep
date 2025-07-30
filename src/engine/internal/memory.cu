#include "memory.h"
#include <cuda_runtime.h>
#include <stdexcept>

namespace sep {
namespace cuda {

template <typename T>
DeviceMemory<T>::DeviceMemory(size_t size) : size_(size) {
    if (size > 0) {
        cudaError_t err = cudaMalloc(&ptr_, size_ * sizeof(T));
        if (err != cudaSuccess) {
            throw std::runtime_error("Failed to allocate device memory: " + std::string(cudaGetErrorString(err)));
        }
    }
}

template <typename T>
DeviceMemory<T>::~DeviceMemory() {
    if (ptr_) {
        cudaFree(ptr_);
    }
}

template <typename T>
DeviceMemory<T>::DeviceMemory(DeviceMemory&& other) noexcept 
    : ptr_(other.ptr_), size_(other.size_) {
    other.ptr_ = nullptr;
    other.size_ = 0;
}

template <typename T>
DeviceMemory<T>& DeviceMemory<T>::operator=(DeviceMemory&& other) noexcept {
    if (this != &other) {
        if (ptr_) {
            cudaFree(ptr_);
        }
        ptr_ = other.ptr_;
        size_ = other.size_;
        other.ptr_ = nullptr;
        other.size_ = 0;
    }
    return *this;
}

template <typename T>
T* DeviceMemory<T>::get() const {
    return ptr_;
}

template <typename T>
size_t DeviceMemory<T>::size() const {
    return size_;
}

// Explicit template instantiations for common types
template class DeviceMemory<float>;
template class DeviceMemory<double>;
template class DeviceMemory<int>;
template class DeviceMemory<char>;

}  // namespace cuda
}  // namespace sep
