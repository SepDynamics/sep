#include <mutex>

#include "core/types.h"
#include "compat/kernels.cuh"
#include "memory/logger.hpp"
#include "memory/memory_tier.hpp"
#include "memory/memory_tier_manager.hpp"

namespace sep::memory {

std::unique_ptr<MemoryTierManager> MemoryTierManager::instance_;
std::once_flag MemoryTierManager::once_flag_;

MemoryTierManager& MemoryTierManager::getInstance() {
    std::call_once(once_flag_, []() { instance_ = std::make_unique<MemoryTierManager>(); });
    return *instance_;
}

MemoryTierManager::MemoryTierManager(const Config& cfg) : config_(cfg) {
    MemoryTier::Config scfg{static_cast<TierType>(sep::MemoryTierEnum::STM), cfg.stm_size};
    MemoryTier::Config mcfg{static_cast<TierType>(sep::MemoryTierEnum::MTM), cfg.mtm_size};
    MemoryTier::Config lcfg{static_cast<TierType>(sep::MemoryTierEnum::LTM), cfg.ltm_size};
    stm_ = std::make_unique<MemoryTier>(scfg);
    mtm_ = std::make_unique<MemoryTier>(mcfg);
    ltm_ = std::make_unique<MemoryTier>(lcfg);
}

MemoryTierManager::~MemoryTierManager() = default;

MemoryBlock* MemoryTierManager::allocate(std::size_t size, sep::memory::TierType tier) {
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

MemoryTier* MemoryTierManager::getTier(sep::memory::TierType tier) {
    switch (static_cast<MemoryTierEnum>(tier)) {
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

float MemoryTierManager::getTierUtilization(sep::memory::TierType tier) const {
    const MemoryTier* t = const_cast<MemoryTierManager*>(this)->getTier(tier);
    return t ? t->calculateUtilization() : 0.0f;
}

float MemoryTierManager::getTierFragmentation(sep::memory::TierType tier) const {
    const MemoryTier* t = const_cast<MemoryTierManager*>(this)->getTier(tier);
    return t ? t->calculateFragmentation() : 0.0f;
}

void MemoryTierManager::updateBlockMetrics(MemoryBlock* block, float coherence, float stability,
                                           std::uint32_t generation, float context_score) {
    if (!block)
        return;
    block->coherence = coherence;
    block->stability = stability;
    block->generation = generation;
    block->weight = context_score;

    MemoryTier* target = determineTier(coherence, stability, static_cast<int>(generation));
    if (target && target->getType() != block->tier) {
        MemoryBlock* out = nullptr;
        if (static_cast<int>(target->getType()) > static_cast<int>(block->tier))
            promoteBlock(block, out);
        else
            demoteBlock(block, out);
    }
}

void MemoryTierManager::defragmentTier(sep::memory::TierType tier) {
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

SEPResult MemoryTierManager::promoteBlock(MemoryBlock* block, MemoryBlock*& out_block) {
    if (!block)
        return SEPResult::INVALID_ARGUMENT;
    sep::memory::TierType next = block->tier == static_cast<TierType>(MemoryTierEnum::STM)
                                     ? static_cast<TierType>(MemoryTierEnum::MTM)
                                     : static_cast<TierType>(MemoryTierEnum::LTM);
    MemoryTier* dst = getTier(next);
    if (!dst)
        return SEPResult::INVALID_ARGUMENT;
    out_block = dst->allocate(block->size);
    if (!out_block)
        return SEPResult::ALLOCATION_FAILED;
    dst->moveData(out_block, block);  // block is already a pointer
    lookup_map_[out_block->ptr] = out_block;
    getTier(block->tier)->deallocate(block);
    lookup_map_.erase(block->ptr);
    out_block->tier = next;
    return SEPResult::SUCCESS;
}

SEPResult MemoryTierManager::demoteBlock(MemoryBlock* block, MemoryBlock*& out_block) {
    if (!block)
        return SEPResult::INVALID_ARGUMENT;
    sep::memory::TierType next = block->tier == static_cast<TierType>(MemoryTierEnum::LTM)
                                     ? static_cast<TierType>(MemoryTierEnum::MTM)
                                     : static_cast<TierType>(MemoryTierEnum::STM);
    MemoryTier* dst = getTier(next);
    if (!dst)
        return SEPResult::INVALID_ARGUMENT;
    out_block = dst->allocate(block->size);
    if (!out_block)
        return SEPResult::ALLOCATION_FAILED;
    dst->moveData(out_block, block);  // block is already a pointer
    lookup_map_[out_block->ptr] = out_block;
    getTier(block->tier)->deallocate(block);
    lookup_map_.erase(block->ptr);
    out_block->tier = next;
    return SEPResult::SUCCESS;
}

SEPResult MemoryTierManager::launch_pattern_processing(pattern::PatternData* patterns, pattern::PatternData* results,
                                                       const pattern::PatternConfig& config, size_t pattern_count,
                                                       const pattern::PatternData* previous_patterns, void* stream) {
#ifdef __CUDACC__
    cudaError_t err =
        sep::cuda::launch_pattern_processing(patterns, results, config, pattern_count, previous_patterns, stream);
    return err == cudaSuccess ? SEPResult::SUCCESS : SEPResult::PROCESSING_ERROR;
#else
    (void)patterns;
    (void)results;
    (void)config;
    (void)pattern_count;
    (void)previous_patterns;
    (void)stream;
    return SEPResult::SUCCESS;
#endif
}

MemoryBlock* MemoryTierManager::findBlockByPtr(void* ptr) {
    auto it = lookup_map_.find(ptr);
    return it != lookup_map_.end() ? it->second : nullptr;
}

MemoryTier* MemoryTierManager::determineTier(float coherence, float stability, int generation_count) {
    if (coherence >= config_.promote_mtm_to_ltm && generation_count >= static_cast<int>(config_.mtm_to_ltm_min_gen))
        return ltm_.get();
    if (coherence >= config_.promote_stm_to_mtm && generation_count >= static_cast<int>(config_.stm_to_mtm_min_gen))
        return mtm_.get();
    return stm_.get();
}

void MemoryTierManager::updateRelationship(std::size_t id_a, std::size_t id_b, uint8_t type) {
    pattern_relationships_[id_a][id_b] = 1.0;
    pattern_relationships_[id_b][id_a] = 1.0;
    (void)type;  // Prevent unused parameter warning
}

void MemoryTierManager::removePattern(std::size_t id) {
    auto it = pattern_registry_.find(id);
    if (it == pattern_registry_.end())
        return;
    const sep::pattern::PatternData* p = it->second.get();
    if (MemoryTier* t = getTier(static_cast<sep::memory::TierType>(p->memory_tier)))
        t->removePattern(id);
    pattern_registry_.erase(it);
    pattern_relationships_.erase(id);
    for (auto& r : pattern_relationships_)
        r.second.erase(id);
}

void MemoryTierManager::pruneWeakRelationships() {
    for (auto& map : pattern_relationships_)
        for (auto it = map.second.begin(); it != map.second.end();)
            if (it->second < config_.demote_threshold)
                it = map.second.erase(it);
            else
                ++it;
}

void MemoryTierManager::calculateRelationshipCoherence() {}
void MemoryTierManager::loadLTMFromPersistence() {}
void MemoryTierManager::storeLTMToPersistence(const quantum::Pattern&) {}

quantum::Pattern* MemoryTierManager::findPattern(std::size_t id) {
    auto it = pattern_registry_.find(id);
    if (it == pattern_registry_.end()) {
        return nullptr;
    }
    const sep::pattern::PatternData* data = it->second.get();
    quantum::Pattern* pattern = new quantum::Pattern();
    pattern->id = data->id;
    const float values[] = {data->attributes.x, data->attributes.y, data->attributes.z, data->attributes.w};
    pattern->data.assign(values, values + 4);
    pattern->metadata.coherence_score = data->coherence;
    pattern->metadata.stability_score = data->stability;
    pattern->metadata.generation_count = data->generation;
    pattern->relationships = data->relationships;
    return pattern;
}

const quantum::Pattern* MemoryTierManager::findPattern(std::size_t id) const {
    auto it = pattern_registry_.find(id);
    if (it == pattern_registry_.end()) {
        return nullptr;
    }
    const sep::pattern::PatternData* data = it->second.get();
    quantum::Pattern* pattern = new quantum::Pattern();
    pattern->id = data->id;
    const float values[] = {data->attributes.x, data->attributes.y, data->attributes.z, data->attributes.w};
    pattern->data.assign(values, values + 4);
    pattern->metadata.coherence_score = data->coherence;
    pattern->metadata.stability_score = data->stability;
    pattern->metadata.generation_count = data->generation;
    pattern->relationships = data->relationships;
    return pattern;
}

void MemoryTierManager::cleanupExpiredPatterns() {}
void MemoryTierManager::prunePatternsByPriority(sep::memory::TierType, size_t) {}

}  // namespace sep::memory
