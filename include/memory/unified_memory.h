#pragma once

#include "cuda/raii.h"
#include "cuda/cuda_defs.h"
#include "memory/manager.h"
#include "memory/types.h"
#include "memory/memory_tier.hpp"
#include <cstddef>

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

// Implementation of CUDA memory functions
inline void* allocateDeviceMemory(std::size_t size) {
  auto* block = memory::MemoryTierManager::getInstance().allocate(size, ::sep::memory::TierType::UNIFIED);
  return block ? block->ptr : nullptr;
}

inline void freeDeviceMemory(void* ptr) {
  auto& mgr = memory::MemoryTierManager::getInstance();
  memory::MemoryBlock* block = mgr.findBlockByPtr(ptr);
  if (block) {
    mgr.deallocate(block);
  } else {
    auto logger = sep::logging::Manager::getInstance().getLogger("memory");
    if (logger) {
      logger->error("Attempted to free unknown pointer {}", ptr);
    }
  }
}

inline void* allocateUnifiedMemory(std::size_t size, cudaStream_t stream) {
  auto* blk = memory::MemoryTierManager::getInstance().allocate(
      size, ::sep::memory::TierType::UNIFIED);
  if (blk && stream)
    cudaStreamAttachMemAsync(stream, blk->ptr);
  return blk ? blk->ptr : nullptr;
}

inline void freeUnifiedMemory(void* ptr) {
  auto& mgr = memory::MemoryTierManager::getInstance();
  if (auto* blk = mgr.findBlockByPtr(ptr))
    mgr.deallocate(blk);
}

} // namespace cuda
}  // namespace sep
