#pragma once

#include <cstddef>
#include <cstdlib>
#include <cstring>

// Include CUDA runtime first to define CUDA types
#if SEP_CUDA_AVAILABLE
#include <cuda_runtime.h>
#endif

#include "types.h"
#include "compat/macros.h"
#include "compat/cuda_common.h"
#if SEP_CUDA_AVAILABLE
#include "compat/cuda_helpers.h"
#endif

namespace sep {
namespace cuda {

// Forward declarations
class Stream;

// Function declarations (must be before template class)
void* allocateDeviceMemory(size_t size);
void freeDeviceMemory(void* ptr);

// Device memory template
template <typename T>
class DeviceMemory {
public:
    DeviceMemory() : ptr_(nullptr), size_(0) {}

    DeviceMemory(size_t size) : ptr_(nullptr), size_(size) {
        ptr_ = static_cast<T*>(allocateDeviceMemory(size * sizeof(T)));
    }

    ~DeviceMemory() {
        if (ptr_)
            freeDeviceMemory(ptr_);
    }

    DeviceMemory(const DeviceMemory&) = delete;
    DeviceMemory& operator=(const DeviceMemory&) = delete;

    DeviceMemory(DeviceMemory&& other) noexcept : ptr_(other.ptr_), size_(other.size_) {
        other.ptr_ = nullptr;
        other.size_ = 0;
    }

    DeviceMemory& operator=(DeviceMemory&& other) noexcept {
        if (this != &other) {
            if (ptr_) {
                freeDeviceMemory(ptr_);
            }
            ptr_ = other.ptr_;
            size_ = other.size_;
            other.ptr_ = nullptr;
            other.size_ = 0;
        }
        return *this;
    }

    T* get() {
        return ptr_;
    }
    const T* get() const {
        return ptr_;
    }
    size_t size() const {
        return size_;
    }

private:
    T* ptr_ = nullptr;
    size_t size_ = 0;
};

// Memory management utilities
bool checkMemory(size_t required_size);

#if !SEP_CUDA_AVAILABLE
// Host-only implementations when CUDA is not available
inline void* allocateDeviceMemory(size_t size) {
    if (size == 0)
        return nullptr;
    return new (std::nothrow) std::uint8_t[size];
}

inline void freeDeviceMemory(void* ptr) {
    delete[] static_cast<std::uint8_t*>(ptr);
}

inline void* allocateUnifiedMemory(size_t size, void* stream = nullptr) {
    (void)stream;
    return allocateDeviceMemory(size);
}

inline void freeUnifiedMemory(void* ptr) {
    freeDeviceMemory(ptr);
}
#else
// Declarations for actual CUDA implementations
void* allocateDeviceMemory(size_t size);
void freeDeviceMemory(void* ptr);
void* allocateUnifiedMemory(size_t size, cudaStream_t stream = nullptr);
void freeUnifiedMemory(void* ptr);

// Memory copy utilities
template <typename T>
bool copyToDevice(T* dst, const T* src, size_t count, Stream* stream  = nullptr) {
#if SEP_CUDA_AVAILABLE
    cudaError_t error = ::sep::cuda::cudaMemcpyAsync(dst, src, count * sizeof(T), cudaMemcpyHostToDevice, nullptr);
    return error == cudaSuccess;
#else
    (void)stream;
    ::memcpy(dst, src, count * sizeof(T));
    return true;
#endif
}

template <typename T>
bool copyToHost(T* dst, const T* src, size_t count, Stream* stream  = nullptr) {
#if SEP_CUDA_AVAILABLE
    cudaError_t error = ::sep::cuda::cudaMemcpyAsync(dst, src, count * sizeof(T), cudaMemcpyDeviceToHost, nullptr);
    return error == cudaSuccess;
#else
    (void)stream;
    ::memcpy(dst, src, count * sizeof(T));
    return true;
#endif
}

// Memory initialization utilities
template <typename T>
bool memsetDevice(T* ptr, int value, size_t count) {
#if SEP_CUDA_AVAILABLE
    return cudaMemset(ptr, value, count * sizeof(T)) == cudaSuccess;
#else
    std::memset(ptr, value, count * sizeof(T));
    return true;
#endif
}

#endif  // !SEP_CUDA_AVAILABLE

}  // namespace cuda
}  // namespace sep
