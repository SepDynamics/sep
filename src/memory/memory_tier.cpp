#include "memory/memory_tier.hpp"
#include "compat/cuda.h"
#include "compat/macros.h"
#include "compat/math_common.h"
#include "core/allocation_metrics.h"
#include "core/common.h"
#include "core/logging.h"
#include "core/types.h"
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

namespace sep::memory {

using ::sep::MemoryTierEnum;

MemoryTier::MemoryTier(const Config &config)
    : config_(config), memory_pool_(nullptr), used_space_(0) {
  // Allocate memory pool based on tier type
  if (config.type == MemoryTierEnum::HOST) {
    memory_pool_ = std::malloc(config.size);
  } else {
    memory_pool_ = nullptr;
#if SEP_MEMORY_HAS_CUDA
    cudaError_t err = cudaMallocManaged(&memory_pool_, config.size);
    if (err != cudaSuccess) {
        auto logger = ::sep::logging::Manager::getInstance().getLogger("memory");
      if (logger) {
        logger->error("Failed to allocate managed memory: {}", err);
        logger->info("Falling back to host allocation");
      }
      memory_pool_ = std::malloc(config.size);
      if (!memory_pool_ && logger) {
        logger->error("Host allocation fallback failed");
      }
    }
#else
    memory_pool_ = std::malloc(config.size);
    cudaError_t err = memory_pool_ ? cudaSuccess : cudaErrorMemoryAllocation;
    if (err != cudaSuccess) {
        auto logger = ::sep::logging::Manager::getInstance().getLogger("memory");
      if (logger) {
        logger->error("Failed to allocate host memory: {}", err);
      }
    }
#endif
  }
  if (!memory_pool_) {
#if SEP_HAS_EXCEPTIONS
    throw std::runtime_error("Failed to allocate memory pool");
#else
    auto logger = ::sep::logging::Manager::getInstance().getLogger("memory");
    if (logger)
      LOG_CRITICAL(logger, "Failed to allocate memory pool");
    ::sep::metrics::allocationFailures().value++;
    // leave object in uninitialized state
    return;
#endif
  }
  blocks_.push_back(MemoryBlock(memory_pool_, config.size, 0, config.type));
}

MemoryTier::MemoryTier(MemoryTierEnum type, size_t max_patterns,
                       float coherence_threshold, int min_generations)
    : config_{Config{type, 0}}, memory_pool_(nullptr), used_space_(0),
      m_max_patterns(max_patterns), m_coherence_threshold(coherence_threshold),
      m_min_generations(min_generations) {}

MemoryTier::MemoryTier(const Config &config, size_t max_patterns,
                       float coherence_threshold,
                       int min_generations)
    : MemoryTier(config) // delegate to base memory constructor
{
  m_max_patterns = max_patterns;
  m_coherence_threshold = coherence_threshold;
  m_min_generations = min_generations;
}

MemoryTier::~MemoryTier() {
  if (memory_pool_) {
    if (config_.type == MemoryTierEnum::HOST) {
      std::free(memory_pool_);
    } else {
#if SEP_MEMORY_HAS_CUDA
      cudaFree(memory_pool_);
#else
      std::free(memory_pool_);
#endif
    }
    memory_pool_ = nullptr;
  }

  blocks_.clear();
  used_space_ = 0;
}

MemoryBlock *MemoryTier::allocate(std::size_t size) {
  // Find a suitable free block
  MemoryBlock *block = findFreeBlock(size);
  if (!block) {
    // Try defragmentation if no suitable block found
    defragment();
    block = findFreeBlock(size);
    if (!block) {
      ::sep::metrics::allocationFailures().value++;
      return nullptr; // Still no suitable block
    }
  }

  // Split block if it's significantly larger than requested
  if (block->size > size + sizeof(MemoryBlock)) {
    auto it = std::find_if(blocks_.begin(), blocks_.end(),
                           [&](const MemoryBlock &b) { return &b == block; });
    if (it != blocks_.end()) {
      std::size_t index = std::distance(blocks_.begin(), it);
      MemoryBlock new_block(static_cast<char *>(block->ptr) + size,
                            block->size - size, block->offset + size,
                            config_.type);
      blocks_.insert(std::next(it), new_block);
      // Reacquire block pointer since insertion may invalidate references
      block = &blocks_[index];
      block->size = size;
    }
  }

  block->allocated = true;
  block->utilization =
      block->allocated
          ? (static_cast<float>(size) / static_cast<float>(config_.size))
          : 0.0f; // Use floating point division
  block->access_count = 0;
  block->compression = ::sep::CompressionMethod::None;
  block->original_size = size;
  block->coherence = 0.0f;
  block->last_coherence = 0.0f;
  block->coherence_trend = 0.0f;
  block->generation = 0;
  block->weight = 1.0f; // Initialize with default weight of 1
  block->wait = 0;
  block->compression_ratio = 1.0f;
  block->stability = 0.0f;
  used_space_ += block->size;
  return block;
}

void MemoryTier::deallocate(MemoryBlock *block) {
  if (!block || !block->allocated) {
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
}

::sep::SEPResult MemoryTier::defragment() {
  auto logger = ::sep::logging::Manager::getInstance().getLogger("memory");
  if (logger) {
    LOG_DEBUG(logger, "Defragmenting tier {}", static_cast<int>(config_.type));
  }
  // Sort blocks by offset
  std::sort(blocks_.begin(), blocks_.end(),
            [](const MemoryBlock &a, const MemoryBlock &b) {
              return a.offset < b.offset;
            });

  // Compact allocated blocks to the start
  std::size_t current_offset = 0;
  for (auto &block : blocks_) {
    if (block.allocated) {
      if (block.offset != current_offset) {
        // Move memory to new position
        void *new_location = static_cast<char *>(memory_pool_) + current_offset;
#if SEP_MEMORY_HAS_CUDA
        cudaError_t err = cudaMemcpyAsync(new_location, block.ptr, block.size,
                                          cudaMemcpyDefault, nullptr);
        if (err != cudaSuccess) {
          if (logger) {
            LOG_ERROR(logger, "Defragment memory copy failed: {}", err);
          }
          return ::sep::SEPResult::CUDA_ERROR;
        }
        err = cudaStreamSynchronize(nullptr);
        if (err != cudaSuccess) {
          if (logger) {
            LOG_ERROR(logger, "Defragment stream sync failed: {}", err);
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
  if (current_offset < config_.size &&
      !blocks_.empty()) { // Check if blocks_ is not empty
    blocks_.erase(std::remove_if(blocks_.begin(), blocks_.end(),
                                 [](const MemoryBlock &block) {
                                   return !block.allocated;
                                 }),
                  blocks_.end());

    blocks_.push_back(MemoryBlock(
        static_cast<char *>(memory_pool_) + current_offset,
        config_.size - current_offset, current_offset, config_.type));
  }

  // Reevaluate block placement after defragmentation. Updating the blocks while
  // iterating over the container can invalidate references, so we build a list
  // of pointers first and then process them after the compaction step.
  MemoryTierManager &mgr = MemoryTierManager::getInstance();
  std::vector<MemoryBlock *> active_blocks;
  for (auto &blk : blocks_) {
    if (blk.allocated) {
      blk.utilization = static_cast<float>(blk.size) / config_.size;
      active_blocks.push_back(&blk);
    }
  }
  // Refresh the lookup table before invoking any promotion logic so that
  // updateBlockMetrics operates on up-to-date addresses.
  mgr.rebuildLookup();
  for (MemoryBlock *blk : active_blocks) {
    // The block may have been moved to another tier in a previous iteration, so
    // ensure it is still allocated before attempting to update its metrics.
    if (blk && blk->allocated) {
      mgr.updateBlockMetrics(blk, blk->coherence, blk->stability,
                             blk->generation, 1.0f);
    }
  }
  mgr.rebuildLookup();

  if (logger) {
    LOG_INFO(logger, "Tier {} fragmentation now {:.2f}",
             static_cast<int>(config_.type), calculateFragmentation());
  }
  return ::sep::SEPResult::SUCCESS;
}

float MemoryTier::calculateFragmentation() const {
  if (blocks_.empty())
    return 0.0f;

  // Count number of free blocks and total free space
  std::size_t free_block_count = 0;
  std::size_t total_free_space = 0;
  std::size_t largest_free_block = 0;

  for (const auto &block : blocks_) {
    if (!block.allocated) {
      free_block_count++;
      total_free_space += block.size;
      largest_free_block = std::max(largest_free_block, block.size);
    }
  }

  if (free_block_count <= 1)
    return 0.0f; // No fragmentation
  if (total_free_space == 0)
    return 0.0f; // No free space

  // Calculate fragmentation as ratio of largest free block to total free space
  return 1.0f - (static_cast<float>(largest_free_block) / total_free_space);
}

float MemoryTier::calculateUtilization() const {
  if (config_.size == 0)
    return 0.0f;

  // Recalculate used space on demand to avoid stale values in unit tests
  std::size_t used = 0;
  for (const auto &blk : blocks_) {
    if (blk.allocated)
      used += blk.size;
  }

  if (used == 0)
    return 0.0f;

  float util = static_cast<float>(used) / static_cast<float>(config_.size);

  // Guard against residual rounding errors that may appear when the used
  // space is very small compared to the tier size.  Unit tests expect an
  // exact zero value when nothing is allocated, so anything extremely close
  // to zero should be treated as zero.
  if (std::fabs(util) <= kUtilizationEpsilon)
    return 0.0f;

  return util > 1.0f ? 1.0f : util; // Cap at 100%
}

std::size_t MemoryTier::getFreeSpace() const {
  return config_.size - used_space_;
}

std::size_t MemoryTier::getLargestFreeBlock() const {
  std::size_t largest = 0;
  for (const auto &block : blocks_) {
    if (!block.allocated && block.size > largest) {
      largest = block.size;
    }
  }
  return largest;
}

const std::deque<MemoryBlock> &MemoryTier::getBlocks() const { return blocks_; }

bool MemoryTier::moveData(MemoryBlock *dst, const MemoryBlock *src) {
  auto logger = ::sep::logging::Manager::getInstance().getLogger("memory");

  if (!dst || !src || !dst->allocated || !src->allocated) {
    if (logger) {
      LOG_ERROR(logger, "Invalid blocks for data move");
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
  dst->utilization =
      static_cast<float>(size) / dst->size; // Use block size for utilization
  dst->access_count = src->access_count;
  dst->compression = src->compression;
  dst->original_size = src->original_size;
  dst->coherence_trend = src->coherence_trend;
  dst->last_coherence = src->last_coherence;
  dst->compression_ratio = src->compression_ratio;

#if SEP_MEMORY_HAS_CUDA
  cudaError_t err = ::sep::cuda::cudaMemcpyAsync(dst->ptr, src->ptr, size,
                                               cudaMemcpyDefault, nullptr);
  if (err != cudaSuccess) {
    if (logger) {
      LOG_ERROR(logger, "Failed to copy memory via CUDA: {}", err);
      LOG_INFO(logger, "Falling back to CPU memcpy");
    }
    std::memcpy(dst->ptr, src->ptr, size);
  } else {
    err = cudaStreamSynchronize(nullptr);
    if (err != cudaSuccess) {
      if (logger) {
        LOG_ERROR(logger, "Failed to synchronize stream: {}", err);
      }
      return false;
    }
  }
#else
  std::memcpy(dst->ptr, src->ptr, size);
#endif
  (void)logger; // suppress unused variable warning when CUDA is disabled

  // No need to update used_space_ here since it's already tracked in
  // allocate/deallocate
  return true;
}

MemoryBlock *MemoryTier::findFreeBlock(std::size_t size) {
  // Best fit strategy
  MemoryBlock *best_fit = nullptr;
  std::size_t smallest_sufficient = std::numeric_limits<std::size_t>::max();

  for (auto &block : blocks_) {
    if (!block.allocated && block.size >= size) {
      if (block.size < smallest_sufficient) {
        smallest_sufficient = block.size;
        best_fit = &block;
      }
    }
  }

  return best_fit;
}

void MemoryTier::splitBlock(MemoryBlock *block, std::size_t size) {
  assert(block && block->size > size);

  std::size_t remaining_size = block->size - size;
  block->size = size;

  // Create new block for remaining space
  blocks_.push_back(MemoryBlock(static_cast<char *>(block->ptr) + size,
                                remaining_size, block->offset + size,
                                config_.type));
}

void MemoryTier::mergeAdjacentBlocks() {
  // Sort blocks by offset
  std::sort(blocks_.begin(), blocks_.end(),
            [](const MemoryBlock &a, const MemoryBlock &b) {
              return a.offset < b.offset;
            });

  // Merge adjacent free blocks
  for (auto it = blocks_.begin(); it != blocks_.end();) {
    auto next = std::next(it);
    if (next != blocks_.end() && !it->allocated && !next->allocated) {
      // Merge blocks
      it->size += next->size;
      it = blocks_.erase(next);
    } else {
      ++it;
    }
  }

  // Recalculate used space to maintain accurate utilization metrics
  used_space_ = 0;
  for (const auto &blk : blocks_) {
    if (blk.allocated) {
      used_space_ += blk.size;
    }
  }
}

bool MemoryTier::resize(std::size_t new_size) {
  if (new_size == config_.size)
    return true;

  void *new_pool = nullptr;
  auto logger = ::sep::logging::Manager::getInstance().getLogger("memory");

  if (config_.type == MemoryTierEnum::HOST) {
    new_pool = std::malloc(new_size);
  } else {
#if SEP_MEMORY_HAS_CUDA
    cudaError_t err = ::sep::cuda::allocateManaged(&new_pool, new_size);
    if (err != cudaSuccess) {
      if (logger) {
        LOG_ERROR(logger, "Failed to allocate managed memory: {}", err);
      }
      ::sep::metrics::allocationFailures().value++;
      return false;
    }
#else
    new_pool = std::malloc(new_size);
    cudaError_t err = new_pool ? cudaSuccess : cudaErrorMemoryAllocation;
    if (err != cudaSuccess) {
      if (logger) {
        LOG_ERROR(logger, "Failed to allocate host memory: {}", err);
      }
      ::sep::metrics::allocationFailures().value++;
      return false;
    }
#endif
  }
  if (!new_pool) {
    if (logger) {
      LOG_ERROR(logger, "Failed to allocate memory pool of size {}", new_size);
    }
    ::sep::metrics::allocationFailures().value++;
    return false;
  }

  std::deque<MemoryBlock> new_blocks;
  std::size_t offset = 0;
  for (auto &block : blocks_) {
    if (!block.allocated)
      continue;
    if (offset + block.size > new_size) {
      if (config_.type == MemoryTierEnum::HOST)
        std::free(new_pool);
      else {
#if SEP_MEMORY_HAS_CUDA
        cudaFree(new_pool);
#else
        std::free(new_pool);
#endif
      }
      ::sep::metrics::allocationFailures().value++;
      return false;
    }
    std::memcpy(static_cast<char *>(new_pool) + offset, block.ptr, block.size);
    new_blocks.emplace_back(static_cast<char *>(new_pool) + offset, block.size,
                            offset, config_.type);
    MemoryBlock &nb = new_blocks.back();
    nb.allocated = true;
    nb.utilization = block.utilization;
    nb.access_count = block.access_count;
    nb.compression = block.compression;
    nb.original_size = block.original_size;
    nb.stability = block.stability;
    nb.coherence = block.coherence;
    nb.generation = block.generation;
    nb.weight = block.weight;
    nb.wait = block.wait;
    nb.coherence_trend = block.coherence_trend;
    nb.last_coherence = block.last_coherence;
    nb.compression_ratio = block.compression_ratio;
    offset += block.size;
  }

  if (offset < new_size)
    new_blocks.emplace_back(static_cast<char *>(new_pool) + offset,
                            new_size - offset, offset, config_.type);

  if (memory_pool_) {
    if (config_.type == MemoryTierEnum::HOST)
      std::free(memory_pool_);
    else {
#if SEP_MEMORY_HAS_CUDA
      cudaFree(memory_pool_);
#else
      std::free(memory_pool_);
#endif
    }
  }
  memory_pool_ = new_pool;
  config_.size = new_size;
  blocks_.swap(new_blocks);
  used_space_ = offset;
  return true;
}

bool MemoryTier::canAcceptPattern(
    const ::sep::persistence::PersistentPatternData &pattern) const {
  if (m_patterns.size() >= m_max_patterns)
    return false;
  if (pattern.coherence < m_coherence_threshold)
    return false;
  // STM should accept new patterns (generation = 0)
  // Only MTM and LTM have minimum generation requirements
  // Check generation count instead of memory_tier and generation
  if (pattern.generation_count < m_min_generations)
    return false;
  return true;
}

void MemoryTier::addPattern(size_t id,
                            ::sep::persistence::PersistentPatternData pattern) {
  if (!canAcceptPattern(pattern))
    return;
  // PatternData doesn't have id or memory_tier fields
  // Just store the pattern as is
  m_patterns[id] = std::move(pattern);
}

void MemoryTier::removePattern(size_t id) { m_patterns.erase(id); }

const ::sep::persistence::PersistentPatternData *
MemoryTier::getPattern(size_t id) const {
  auto it = m_patterns.find(id);
  return it == m_patterns.end() ? nullptr : &it->second;
}

::sep::persistence::PersistentPatternData *MemoryTier::getPattern(size_t id) {
  auto it = m_patterns.find(id);
  return it == m_patterns.end() ? nullptr : &it->second;
}

} // namespace sep::memory
