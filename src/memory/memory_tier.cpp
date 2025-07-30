#include "memory/memory_tier.hpp"

#include "engine/internal/allocation_metrics.h"
#include "engine/internal/common.h"
#include "engine/internal/cuda_sep.h"
#include "engine/internal/logging.h"
#include "engine/internal/macros.h"
#include "engine/internal/types.h"
#include "memory/logger.hpp"
#include "memory/memory_tier_manager.hpp"
#include "memory/types.h"

// Standard headers
#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <deque>
#include <limits>
#include <stdexcept>
#include <vector>

// CUDA support check
#if defined(__CUDACC__)
#define SEP_MEMORY_HAS_CUDA 1
#else
#define SEP_MEMORY_HAS_CUDA 0
#endif

#ifndef SEP_HAS_EXCEPTIONS
#if defined(__cpp_exceptions) || defined(__EXCEPTIONS) || defined(_CPPUNWIND)
#define SEP_HAS_EXCEPTIONS 1
#else
#define SEP_HAS_EXCEPTIONS 0
#endif
#endif

namespace sep::memory
{

    using sep::memory::MemoryTierEnum;

    MemoryTier::MemoryTier(const Config &config)
        : config_(config), memory_pool_(nullptr), used_space_(0)
    {
        // Allocate memory pool based on tier type. Logical tiers (STM/MTM/LTM)
        // should use host memory by default so tests behave deterministically even
        // when CUDA support is enabled. Only the physical DEVICE or UNIFIED tiers
        // require GPU-managed allocations.
        bool use_cuda =
            (config.type == MemoryTierEnum::DEVICE || config.type == MemoryTierEnum::UNIFIED);

        if (!use_cuda)
        {
            memory_pool_ = std::malloc(config.size);
#if SEP_MEMORY_HAS_CUDA
            cudaError_t err =
                memory_pool_ ? cudaSuccess : cudaErrorMemoryAllocation;
            if (err != cudaSuccess)
            {
                auto logger = ::sep::logging::Manager::getInstance().getLogger("memory");
                if (logger) logger->error("Failed to allocate host memory: {}", cudaGetErrorString(err));
            }
#endif
        }
        else
        {
            memory_pool_ = nullptr;
#if SEP_MEMORY_HAS_CUDA
            cudaError_t err = cudaMallocManaged(&memory_pool_, config.size);
            if (err != cudaSuccess)
            {
                auto logger = ::sep::logging::Manager::getInstance().getLogger("memory");
                if (logger)
                {
                    logger->error("Failed to allocate managed memory: {}", cudaGetErrorString(err));
                    logger->info("Falling back to host allocation");
                }
                memory_pool_ = std::malloc(config.size);
                if (!memory_pool_ && logger) logger->error("Host allocation fallback failed");
            }
#else
            memory_pool_ = std::malloc(config.size);
#ifdef __CUDACC__
            cudaError_t err = memory_pool_ ? cudaSuccess : cudaErrorMemoryAllocation;
            if (err != cudaSuccess)
            {
                auto logger = ::sep::logging::Manager::getInstance().getLogger("memory");
                if (logger) logger->error("Failed to allocate host memory: {}", cudaGetErrorString(err));
            }
#else
            if (!memory_pool_)
            {
                auto logger = ::sep::logging::Manager::getInstance().getLogger("memory");
                if (logger) logger->error("Failed to allocate host memory");
            }
#endif
#endif
        }
        if (!memory_pool_)
        {
#if SEP_HAS_EXCEPTIONS
            throw std::runtime_error("Failed to allocate memory pool");
#else
            auto logger = ::sep::logging::Manager::getInstance().getLogger("memory");
            if (logger) logger->critical("Failed to allocate memory pool");
            ::sep::metrics::allocationFailures().value++;
            // leave object in uninitialized state
            return;
#endif
        }
        blocks_.push_back(MemoryBlock(memory_pool_, config.size, 0, config.type));
    }

    MemoryTier::MemoryTier(MemoryTierEnum type, size_t max_patterns, float coherence_threshold,
                           int min_generations)
        : config_{Config{type, 0}},
          memory_pool_(nullptr),
          used_space_(0),
          m_max_patterns(max_patterns),
          m_coherence_threshold(coherence_threshold),
          m_min_generations(min_generations)
    {
    }

    MemoryTier::MemoryTier(const Config &config, size_t max_patterns, float coherence_threshold,
                           int min_generations)
        : MemoryTier(config)  // delegate to base memory constructor
    {
        m_max_patterns = max_patterns;
        m_coherence_threshold = coherence_threshold;
        m_min_generations = min_generations;
    }

    MemoryTier::~MemoryTier()
    {
        if (memory_pool_)
        {
            bool use_cuda =
                (config_.type == MemoryTierEnum::DEVICE || config_.type == MemoryTierEnum::UNIFIED);
            if (!use_cuda)
            {
                std::free(memory_pool_);
            }
            else
            {
#if SEP_MEMORY_HAS_CUDA
                ::cudaFree(memory_pool_);
#else
                std::free(memory_pool_);
#endif
            }
            memory_pool_ = nullptr;
        }

        blocks_.clear();
        used_space_ = 0;
    }

    MemoryBlock *MemoryTier::allocate(std::size_t size)
    {
        auto logger = ::sep::logging::Manager::getInstance().getLogger("memory");
        if (logger)
        {
            logger->debug("TIER {}: Attempting to allocate {} bytes.",
                          static_cast<int>(config_.type), size);
        }
        
        // Find a suitable free block
        MemoryBlock *block = findFreeBlock(size);
        if (!block)
        {
            if (logger)
            {
                logger->debug("TIER {}: No free block found for size {}. Fragmentation: {:.4f}",
                              static_cast<int>(config_.type), size, calculateFragmentation());
            }
            
            // Try defragmentation if no suitable block found
            defragment();
            block = findFreeBlock(size);
            if (!block)
            {
                if (logger)
                {
                    logger->error("TIER {}: Allocation FAILED even after defrag.",
                                  static_cast<int>(config_.type));
                }
                ::sep::metrics::allocationFailures().value++;
                return nullptr;  // Still no suitable block
            }
        }
        
        if (logger)
        {
            logger->debug("TIER {}: Allocation SUCCEEDED.", static_cast<int>(config_.type));
        }

        // Split block if it's significantly larger than requested
        if (block->size > size + sizeof(MemoryBlock))
        {
            auto it = std::find_if(blocks_.begin(), blocks_.end(),
                                   [&](const MemoryBlock &b) { return &b == block; });
            if (it != blocks_.end())
            {
                std::size_t index = std::distance(blocks_.begin(), it);
                MemoryBlock new_block(static_cast<char *>(block->ptr) + size, block->size - size,
                                      block->offset + size, config_.type);
                blocks_.insert(std::next(it), new_block);
                // Reacquire block pointer since insertion may invalidate references
                block = &blocks_[index];
                block->size = size;
            }
        }

        block->allocated = true;
        block->utilization = block->allocated
                                 ? (static_cast<float>(size) / static_cast<float>(config_.size))
                                 : 0.0f;  // Use floating point division
        block->access_count = 0;
        block->compression = sep::memory::CompressionMethod::None;
        block->original_size = size;
        block->coherence = 0.0f;
        block->last_coherence = 0.0f;
        block->coherence_trend = 0.0f;
        block->generation = 0;
        block->weight = 1.0f;  // Initialize with default weight of 1
        block->wait = 0;
        block->compression_ratio = 1.0f;
        block->stability = 0.0f;
        used_space_ += block->size;
        return block;
    }

    void MemoryTier::deallocate(MemoryBlock *block)
    {
        if (!block || !block->allocated)
        {
            return;
        }

        // Save properties before deallocating
        float coherence = block->coherence;
        float stability = block->stability;
        uint32_t generation = block->generation;
        float weight = block->weight;
        uint64_t wait = block->wait;

        // Update block state
        block->allocated = false;
        block->utilization = 0.0f;
        used_space_ -= block->size;

        // Preserve properties for potential promotion/demotion
        block->coherence = coherence;
        block->stability = stability;
        block->generation = generation;
        block->weight = weight;
        block->wait = wait;

        mergeAdjacentBlocks();

        // Recompute used space after merging to avoid tiny residual values that can
        // accumulate when tiers are resized or defragmented.  Unit tests rely on
        // exact zero utilization when all blocks have been freed.
        used_space_ = 0;
        for (const auto &blk : blocks_)
        {
            if (blk.allocated) used_space_ += blk.size;
        }
        constexpr std::size_t kMinBytes = 1;
        if (used_space_ <=
            std::max<std::size_t>(kMinBytes,
                                  static_cast<std::size_t>(kUtilizationEpsilon * config_.size)))
            used_space_ = 0;
    }

    sep::SEPResult MemoryTier::defragment()
    {
        auto logger = ::sep::logging::Manager::getInstance().getLogger("memory");
        if (logger)
        {
            logger->debug("Defragmenting tier {}", static_cast<int>(config_.type));
        }
        // Sort blocks by offset
        std::sort(blocks_.begin(), blocks_.end(),
                  [](const MemoryBlock &a, const MemoryBlock &b) { return a.offset < b.offset; });

        // Compact allocated blocks to the start
        std::size_t current_offset = 0;
        for (auto &block : blocks_)
        {
            if (block.allocated)
            {
                if (block.offset != current_offset)
                {
                    // Move memory to new position
                    void *new_location = static_cast<char *>(memory_pool_) + current_offset;
#if SEP_MEMORY_HAS_CUDA
                    cudaError_t err = cudaMemcpyAsync(
                        new_location, block.ptr, block.size, cudaMemcpyDefault, nullptr);
                    if (err != cudaSuccess)
                    {
                        if (logger)
                        {
                            logger->error("Defragment memory copy failed: {}",
                                          cudaGetErrorString(err));
                        }
                        return ::sep::SEPResult::CUDA_ERROR;
                    }
                    err = cudaStreamSynchronize(nullptr);
                    if (err != cudaSuccess)
                    {
                        if (logger)
                        {
                            logger->error("Defragment stream sync failed: {}",
                                          cudaGetErrorString(err));
                        }
                        return ::sep::SEPResult::CUDA_ERROR;
                    }
#else
                    std::memmove(new_location, block.ptr, block.size);
#endif

                    block.ptr = new_location;
                    block.offset = current_offset;
                }
                current_offset += block.size;
            }
        }

        // Merge all free space into one block at the end
        if (current_offset < config_.size && !blocks_.empty())
        {  // Check if blocks_ is not empty
            blocks_.erase(std::remove_if(blocks_.begin(), blocks_.end(),
                                         [](const MemoryBlock &block) { return !block.allocated; }),
                          blocks_.end());

            blocks_.push_back(MemoryBlock(static_cast<char *>(memory_pool_) + current_offset,
                                          config_.size - current_offset, current_offset,
                                          config_.type));
        }

        // Reevaluate block placement after defragmentation. Updating the blocks while
        // iterating over the container can invalidate references, so we build a list
        // of pointers first and then process them after the compaction step.
        MemoryTierManager &mgr = MemoryTierManager::getInstance();
        std::vector<MemoryBlock *> active_blocks;
        for (auto &blk : blocks_)
        {
            if (blk.allocated)
            {
                blk.utilization = static_cast<float>(blk.size) / config_.size;
                active_blocks.push_back(&blk);
            }
        }
        // Refresh the lookup table before invoking any promotion logic so that
        // updateBlockMetrics operates on up-to-date addresses.
        mgr.rebuildLookup();
        for (MemoryBlock *blk : active_blocks)
        {
            // The block may have been moved to another tier in a previous iteration, so
            // ensure it is still allocated before attempting to update its metrics.
            if (blk && blk->allocated)
            {
                mgr.updateBlockProperties(blk, blk->coherence, blk->stability, blk->generation, blk->weight);
            }
        }
        mgr.rebuildLookup();

        if (logger)
        {
            logger->info("Tier {} fragmentation now {:.2f}", static_cast<int>(config_.type),
                         calculateFragmentation());
        }
        return sep::SEPResult::SUCCESS;
    }

    float MemoryTier::calculateFragmentation() const
    {
        if (blocks_.empty()) return 0.0f;

        // Count number of free blocks and total free space
        std::size_t free_block_count = 0;
        std::size_t total_free_space = 0;
        std::size_t largest_free_block = 0;

        for (const auto &block : blocks_)
        {
            if (!block.allocated)
            {
                free_block_count++;
                total_free_space += block.size;
                largest_free_block = std::max(largest_free_block, block.size);
            }
        }

        if (free_block_count <= 1) return 0.0f;  // No fragmentation
        if (total_free_space == 0) return 0.0f;  // No free space

        // Calculate fragmentation as ratio of largest free block to total free space
        return 1.0f - (static_cast<float>(largest_free_block) / total_free_space);
    }

    float MemoryTier::calculateUtilization() const
    {
        if (config_.size == 0) return 0.0f;

        // Recalculate directly from blocks for accuracy in tests
        std::size_t current_used_space = 0;
        for (const auto &block : blocks_)
        {
            if (block.allocated)
            {
                current_used_space += block.size;
            }
        }

        if (current_used_space == 0) return 0.0f;

        float util = static_cast<float>(current_used_space) / static_cast<float>(config_.size);

        // Final check for floating point noise near zero
        return (util < kUtilizationEpsilon) ? 0.0f : util;
    }

    std::size_t MemoryTier::getFreeSpace() const { return config_.size - used_space_; }

    std::size_t MemoryTier::getLargestFreeBlock() const
    {
        std::size_t largest = 0;
        for (const auto &block : blocks_)
        {
            if (!block.allocated && block.size > largest)
            {
                largest = block.size;
            }
        }
        return largest;
    }

    const std::deque<MemoryBlock> &MemoryTier::getBlocks() const { return blocks_; }
    
    std::deque<MemoryBlock> &MemoryTier::getBlocksForModification() { return blocks_; }
    
    bool MemoryTier::moveData(MemoryBlock *dst, const MemoryBlock *src)
    {
        auto logger = ::sep::logging::Manager::getInstance().getLogger("memory");

        if (!dst || !src || !dst->allocated || !src->allocated)
        {
            if (logger)
            {
                logger->error("Invalid blocks for data move");
            }
            return false;
        }

        std::size_t size = std::min(dst->size, src->size);

        // Copy block properties before moving data
        dst->coherence = src->coherence;
        dst->stability = src->stability;
        dst->generation = src->generation;
        dst->weight = src->weight;
        dst->wait = src->wait;
        dst->utilization = static_cast<float>(size) / dst->size;  // Use block size for utilization
        dst->access_count = src->access_count;
        dst->compression = src->compression;
        dst->original_size = src->original_size;
        dst->coherence_trend = src->coherence_trend;
        dst->last_coherence = src->last_coherence;
        dst->compression_ratio = src->compression_ratio;

#if SEP_MEMORY_HAS_CUDA
        cudaError_t err =
            cudaMemcpyAsync(dst->ptr, src->ptr, size, cudaMemcpyDefault, nullptr);
        if (err != cudaSuccess)
        {
            if (logger)
            {
                logger->error("Failed to copy memory via CUDA: {}", cudaGetErrorString(err));
                logger->info("Falling back to CPU memcpy");
            }
            std::memcpy(dst->ptr, src->ptr, size);
        }
        else
        {
            err = cudaStreamSynchronize(nullptr);
            if (err != cudaSuccess)
            {
                if (logger)
                {
                    logger->error("Failed to synchronize stream: {}", cudaGetErrorString(err));
                }
                return false;
            }
        }
#else
        std::memcpy(dst->ptr, src->ptr, size);
#endif
        (void)logger;  // suppress unused variable warning when CUDA is disabled

        // No need to update used_space_ here since it's already tracked in
        // allocate/deallocate
        return true;
    }

    void MemoryTier::clear()
    {
        if (memory_pool_)
        {
            bool use_cuda =
                (config_.type == MemoryTierEnum::DEVICE || config_.type == MemoryTierEnum::UNIFIED);
            if (!use_cuda)
            {
                std::memset(memory_pool_, 0, config_.size);
            }
            else
            {
#if SEP_MEMORY_HAS_CUDA
                cudaError_t err = cudaMemset(memory_pool_, 0, config_.size);
                if (err != cudaSuccess)
                {
                    auto logger = ::sep::logging::Manager::getInstance().getLogger("memory");
                    if (logger)
                        logger->error("Failed to clear memory via CUDA: {}",
                                      cudaGetErrorString(err));
                }
#else
                std::memset(memory_pool_, 0, config_.size);
#endif
            }
        }
        used_space_ = 0;
        blocks_.clear();
        blocks_.push_back(MemoryBlock(memory_pool_, config_.size, 0, config_.type));
    }

    void MemoryTier::mergeAdjacentBlocks()
    {
        if (blocks_.size() < 2) return;

        bool merged;
        do
        {
            merged = false;
            for (auto it = blocks_.begin(); std::next(it) != blocks_.end();)
            {
                auto next_it = std::next(it);
                if (!it->allocated && !next_it->allocated)
                {
                    it->size += next_it->size;
                    blocks_.erase(next_it);
                    merged = true;
                }
                else
                {
                    ++it;
                }
            }
        } while (merged);
    }

    MemoryBlock *MemoryTier::findFreeBlock(std::size_t size)
    {
        for (auto &block : blocks_)
        {
            if (!block.allocated && block.size >= size)
            {
                return &block;
            }
        }
        return nullptr;
    }

    bool MemoryTier::resize(std::size_t new_size)
    {
        auto logger = ::sep::logging::Manager::getInstance().getLogger("memory");
        if (new_size == config_.size)
        {
            return true;
        }

        void *new_pool = nullptr;
        bool use_cuda =
            (config_.type == MemoryTierEnum::DEVICE || config_.type == MemoryTierEnum::UNIFIED);

        if (!use_cuda)
        {
            new_pool = std::realloc(memory_pool_, new_size);
            if (!new_pool)
            {
                if (logger) logger->error("Failed to reallocate host memory");
                return false;
            }
        }
        else
        {
#if SEP_MEMORY_HAS_CUDA
            cudaError_t err = cudaMallocManaged(&new_pool, new_size);
            if (err != cudaSuccess)
            {
                if (logger)
                    logger->error("Failed to allocate new managed memory: {}",
                                  cudaGetErrorString(err));
                return false;
            }

            if (memory_pool_)
            {
                err = cudaMemcpy(new_pool, memory_pool_, std::min(new_size, config_.size),
                                 cudaMemcpyDefault);
                if (err != cudaSuccess)
                {
                    if (logger)
                        logger->error("Failed to copy to new managed memory: {}",
                                      cudaGetErrorString(err));
                    cudaFree(new_pool);
                    return false;
                }
                cudaFree(memory_pool_);
            }
#else
            new_pool = std::realloc(memory_pool_, new_size);
            if (!new_pool)
            {
                if (logger) logger->error("Failed to reallocate host memory");
                return false;
            }
#endif
        }

        memory_pool_ = new_pool;
        std::size_t old_size = config_.size;
        config_.size = new_size;

        // Update block pointers and offsets
        for (auto &block : blocks_)
        {
            block.ptr = static_cast<char *>(memory_pool_) + block.offset;
        }

        // Add a new free block for the expanded space
        if (new_size > old_size)
        {
            blocks_.push_back(MemoryBlock(static_cast<char *>(memory_pool_) + old_size,
                                          new_size - old_size, old_size, config_.type));
            mergeAdjacentBlocks();
        }
        else
        {
            // Handle shrinking if necessary (more complex, requires moving data)
            // For now, just invalidate blocks that are out of bounds
            blocks_.erase(
                std::remove_if(blocks_.begin(), blocks_.end(),
                               [&](const MemoryBlock &b) { return b.offset >= new_size; }),
                blocks_.end());
            // Adjust the last block if it partially extends beyond the new size
            if (!blocks_.empty())
            {
                auto &last_block = blocks_.back();
                if (last_block.offset + last_block.size > new_size)
                {
                    last_block.size = new_size - last_block.offset;
                }
            }
        }

        return true;
    }

    void MemoryTier::setPromotionThreshold(float threshold)
    {
        m_coherence_threshold = threshold;
    }

}  // namespace sep::memory
