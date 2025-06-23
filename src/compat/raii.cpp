// Minimal implementation of CUDA RAII without including problematic headers

#ifndef SEP_CUDACC_DISABLE_EXCEPTION_SPEC_CHECKS
#define SEP_CUDACC_DISABLE_EXCEPTION_SPEC_CHECKS 1
#endif

// Include standard headers first
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <utility>

// Include CUDA headers in the correct order
#include "include/compat/raii.h"
#include "memory/memory_tier_manager.hpp"
#include "compat/cuda_helpers.h"

// Simple debug flag check without external logger dependency
namespace {
bool debugAllocEnabled() {
    static bool enabled = []() {
        const char* env = std::getenv("SEP_DEBUG_ALLOC");
        return env && (std::string(env) == "1");
    }();
    return enabled;
}
}  // namespace

namespace sep::cuda {

// Define dummy CUDA types and functions for compilation
typedef void* cudaStream_t;
typedef void* cudaEvent_t;
typedef int cudaError_t;
// Using constants from cuda.h instead of redefining them

// Dummy CUDA function implementations
cudaError_t cudaStreamCreateWithFlags(cudaStream_t* stream, unsigned int flags) {
    if (stream) {
        *stream = nullptr;
    }
    return cudaSuccess;
}

cudaError_t cudaStreamDestroy(cudaStream_t stream) {
    return cudaSuccess;
}

cudaError_t cudaStreamSynchronize(cudaStream_t stream) {
    return cudaSuccess;
}

cudaError_t cudaEventCreate(cudaEvent_t* event) {
    if (event) {
        *event = nullptr;
    }
    return cudaSuccess;
}

cudaError_t cudaEventDestroy(cudaEvent_t event) {
    return cudaSuccess;
}

cudaError_t cudaEventSynchronize(cudaEvent_t event) {
    return cudaSuccess;
}

cudaError_t cudaStreamAttachMemAsync(cudaStream_t stream, void* ptr, size_t size, unsigned int flags) {
    return cudaSuccess;
}

const char* cudaGetErrorString(cudaError_t error) {
    return "CUDA not available";
}

StreamRAII::StreamRAII(sep::StreamFlags flags) {
    unsigned int cuda_flags = (flags == sep::StreamFlags::NonBlocking) ? cudaStreamNonBlocking : cudaStreamDefault;
    cudaError_t err = cudaStreamCreateWithFlags(&stream_, cuda_flags);
    if (err != cudaSuccess) {
        stream_ = nullptr;
        if (debugAllocEnabled()) {
            (void)fprintf(stderr, "Failed to create CUDA stream: %s\n", cudaGetErrorString(err));
        }
    } else if (debugAllocEnabled()) {
        std::cout << "Stream created" << std::endl;
    }
}

StreamRAII::~StreamRAII() noexcept {
    if (stream_) {
        if (debugAllocEnabled()) {
            std::cout << "Stream destroyed" << std::endl;
        }
        CUDA_CHECK(cudaStreamDestroy(stream_));
        stream_ = nullptr;
    }
}

StreamRAII::StreamRAII(StreamRAII&& other) noexcept : stream_(std::exchange(other.stream_, nullptr)) {}

StreamRAII& StreamRAII::operator=(StreamRAII&& other) noexcept {
    if (this != &other) {
        if (stream_) {
            CUDA_CHECK(cudaStreamDestroy(stream_));
        }
        stream_ = std::exchange(other.stream_, nullptr);
    }
    return *this;
}

void StreamRAII::synchronize() const {
    if (stream_) {
        cudaError_t err = cudaStreamSynchronize(stream_);
        if (err != cudaSuccess) {
            if (debugAllocEnabled()) {
                (void)fprintf(stderr, "Failed to synchronize CUDA stream: %s\n", cudaGetErrorString(err));
            }
        }
    }
}

EventRAII::EventRAII() {
    cudaError_t err = cudaEventCreate(reinterpret_cast<cuda_stub_constants::cudaEvent_t*>(&event_));
    if (err != cudaSuccess) {
        event_ = nullptr;
        if (debugAllocEnabled()) {
            (void)fprintf(stderr, "Failed to create CUDA event: %s\n", cudaGetErrorString(err));
        }
    } else if (debugAllocEnabled()) {
        std::cout << "Event created" << std::endl;
    }
}

EventRAII::~EventRAII() noexcept {
    if (event_) {
        if (debugAllocEnabled()) {
            std::cout << "Event destroyed" << std::endl;
        }
        cudaError_t err = cudaEventDestroy(event_);
        if (err != cudaSuccess) {
            if (debugAllocEnabled()) {
                (void)fprintf(stderr, "Failed to destroy CUDA event: %s\n", cudaGetErrorString(err));
            }
        }
        event_ = nullptr;
    }
}

EventRAII::EventRAII(EventRAII&& other) noexcept : event_(std::exchange(other.event_, nullptr)) {}

EventRAII& EventRAII::operator=(EventRAII&& other) noexcept {
    if (this != &other) {
        if (event_) {
            cudaError_t err = cudaEventDestroy(event_);
            if (err != cudaSuccess) {
                if (debugAllocEnabled()) {
                    (void)fprintf(stderr, "Failed to destroy CUDA event: %s\n", cudaGetErrorString(err));
                }
            }
        }
        event_ = std::exchange(other.event_, nullptr);
    }
    return *this;
}

void EventRAII::synchronize() const {
    if (event_) {
        cudaError_t err = cudaEventSynchronize(event_);
        if (err != cudaSuccess) {
            if (debugAllocEnabled()) {
                (void)fprintf(stderr, "Failed to synchronize CUDA event: %s\n", cudaGetErrorString(err));
            }
        }
    }
}

template <typename T>
DeviceBufferRAII<T>::DeviceBufferRAII(std::size_t count) : count_(count) {
    if (count_ > 0) {
        ptr_ = static_cast<T*>(allocateDeviceMemory(count_ * sizeof(T)));
        if (debugAllocEnabled() && ptr_) {
            std::cout << "Device buffer allocated: " << (count_ * sizeof(T)) << " bytes" << std::endl;
        }
    }
}

template <typename T>
DeviceBufferRAII<T>::~DeviceBufferRAII() noexcept {
    if (ptr_) {
        if (debugAllocEnabled()) {
            std::cout << "Device buffer freed" << std::endl;
        }
        freeDeviceMemory(ptr_);
        ptr_ = nullptr;
        count_ = 0;
    }
}

template <typename T>
DeviceBufferRAII<T>::DeviceBufferRAII(DeviceBufferRAII&& other) noexcept
    : ptr_(std::exchange(other.ptr_, nullptr)), count_(std::exchange(other.count_, 0)) {}

template <typename T>
DeviceBufferRAII<T>& DeviceBufferRAII<T>::operator=(DeviceBufferRAII&& other) noexcept {
    if (this != &other) {
        if (ptr_) {
            freeDeviceMemory(ptr_);
        }
        ptr_ = std::exchange(other.ptr_, nullptr);
        count_ = std::exchange(other.count_, 0);
    }
    return *this;
}

// Explicit template instantiations for common types
template class DeviceBufferRAII<std::uint32_t>;
template class DeviceBufferRAII<std::uint64_t>;
template class DeviceBufferRAII<float>;
template class DeviceBufferRAII<double>;

// Implementation of memory management functions
void* allocateDeviceMemory(std::size_t size) {
    auto* block = memory::MemoryTierManager::getInstance().allocate(size, sep::memory::TierType::UNIFIED);
    return block ? block->ptr : nullptr;
}

void freeDeviceMemory(void* ptr) {
    if (!ptr)
        return;
    auto& mgr = memory::MemoryTierManager::getInstance();
    auto* blk = mgr.findBlockByPtr(ptr);
    if (blk)
        mgr.deallocate(blk);
}

void* allocateUnifiedMemory(std::size_t size, cudaStream_t stream) {
    auto* blk = memory::MemoryTierManager::getInstance().allocate(size, sep::memory::TierType::UNIFIED);
    if (blk && stream) {
        cudaError_t err = cudaStreamAttachMemAsync(stream, blk->ptr, 0, 0);
        if (err != cudaSuccess) {
            if (debugAllocEnabled()) {
                (void)fprintf(stderr, "Failed to attach memory to CUDA stream: %s\n", cudaGetErrorString(err));
            }
        }
    }
    return blk ? blk->ptr : nullptr;
}

void freeUnifiedMemory(void* ptr) {
    freeDeviceMemory(ptr);
}

}  // namespace sep::cuda
