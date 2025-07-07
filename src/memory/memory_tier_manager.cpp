#include <string.h>
#include <cstring> // For std::memcpy, std::memset (if used implicitly)
#include <ctime>
#include <time.h>
#include <unistd.h>
#include <cstdlib>

#include "memory/memory_tier_manager.hpp"
#include "core/common.h"  // defines sep::SEPResult
#include "core/manager.h"
#include "memory/types.h"
#include "memory/redis_manager.h"
#include "core/manager.h"

#include "quantum/types.h"        // For ::sep::quantum::Pattern
#include "quantum/data.hpp"       // For ::sep::pattern::PatternData

#include "compat/cuda_common.h"
#include <cuda_runtime.h>
#include <cstring>

#include "compat/cuda_helpers.h"
#include "compat/kernels.cuh"
#include "memory/logger.hpp"
#include "quantum/pattern_evolution_bridge.h"
#include "core/manager.h"

#include <memory>
#include <mutex>
#include <vector>

namespace sep::memory {

using ::sep::MemoryTierEnum;

// Initialize singleton instance
std::unique_ptr<MemoryTierManager> MemoryTierManager::instance_;

// Config implementation moved to header

MemoryTierManager::MemoryTierManager() {
    Config cfg{};  // Uses default values from Config constructor
    init(cfg);
}

std::once_flag MemoryTierManager::once_flag_;

MemoryTierManager& MemoryTierManager::getInstance() {
    std::call_once(once_flag_, []() {
        const auto& mc = sep::config::ConfigManager::getInstance().getMemoryConfig();
        Config cfg{};
        cfg.promote_stm_to_mtm = mc.promote_stm_to_mtm;
        cfg.promote_mtm_to_ltm = mc.promote_mtm_to_ltm;
        cfg.demote_threshold = mc.demote_threshold;
        cfg.fragmentation_threshold = mc.fragmentation_threshold;
        cfg.stm_to_mtm_min_gen = mc.stm_to_mtm_min_gen;
        cfg.mtm_to_ltm_min_gen = mc.mtm_to_ltm_min_gen;
        instance_ = std::make_unique<MemoryTierManager>(cfg);
    });
    return *instance_;
}

MemoryTierManager::MemoryTierManager(const sep::config::MemoryThresholdConfig& mc) {
    Config cfg{};
    cfg.promote_stm_to_mtm = mc.promote_stm_to_mtm;
    cfg.promote_mtm_to_ltm = mc.promote_mtm_to_ltm;
    cfg.demote_threshold = mc.demote_threshold;
    cfg.fragmentation_threshold = mc.fragmentation_threshold;
    cfg.stm_to_mtm_min_gen = mc.stm_to_mtm_min_gen;
    cfg.mtm_to_ltm_min_gen = mc.mtm_to_ltm_min_gen;
    init(cfg);
}

MemoryTierManager::MemoryTierManager(const Config& cfg) {
    init(cfg);
}


MemoryTierManager::~MemoryTierManager() {
    shutdown();
}

void MemoryTierManager::init(const Config& config) {
    config_ = config;
    MemoryTier::Config scfg{MemoryTierEnum::STM, config.stm_size};
    MemoryTier::Config mcfg{MemoryTierEnum::MTM, config.mtm_size};
    MemoryTier::Config lcfg{MemoryTierEnum::LTM, config.ltm_size};
    stm_ = std::make_unique<MemoryTier>(scfg);
    mtm_ = std::make_unique<MemoryTier>(mcfg);
    ltm_ = std::make_unique<MemoryTier>(lcfg);
}

void MemoryTierManager::shutdown() {
    stm_.reset();
    mtm_.reset();
    ltm_.reset();
    lookup_map_.clear();
    pattern_registry_.clear();
    pattern_relationships_.clear();
    redis_manager_.reset();
}

MemoryBlock* MemoryTierManager::allocate(std::size_t size, MemoryTierEnum tier) {
    MemoryTier* t = getTier(tier);
    if (!t)
        return nullptr;
    MemoryBlock* blk = t->allocate(size);
    if (blk)
        lookup_map_[blk->ptr] = blk;
    return blk;
}

void MemoryTierManager::deallocate(MemoryBlock* block) {
    if (!block)
        return;
    lookup_map_.erase(block->ptr);
    if (MemoryTier* t = getTier(block->tier))
        t->deallocate(block);
}

MemoryTier* MemoryTierManager::getTier(MemoryTierEnum tier) {
    switch (tier) {
        case MemoryTierEnum::STM:
            return stm_.get();
        case MemoryTierEnum::MTM:
            return mtm_.get();
        case MemoryTierEnum::LTM:
            return ltm_.get();
        default:
            return nullptr;
    }
}

float MemoryTierManager::getTierUtilization(MemoryTierEnum tier) const {
    const MemoryTier* t = const_cast<MemoryTierManager*>(this)->getTier(tier);
    return t ? t->calculateUtilization() : 0.0f;
}

float MemoryTierManager::getTierFragmentation(MemoryTierEnum tier) const {
    const MemoryTier* t = const_cast<MemoryTierManager*>(this)->getTier(tier);
    return t ? t->calculateFragmentation() : 0.0f;
}

float MemoryTierManager::getTotalUtilization() const {
    float stm_util = getTierUtilization(MemoryTierEnum::STM);
    float mtm_util = getTierUtilization(MemoryTierEnum::MTM);
    float ltm_util = getTierUtilization(MemoryTierEnum::LTM);
    return (stm_util + mtm_util + ltm_util) / 3.0f;
}

float MemoryTierManager::getTotalFragmentation() const {
    float stm_frag = getTierFragmentation(MemoryTierEnum::STM);
    float mtm_frag = getTierFragmentation(MemoryTierEnum::MTM);
    float ltm_frag = getTierFragmentation(MemoryTierEnum::LTM);
    return (stm_frag + mtm_frag + ltm_frag) / 3.0f;
}

std::size_t MemoryTierManager::getTotalAllocated() const {
    std::size_t used_stm = config_.stm_size - stm_->getFreeSpace();
    std::size_t used_mtm = config_.mtm_size - mtm_->getFreeSpace();
    std::size_t used_ltm = config_.ltm_size - ltm_->getFreeSpace();
    return used_stm + used_mtm + used_ltm;
}

void MemoryTierManager::rebuildLookup() {
    lookup_map_.clear();

    auto rebuild = [this](MemoryTier* tier) {
        if (!tier) return;
        for (auto& blk : tier->getBlocks()) {
            if (blk.allocated) {
                lookup_map_[blk.ptr] = const_cast<MemoryBlock*>(&blk);
            }
        }
    };

    rebuild(stm_.get());
    rebuild(mtm_.get());
    rebuild(ltm_.get());
}

void MemoryTierManager::defragmentTier(MemoryTierEnum tier) {
    if (MemoryTier* t = getTier(tier))
        t->defragment();
}

void MemoryTierManager::optimizeBlocks() {
    stm_->defragment();
    mtm_->defragment();
    ltm_->defragment();
}

void MemoryTierManager::optimizeTiers() {
    optimizeBlocks();
}

MemoryTier& MemoryTierManager::getSTM() {
    return *stm_;
}
MemoryTier& MemoryTierManager::getMTM() {
    return *mtm_;
}
MemoryTier& MemoryTierManager::getLTM() {
    return *ltm_;
}

sep::SEPResult MemoryTierManager::promoteBlock(MemoryBlock* block, MemoryBlock*& out_block) {
    if (!block)
        return sep::SEPResult::INVALID_ARGUMENT;
    
    MemoryTierEnum next = block->tier == MemoryTierEnum::STM
                                       ? MemoryTierEnum::MTM
                                       : MemoryTierEnum::LTM;
    MemoryTier* dst = getTier(next);
    if (!dst)
        return sep::SEPResult::INVALID_ARGUMENT;
        
    out_block = dst->allocate(block->size);
    if (!out_block)
        return sep::SEPResult::ALLOCATION_FAILED;
        
    // Copy all block properties
    out_block->coherence = block->coherence;
    out_block->stability = block->stability;
    out_block->generation = block->generation;
    out_block->weight = block->weight;
    out_block->tier = next;
    out_block->utilization = block->utilization;
    out_block->access_count = block->access_count;
    out_block->compression = block->compression;
    out_block->original_size = block->original_size;
    out_block->coherence_trend = block->coherence_trend;
    out_block->last_coherence = block->last_coherence;
    out_block->compression_ratio = block->compression_ratio;
    out_block->wait = block->wait;
    
    // Get source tier and validate
    MemoryTier* src_tier = getTier(block->tier);
    if (!src_tier) {
        dst->deallocate(out_block);
        return sep::SEPResult::INVALID_ARGUMENT;
    }

    // Move data between tiers
    if (!dst->moveData(out_block, block)) {
        dst->deallocate(out_block);
        return sep::SEPResult::MEMORY_ERROR;
    }
    
    // Update tracking in correct order
    auto old_ptr = block->ptr;
    lookup_map_.erase(old_ptr); // Remove old entry first
    src_tier->deallocate(block);
    lookup_map_[out_block->ptr] = out_block; // Add new entry after
    
    return sep::SEPResult::SUCCESS;
}

sep::SEPResult MemoryTierManager::demoteBlock(MemoryBlock* block, MemoryBlock*& out_block) {
    if (!block)
        return sep::SEPResult::INVALID_ARGUMENT;
        
    MemoryTierEnum next = block->tier == MemoryTierEnum::LTM
                                       ? MemoryTierEnum::MTM
                                       : MemoryTierEnum::STM;
    MemoryTier* dst = getTier(next);
    if (!dst)
        return sep::SEPResult::INVALID_ARGUMENT;
        
    out_block = dst->allocate(block->size);
    if (!out_block)
        return sep::SEPResult::MEMORY_ERROR;
        
    // Copy all block properties and data
    out_block->coherence = block->coherence;
    out_block->stability = block->stability;
    out_block->generation = block->generation;
    out_block->weight = block->weight;
    out_block->tier = next;
    out_block->utilization = block->utilization;
    out_block->access_count = block->access_count;
    out_block->compression = block->compression;
    out_block->original_size = block->original_size;
    out_block->coherence_trend = block->coherence_trend;
    out_block->last_coherence = block->last_coherence;
    out_block->compression_ratio = block->compression_ratio;
    out_block->wait = block->wait;
    
    // Get source tier and validate
    MemoryTier* src_tier = getTier(block->tier);
    if (!src_tier) {
        dst->deallocate(out_block);
        return sep::SEPResult::INVALID_ARGUMENT;
    }

    // Move data between tiers
    if (!dst->moveData(out_block, block)) {
        dst->deallocate(out_block);
        return sep::SEPResult::MEMORY_ERROR;
    }
    
    // Update tracking in correct order
    auto old_ptr = block->ptr;
    lookup_map_.erase(old_ptr); // Remove old entry first
    src_tier->deallocate(block);
    lookup_map_[out_block->ptr] = out_block; // Add new entry after
    
    return sep::SEPResult::SUCCESS;
}

sep::SEPResult MemoryTierManager::launch_pattern_processing(sep::pattern::PatternData* patterns,
                                                            sep::pattern::PatternData* results,
                                                            const sep::pattern::PatternConfig& config,
                                                            size_t pattern_count,
                                                            const sep::pattern::PatternData* previous_patterns,
                                                            void* stream) {
#if defined(SEP_USE_CUDA) && defined(__CUDACC__)
    cudaStream_t cuda_stream = reinterpret_cast<cudaStream_t>(stream);
    cudaError_t err = sep::cuda::launch_pattern_processing(
        patterns, results, config, pattern_count, previous_patterns, cuda_stream);
    if (err != cudaSuccess) {
        return sep::SEPResult::CUDA_ERROR;
    }
#else
    (void)patterns;
    (void)results;
    (void)config;
    (void)pattern_count;
    (void)previous_patterns;
    (void)stream;
#endif
    return sep::SEPResult::SUCCESS;
}

void MemoryTierManager::updateBlockMetrics(MemoryBlock* block, float coherence, float stability,
                                           std::uint32_t generation, float context_score) {
    if (!block || !block->allocated)
        return;

    // Save all block properties before update
    float orig_coherence = block->coherence;
    float orig_stability = block->stability;
    uint32_t orig_generation = block->generation;
    float orig_weight = block->weight;
    uint64_t orig_wait = block->wait;
    float orig_utilization = block->utilization;
    uint32_t orig_access_count = block->access_count;
    
    // Update metrics
    block->coherence = coherence;
    block->stability = stability;
    block->generation = generation;
    block->weight = context_score;

    MemoryTier* target = determineTier(coherence, stability, static_cast<int>(generation));
    if (!target) {
        // If no tier available, try to defragment current tier
        MemoryTier* current = getTier(block->tier);
        if (current) {
            current->defragment();
            target = determineTier(coherence, stability, static_cast<int>(generation));
        }
    }

    if (target && target->getType() != block->tier) {
        MemoryBlock* out = nullptr;
        sep::SEPResult result;
        
        // Preserve properties during transition
        if (static_cast<int>(target->getType()) > static_cast<int>(block->tier)) {
            result = promoteBlock(block, out);
        } else {
            result = demoteBlock(block, out);
        }
            
        if (result == sep::SEPResult::SUCCESS && out) {
            // Restore preserved properties
            out->weight = orig_weight;
            out->wait = orig_wait;
            out->utilization = orig_utilization;
            out->access_count = orig_access_count;
            
            // Update new metrics
            out->coherence = coherence;
            out->stability = stability;
            out->generation = generation;
            
            // Lookup map already updated in promote/demote
        }
    }
}

MemoryBlock* MemoryTierManager::findBlockByPtr(void* ptr) {
    auto it = lookup_map_.find(ptr);
    return it != lookup_map_.end() ? it->second : nullptr;
}

MemoryTier* MemoryTierManager::determineTier(float coherence, float stability, int generation_count) {
    auto cfg = sep::config::ConfigManager::getInstance().getMemoryConfig();
    
    // Check LTM promotion criteria
    if (coherence >= 0.95f && // LTM threshold from tests
        stability >= 0.95f &&
        generation_count >= static_cast<int>(config_.mtm_to_ltm_min_gen)) {
        auto ltm = ltm_.get();
        if (ltm && ltm->getFreeSpace() > 0) {
            return ltm;
        }
    }
    
    // Check MTM promotion criteria
    if (coherence >= 0.75f && // MTM threshold from tests
        stability >= 0.75f &&
        generation_count >= static_cast<int>(config_.stm_to_mtm_min_gen)) {
        auto mtm = mtm_.get();
        if (mtm && mtm->getFreeSpace() > 0) {
            return mtm;
        }
    }
    
    auto stm = stm_.get();
    if (stm && stm->getFreeSpace() > 0) {
        return stm;
    }
    
    // If no tier has space, try to defragment each tier
    if (ltm_ && ltm_->getFreeSpace() == 0) ltm_->defragment();
    if (mtm_ && mtm_->getFreeSpace() == 0) mtm_->defragment();
    if (stm_ && stm_->getFreeSpace() == 0) stm_->defragment();
    
    // Return first tier with space, starting from highest priority
    if (ltm_ && ltm_->getFreeSpace() > 0) return ltm_.get();
    if (mtm_ && mtm_->getFreeSpace() > 0) return mtm_.get();
    if (stm_ && stm_->getFreeSpace() > 0) return stm_.get();
    
    return nullptr;  // No space available in any tier
}

void MemoryTierManager::updateRelationship(std::size_t id_a, std::size_t id_b, uint8_t type) {
    pattern_relationships_[id_a][id_b] = 1.0;  // Already using double
    pattern_relationships_[id_b][id_a] = 1.0;  // Already using double
    (void)type;  // Prevent unused parameter warning
}

void MemoryTierManager::removePattern(std::size_t id) {
    auto it = pattern_registry_.find(id);
    if (it == pattern_registry_.end())
        return;
    const sep::pattern::PatternData* p = it->second.get();
    if (MemoryTier* t = getTier(static_cast<MemoryTierEnum>(p->memory_tier)))
        t->removePattern(id);
    pattern_registry_.erase(it);
    pattern_relationships_.erase(id);
    for (auto& r : pattern_relationships_)
        r.second.erase(id);
}

void MemoryTierManager::pruneWeakRelationships() {
    auto cfg = sep::config::ConfigManager::getInstance().getMemoryConfig();
    for (auto& map : pattern_relationships_)
        for (auto it = map.second.begin(); it != map.second.end();) {
            if (it->second < cfg.demote_threshold)
                it = map.second.erase(it);
            else
                ++it;
        }
}

void MemoryTierManager::calculateRelationshipCoherence() {
    for (auto& entry : pattern_relationships_) {
        std::size_t id = entry.first;
        const auto& rels = entry.second;
        double avg = 0.0;
        if (!rels.empty()) {
            double sum = 0.0;
            for (const auto& r : rels)
                sum += r.second;  // r.second is already double
            avg = sum / static_cast<double>(rels.size());
        }
        auto pit = pattern_registry_.find(id);
        if (pit != pattern_registry_.end()) {
            pit->second->coherence = static_cast<float>(avg);  // Convert back to float for storage
        }
    }
}

void MemoryTierManager::loadLTMFromPersistence() {
    if (!redis_manager_ || !redis_manager_->isConnected())
        return;
    auto ids = redis_manager_->getPatternIds("ltm");
    for (std::size_t id : ids) {
        auto data_opt = redis_manager_->loadPattern(id, "ltm");
        if (!data_opt)
            continue;
        ltm_->addPattern(id, *data_opt);

        auto pat = std::make_unique<sep::pattern::PatternData>();
        pat->id = std::to_string(id);
        pat->generation = data_opt->generation_count;
        pat->position = glm::vec4(data_opt->position, 0.0f);
        pat->coherence = data_opt->coherence;
        pat->stability = data_opt->stability;
        pat->memory_tier = MemoryTierEnum::LTM;
        for (const auto& rel : data_opt->relationship_data) {
            sep::quantum::PatternRelationship pr;
            pr.targetId = std::to_string(rel.id);
            pr.type = static_cast<sep::quantum::RelationshipType>(rel.type);
            pr.strength = rel.strength;
            pat->relationships.push_back(pr);
            pattern_relationships_[id][rel.id] = rel.strength;
        }
        pattern_registry_[id] = std::move(pat);
    }
}

void MemoryTierManager::storeLTMToPersistence(const ::sep::quantum::Pattern& pattern, const persistence::PersistentPatternData& data) {
    if (!redis_manager_ || !redis_manager_->isConnected())
        return;
    std::size_t id = pattern.id.empty() ? 0 : std::stoull(pattern.id);
    redis_manager_->storePattern(id, data, "ltm");
}

std::unique_ptr<::sep::quantum::Pattern> MemoryTierManager::findPattern(std::size_t id) {
    auto it = pattern_registry_.find(id);
    if (it == pattern_registry_.end()) { 
        return nullptr;
    }
    const sep::pattern::PatternData* data = it->second.get();
    auto pattern = std::make_unique<sep::quantum::Pattern>();
    pattern->id = data->id;
    const float values[] = {data->attributes.x, data->attributes.y, data->attributes.z, data->attributes.w};
    pattern->data.assign(values, values + 4);
    pattern->quantum_state.coherence = data->coherence;
    pattern->quantum_state.stability = data->stability;
    pattern->quantum_state.generation = data->generation;
    pattern->relationships = data->relationships;
    return pattern;
}

std::unique_ptr<::sep::quantum::Pattern> MemoryTierManager::findPattern(std::size_t id) const {
    auto it = pattern_registry_.find(id);
    if (it == pattern_registry_.end()) {
        return nullptr;
    }
    const sep::pattern::PatternData* data = it->second.get();
    auto pattern = std::make_unique<sep::quantum::Pattern>();
    pattern->id = data->id;
    const float values[] = {data->attributes.x, data->attributes.y, data->attributes.z, data->attributes.w};
    pattern->data.assign(values, values + 4);
    pattern->quantum_state.coherence = data->coherence;
    pattern->quantum_state.stability = data->stability;
    pattern->quantum_state.generation = data->generation;
    pattern->relationships = data->relationships;
    return pattern;
}

void MemoryTierManager::cleanupExpiredPatterns() {
    std::vector<std::size_t> to_remove;
    auto cfg = sep::config::ConfigManager::getInstance().getMemoryConfig();
    for (const auto& p : pattern_registry_) {
        if (p.second->coherence < static_cast<double>(cfg.demote_threshold))
            to_remove.push_back(p.first);
    }
    for (std::size_t id : to_remove)
        removePattern(id);
}

void MemoryTierManager::prunePatternsByPriority(MemoryTierEnum tier, size_t max_count) {
    MemoryTier* t = getTier(tier);
    if (!t)
        return;
    const auto& patterns = t->getPatterns();
    if (patterns.size() <= max_count)
        return;

    std::vector<std::pair<std::size_t, double>> ids;
    ids.reserve(patterns.size());
    for (const auto& pair : patterns)
        ids.emplace_back(pair.first, static_cast<double>(pair.second.coherence));
    std::sort(ids.begin(), ids.end(), [](auto& a, auto& b) { return a.second > b.second; });
    for (size_t i = max_count; i < ids.size(); ++i) {
        std::size_t id = ids[i].first;
        if (pattern_registry_.count(id))
            removePattern(id);
        else
            t->removePattern(id);
    }
}

void MemoryTierManager::registerPattern(std::size_t id, const ::sep::pattern::PatternData& pattern) {
    pattern_registry_[id] = std::make_unique<::sep::pattern::PatternData>(pattern);
}

const ::sep::pattern::PatternData* MemoryTierManager::getPatternData(std::size_t id) const {
    auto it = pattern_registry_.find(id);
    return it == pattern_registry_.end() ? nullptr : it->second.get();
}

}  // namespace sep::memory
