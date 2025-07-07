#include "memory/memory_tier_manager.hpp"
#include "memory/memory_tier.hpp"
#include "memory/types.h"
#include "core/types.h"
#include "core/common.h"

#include <algorithm>
#include <memory>
#include <mutex>
#include <string>

namespace sep {
namespace memory {

// Forward declarations
class MemoryTier;
struct MemoryBlock;

// Using declarations
using Config = MemoryTierManager::Config;
using ::sep::core::SEPResult;
using ::sep::MemoryTierEnum;
using ::sep::pattern::PatternData;
using ::sep::persistence::PersistentPatternData;
using ::sep::quantum::Pattern;

// Mutex declarations
namespace {
std::mutex lookup_mutex;
std::mutex registry_mutex;
std::mutex relationships_mutex;
} // anonymous namespace

// Static member initialization
std::unique_ptr<MemoryTierManager> MemoryTierManager::instance_;
std::once_flag MemoryTierManager::once_flag_;

// --- Singleton Implementation ---

MemoryTierManager &MemoryTierManager::getInstance() {
  std::call_once(once_flag_, []() {
    const auto &mc =
        sep::config::ConfigManager::getInstance().getMemoryConfig();
    Config cfg{};
    cfg.promote_stm_to_mtm = mc.promote_stm_to_mtm;
    cfg.promote_mtm_to_ltm = mc.promote_mtm_to_ltm;
    cfg.demote_threshold = mc.demote_threshold;
    cfg.fragmentation_threshold = mc.fragmentation_threshold;
    cfg.stm_to_mtm_min_gen = mc.stm_to_mtm_min_gen;
    cfg.mtm_to_ltm_min_gen = mc.mtm_to_ltm_min_gen;
    cfg.stm_size = mc.stm_size;
    cfg.mtm_size = mc.mtm_size;
    cfg.ltm_size = mc.ltm_size;
    cfg.use_unified_memory = mc.use_unified_memory;
    cfg.enable_compression = mc.enable_compression;
    instance_ = std::make_unique<MemoryTierManager>(cfg);
  });
  return *instance_;
}

// --- Constructors and Destructor ---
MemoryTierManager::MemoryTierManager() {
  Config cfg{};
  init(cfg);
}

MemoryTierManager::MemoryTierManager(const Config &cfg) {
  init(cfg);
}

MemoryTierManager::MemoryTierManager(
    const sep::config::MemoryThresholdConfig &mc) {
  Config cfg{};
  cfg.promote_stm_to_mtm = mc.promote_stm_to_mtm;
    cfg.promote_mtm_to_ltm = mc.promote_mtm_to_ltm;
    cfg.demote_threshold = mc.demote_threshold;
    cfg.fragmentation_threshold = mc.fragmentation_threshold;
    cfg.stm_to_mtm_min_gen = mc.stm_to_mtm_min_gen;
    cfg.mtm_to_ltm_min_gen = mc.mtm_to_ltm_min_gen;
    // Note: sizes are missing from MemoryThresholdConfig, using defaults
    init(cfg);
}

MemoryTierManager::~MemoryTierManager() {
  shutdown();
}

void MemoryTierManager::init(const Config& config) {
    printf("DEBUG: Initializing MemoryTierManager\n");
    config_ = config;
    
    MemoryTier::Config scfg{MemoryTierEnum::STM, config_.stm_size};
    MemoryTier::Config mcfg{MemoryTierEnum::MTM, config_.mtm_size};
    MemoryTier::Config lcfg{MemoryTierEnum::LTM, config_.ltm_size};
    
    printf("DEBUG: Creating tiers - STM: %zu, MTM: %zu, LTM: %zu\n",
           config_.stm_size, config_.mtm_size, config_.ltm_size);
           
    stm_ = std::make_unique<MemoryTier>(scfg);
    mtm_ = std::make_unique<MemoryTier>(mcfg);
    ltm_ = std::make_unique<MemoryTier>(lcfg);

    printf("DEBUG: Initial utilization - STM: %f, MTM: %f, LTM: %f\n",
           stm_->calculateUtilization(),
           mtm_->calculateUtilization(),
           ltm_->calculateUtilization());

    // Redis manager is optional in this minimal build
}

void MemoryTierManager::shutdown() {
  stm_.reset();
  mtm_.reset();
  ltm_.reset();
  lookup_map_.clear();
  pattern_registry_.clear();
  pattern_relationships_.clear();
}

// --- Core Memory Operations ---
MemoryBlock* MemoryTierManager::allocate(std::size_t size, MemoryTierEnum tier) {
  MemoryTier* t = getTier(tier);
  if (!t)
    return nullptr;
  MemoryBlock* blk = t->allocate(size);
  if (blk) {
    std::lock_guard<std::mutex> lock(lookup_mutex);
    lookup_map_[blk->ptr] = blk;
    }
    return blk;
}
void MemoryTierManager::deallocate(MemoryBlock* block) {
  if (!block) return;
  {
    std::lock_guard<std::mutex> lock(lookup_mutex);
    lookup_map_.erase(block->ptr);
    }
    if (MemoryTier* t = getTier(block->tier)) {
      t->deallocate(block);
    }
}

MemoryBlock* MemoryTierManager::findBlockByPtr(void* ptr) {
  std::lock_guard<std::mutex> lock(lookup_mutex);
  auto it = lookup_map_.find(ptr);
  return it != lookup_map_.end() ? it->second : nullptr;
}

// --- Tier Management & Metrics ---
MemoryTier* MemoryTierManager::getTier(MemoryTierEnum tier) {
  switch (tier) {
    case MemoryTierEnum::STM: return stm_.get();
    case MemoryTierEnum::MTM: return mtm_.get();
    case MemoryTierEnum::LTM: return ltm_.get();
    default: return nullptr;
  }
}

MemoryTier& MemoryTierManager::getSTM() { return *stm_; }
MemoryTier& MemoryTierManager::getMTM() { return *mtm_; }
MemoryTier& MemoryTierManager::getLTM() { return *ltm_; }

float MemoryTierManager::getTierUtilization(MemoryTierEnum tier) const {
  const MemoryTier* t = const_cast<MemoryTierManager*>(this)->getTier(tier);
  return t ? t->calculateUtilization() : 0.0f;
}

float MemoryTierManager::getTierFragmentation(MemoryTierEnum tier) const {
  const MemoryTier* t = const_cast<MemoryTierManager*>(this)->getTier(tier);
  return t ? t->calculateFragmentation() : 0.0f;
}

std::size_t MemoryTierManager::getTotalAllocated() const {
  std::size_t total = 0;
  if (stm_)
    total += stm_->getSize() - stm_->getFreeSpace();
  if (mtm_)
    total += mtm_->getSize() - mtm_->getFreeSpace();
  if (ltm_)
    total += ltm_->getSize() - ltm_->getFreeSpace();
  return total;
}

float MemoryTierManager::getTotalUtilization() const {
  std::size_t total_size = 0;
  std::size_t used = 0;
  if (stm_) {
    total_size += stm_->getSize();
    used += stm_->getSize() - stm_->getFreeSpace();
  }
  if (mtm_) {
    total_size += mtm_->getSize();
    used += mtm_->getSize() - mtm_->getFreeSpace();
  }
  if (ltm_) {
    total_size += ltm_->getSize();
    used += ltm_->getSize() - ltm_->getFreeSpace();
  }
  if (total_size == 0)
    return 0.0f;
  return static_cast<float>(used) / static_cast<float>(total_size);
}

float MemoryTierManager::getTotalFragmentation() const {
  float total = 0.0f;
  int count = 0;
  if (stm_) {
    total += stm_->calculateFragmentation();
    ++count;
  }
  if (mtm_) {
    total += mtm_->calculateFragmentation();
    ++count;
  }
  if (ltm_) {
    total += ltm_->calculateFragmentation();
    ++count;
  }
  if (count == 0)
    return 0.0f;
  return total / count;
}

void MemoryTierManager::optimizeTiers() {
  if (stm_) stm_->defragment();
  if (mtm_) mtm_->defragment();
  if (ltm_) ltm_->defragment();
}

// --- Promotion and Demotion Logic ---
SEPResult MemoryTierManager::promoteToTier(MemoryBlock* block,
                                          MemoryTierEnum target_tier,
                                          MemoryBlock*& out_block) {
    printf("DEBUG: Attempting promotion from tier %d to tier %d\n",
           static_cast<int>(block->tier), static_cast<int>(target_tier));
    if (!block || !block->allocated) {
        return SEPResult::INVALID_ARGUMENT;
    }

    // Get source and destination tiers
    MemoryTier* src_tier = getTier(block->tier);
    MemoryTier* dst_tier = getTier(target_tier);
    if (!src_tier || !dst_tier) {
        return SEPResult::INVALID_ARGUMENT;
    }

    // Try to allocate in destination tier
    out_block = dst_tier->allocate(block->size);
    if (!out_block) {
        printf("DEBUG: Initial allocation failed, attempting defragmentation\n");
        // Try defragmenting destination tier
        dst_tier->defragment();
        out_block = dst_tier->allocate(block->size);
        if (!out_block) {
            printf("DEBUG: Allocation failed even after defragmentation\n");
            return SEPResult::ALLOCATION_FAILED;
        }
    }
    printf("DEBUG: Successfully allocated block in destination tier\n");

    // Copy block properties
    *out_block = *block;
    out_block->tier = target_tier;
    
    // Calculate new utilization based on destination tier size
    size_t total_size = dst_tier->getSize();
    out_block->utilization = total_size > 0 ?
        static_cast<float>(block->size) / static_cast<float>(total_size) : 0.0f;

    // Move data between tiers
    printf("DEBUG: Attempting to move data between tiers\n");
    if (!dst_tier->moveData(out_block, block)) {
        printf("DEBUG: Data move failed\n");
        dst_tier->deallocate(out_block);
        out_block = nullptr;
        return SEPResult::MEMORY_ERROR;
    }
    printf("DEBUG: Data move successful\n");

    // Update lookup maps in correct order to maintain consistency
    {
        std::lock_guard<std::mutex> lock(lookup_mutex);
        lookup_map_.erase(block->ptr);
        src_tier->deallocate(block);
        lookup_map_[out_block->ptr] = out_block;
    }

    return SEPResult::SUCCESS;
}

MemoryBlock *MemoryTierManager::updateBlockMetrics(MemoryBlock *block,
                                                   float coherence,
                                                   float stability,
                                                   uint32_t generation,
                                                   float context_score) {
  if (!block || !block->allocated) return block;
    block->coherence = coherence;
    block->stability = stability;
    block->generation = generation;
    block->weight = context_score;
    MemoryTier* current_tier_ptr = getTier(block->tier);
    if (!current_tier_ptr) return block; // Should not happen
    MemoryTier* target_tier_ptr = determineTier(coherence, stability, generation);
    if (!target_tier_ptr || target_tier_ptr == current_tier_ptr) {
        return block; // No move needed
    }

    MemoryBlock* new_block = nullptr;
    SEPResult result = promoteToTier(block, target_tier_ptr->getType(), new_block);

    if (result == SEPResult::SUCCESS) {
        return new_block;
    }

    // If migration failed, return the original block.
    return block;

}

MemoryTier *MemoryTierManager::determineTier(float coherence, float stability,
                                             int generation_count) {
    // LTM Check
    if (coherence >= config_.promote_mtm_to_ltm &&
    stability >= config_.promote_mtm_to_ltm && // Use same threshold for stability as a heuristic
    generation_count >= static_cast<int>(config_.mtm_to_ltm_min_gen)) {
    MemoryTier* ltm = getTier(MemoryTierEnum::LTM);
    if (ltm && ltm->getFreeSpace() > 0) return ltm;
    }


    // MTM Check
    if (coherence >= config_.promote_stm_to_mtm &&
        stability >= config_.promote_stm_to_mtm && // Use same threshold for stability as a heuristic
        generation_count >= static_cast<int>(config_.stm_to_mtm_min_gen)) {
        MemoryTier* mtm = getTier(MemoryTierEnum::MTM);
        if (mtm && mtm->getFreeSpace() > 0) return mtm;
    }

    // Default to STM or find first available
    MemoryTier* stm = getTier(MemoryTierEnum::STM);
    if (stm && stm->getFreeSpace() > 0) return stm;

    // Fallback if target tier is full
    MemoryTier* mtm = getTier(MemoryTierEnum::MTM);
    if (mtm && mtm->getFreeSpace() > 0) return mtm;

    MemoryTier* ltm = getTier(MemoryTierEnum::LTM);
    if (ltm && ltm->getFreeSpace() > 0) return ltm;

    return nullptr; // No space available
}

void MemoryTierManager::rebuildLookup() {
    std::lock_guard<std::mutex> lock(lookup_mutex);
    lookup_map_.clear();
    auto add_blocks = [this](MemoryTier* tier) {
        if (!tier) return;
        const auto& blocks = tier->getBlocks();
        for (const auto& blk : blocks) {
            if (blk.allocated)
                lookup_map_[blk.ptr] = const_cast<MemoryBlock*>(&blk);
        }
    };
    add_blocks(stm_.get());
    add_blocks(mtm_.get());
    add_blocks(ltm_.get());
}

// --- Pattern and Relationship Management ---
void MemoryTierManager::registerPattern(
    std::size_t id, const sep::pattern::PatternData &pattern) {
  std::lock_guard<std::mutex> lock(registry_mutex);
  pattern_registry_[id] = std::make_unique<sep::pattern::PatternData>(pattern);
}

const sep::pattern::PatternData* MemoryTierManager::getPatternData(std::size_t id) const {
  std::lock_guard<std::mutex> lock(registry_mutex);
  auto it = pattern_registry_.find(id);
  return it == pattern_registry_.end() ? nullptr : it->second.get();
}

void MemoryTierManager::removePattern(std::size_t id) {
  std::lock_guard<std::mutex> lock(registry_mutex);
  pattern_registry_.erase(id);


  // Also remove relationships associated with this pattern
  std::lock_guard<std::mutex> rel_lock(relationships_mutex);
  pattern_relationships_.erase(id);
  for (auto& pair : pattern_relationships_) {
    pair.second.erase(id);
    }

}

void MemoryTierManager::updateRelationship(std::size_t id_a, std::size_t id_b,
                                           float strength) {
  std::lock_guard<std::mutex> lock(relationships_mutex);
  pattern_relationships_[id_a][id_b] = strength;
  pattern_relationships_[id_b][id_a] = strength;
}

void MemoryTierManager::pruneWeakRelationships() {
  std::lock_guard<std::mutex> lock(relationships_mutex);
  for (auto& [id, relations] : pattern_relationships_) {
    for (auto it = relations.begin(); it != relations.end(); ) {
      if (it->second < config_.demote_threshold) { // Reuse demote threshold
        it = relations.erase(it);
      } else {
        ++it;
      }
    }
  }
}

void MemoryTierManager::calculateRelationshipCoherence() {
  std::lock_guard<std::mutex> reg_lock(registry_mutex);
  std::lock_guard<std::mutex> rel_lock(relationships_mutex);

  for (auto &[id, pattern_ptr] : pattern_registry_) {
    if (pattern_relationships_.count(id)) {
      const auto& rels = pattern_relationships_.at(id);
      if (!rels.empty()) {
        double sum = 0.0;
        for (const auto &r : rels) {
          sum += r.second;
          }
          pattern_ptr->coherence = static_cast<float>(sum / rels.size());
      }
    }
  }
}

} // namespace sep::memory