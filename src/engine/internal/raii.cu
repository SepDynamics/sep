#include <cuda_runtime.h>

#include <stdexcept>

#include "raii.h"

namespace sep {
namespace cuda {

StreamRAII::StreamRAII(unsigned int flags) {
    if (cudaStreamCreateWithFlags(&stream_, flags) != cudaSuccess) {
        throw std::runtime_error("Failed to create CUDA stream");
    }
}

StreamRAII::~StreamRAII() noexcept {
    if (stream_) {
        cudaStreamDestroy(stream_);
    }
}

StreamRAII::StreamRAII(StreamRAII&& other) noexcept : stream_(other.stream_) {
    other.stream_ = nullptr;
}

StreamRAII& StreamRAII::operator=(StreamRAII&& other) noexcept {
    if (this != &other) {
        if (stream_) {
            cudaStreamDestroy(stream_);
        }
        stream_ = other.stream_;
        other.stream_ = nullptr;
    }
    return *this;
}

cudaStream_t StreamRAII::get() const {
    return stream_;
}

bool StreamRAII::valid() const {
    return stream_ != nullptr;
}

void StreamRAII::synchronize() const {
    if (stream_) {
        cudaStreamSynchronize(stream_);
    }
}

template <typename T>
DeviceBufferRAII<T>::DeviceBufferRAII(std::size_t count) : count_(count) {
    if (count_ > 0) {
        if (cudaMalloc(&ptr_, count_ * sizeof(T)) != cudaSuccess) {
            throw std::runtime_error("Failed to allocate device memory");
        }
    }
}

template <typename T>
DeviceBufferRAII<T>::~DeviceBufferRAII() noexcept {
    if (ptr_) {
        cudaFree(ptr_);
    }
}

template <typename T>
DeviceBufferRAII<T>::DeviceBufferRAII(DeviceBufferRAII&& other) noexcept
    : ptr_(other.ptr_), count_(other.count_) {
    other.ptr_ = nullptr;
    other.count_ = 0;
}

template <typename T>
DeviceBufferRAII<T>& DeviceBufferRAII<T>::operator=(DeviceBufferRAII<T>&& other) noexcept {
    if (this != &other) {
        if (ptr_) {
            cudaFree(ptr_);
        }
        ptr_ = other.ptr_;
        count_ = other.count_;
        other.ptr_ = nullptr;
        other.count_ = 0;
    }
    return *this;
}

template <typename T>
T* DeviceBufferRAII<T>::get() const {
    return ptr_;
}

template <typename T>
std::size_t DeviceBufferRAII<T>::count() const {
    return count_;
}

template <typename T>
bool DeviceBufferRAII<T>::valid() const {
    return ptr_ != nullptr;
}

template class DeviceBufferRAII<std::uint32_t>;
template class DeviceBufferRAII<std::uint64_t>;
template class DeviceBufferRAII<int>;
template class DeviceBufferRAII<float>;
template class DeviceBufferRAII<double>;

}  // namespace cuda
}  // namespace sep
