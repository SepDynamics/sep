#pragma once

#include <cstddef>

#include "engine/internal/cuda.h"
#include "engine/raii.h"
#include "memory/types.h"

namespace sep {

/**
 * @brief Template class for managing CUDA unified memory
 * 
 * This class provides automatic memory management for CUDA unified memory,
 * which can be accessed by both CPU and GPU.
 */
template <typename T>
class UnifiedMemory {
public:
    /**
     * @brief Constructs a new UnifiedMemory object with the specified size
     * 
     * @param size Number of elements to allocate
     */
    UnifiedMemory(std::size_t size = 0) {
        if (size > 0) {
            void* ptr = sep::cuda::allocateUnifiedMemory(size * sizeof(T));
            if (ptr) {
                data_ = static_cast<T*>(ptr);
                size_ = size;
            }
        }
    }

    /**
     * @brief Constructs a new UnifiedMemory object with existing data pointer
     * 
     * @param data Pointer to existing data
     * @param size Number of elements
     */
    UnifiedMemory(T* data, std::size_t size) : data_(data), size_(size) {}
    
    /**
     * @brief Destroys the UnifiedMemory object and frees the allocated memory
     */
    ~UnifiedMemory() {
        if (data_) {
            sep::cuda::freeUnifiedMemory(data_);
            data_ = nullptr;
            size_ = 0;
        }
    }
    
    /**
     * @brief Gets the data pointer
     * 
     * @return T* Pointer to the allocated memory
     */
    T* get() const { return data_; }
    
    /**
     * @brief Gets the size of the allocation
     * 
     * @return std::size_t Number of elements
     */
    std::size_t size() const { return size_; }
    
    /**
     * @brief Checks if the allocation is valid
     * 
     * @return true If the memory is allocated
     * @return false If no memory is allocated
     */
    bool valid() const { return data_ != nullptr; }

private:
    T* data_ = nullptr;
    std::size_t size_ = 0;
};

namespace cuda {
// Use implementations provided by compat/raii
using ::sep::cuda::allocateDeviceMemory;
using ::sep::cuda::freeDeviceMemory;
using ::sep::cuda::allocateUnifiedMemory;
using ::sep::cuda::freeUnifiedMemory;
} // namespace cuda
}  // namespace sep
