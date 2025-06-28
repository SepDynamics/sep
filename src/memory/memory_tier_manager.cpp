#include "memory/memory_tier_manager.hpp"
#include "memory/types.h"

#include "compat/cuda_common.h"

#include "compat/component_bridge.h"
#include "compat/cuda_helpers.h"
#include "memory/logger.hpp"
#include "memory/redis_manager.h"
#include "quantum/types.h"
#include "quantum/pattern_evolution_bridge.h"
#include "quantum/data.hpp"


namespace sep::memory {

// Initialize singleton instance
std::unique_ptr<MemoryTierManager> MemoryTierManager::instance_;
std::once_flag MemoryTierManager::once_flag_;

MemoryTierManager& MemoryTierManager::getInstance() {
    std::call_once(once_flag_, []() { instance_ = std::make_unique<MemoryTierManager>(); });
    return *instance_;
}

MemoryTierManager::MemoryTierManager(const sep::memory::Config& cfg) : config_(cfg) {
    init(cfg);
}

MemoryTierManager::MemoryTierManager() : MemoryTierManager(sep::memory::Config{}) {}

MemoryTierManager::~MemoryTierManager() {
    shutdown();
}

void MemoryTierManager::init(const sep::memory::Config& config) {
    config_ = config;
    MemoryTier::Config scfg{static_cast<TierType>(sep::MemoryTierEnum::STM), config.stm_size};
    MemoryTier::Config mcfg{static_cast<TierType>(sep::MemoryTierEnum::MTM), config.mtm_size};
    MemoryTier::Config lcfg{static_cast<TierType>(sep::MemoryTierEnum::LTM), config.ltm_size};
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

MemoryBlock* MemoryTierManager::allocate(std::size_t size, TierType tier) {
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

MemoryTier* MemoryTierManager::getTier(TierType tier) {
    switch (static_cast<MemoryTier>(tier)) {
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

double MemoryTierManager::getTierUtilization(TierType tier) const {
    const MemoryTier* t = const_cast<MemoryTierManager*>(this)->getTier(tier);
    return t ? t->calculateUtilization() : 0.0;
}

double MemoryTierManager::getTierFragmentation(TierType tier) const {
    const MemoryTier* t = const_cast<MemoryTierManager*>(this)->getTier(tier);
    return t ? t->calculateFragmentation() : 0.0;
}

double MemoryTierManager::getTotalUtilization() const {
    double stm_util = getTierUtilization(static_cast<TierType>(MemoryTierEnum::STM));
    double mtm_util = getTierUtilization(static_cast<TierType>(MemoryTierEnum::MTM));
    double ltm_util = getTierUtilization(static_cast<TierType>(MemoryTierEnum::LTM));
    return (stm_util + mtm_util + ltm_util) / 3.0;
}

double MemoryTierManager::getTotalFragmentation() const {
    double stm_frag = getTierFragmentation(static_cast<TierType>(MemoryTierEnum::STM));
    double mtm_frag = getTierFragmentation(static_cast<TierType>(MemoryTierEnum::MTM));
    double ltm_frag = getTierFragmentation(static_cast<TierType>(MemoryTierEnum::LTM));
    return (stm_frag + mtm_frag + ltm_frag) / 3.0;
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

void MemoryTierManager::defragmentTier(TierType tier) {
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
    TierType next = block->tier == static_cast<TierType>(MemoryTierEnum::STM)
                                     ? static_cast<TierType>(MemoryTierEnum::MTM)
                                     : static_cast<TierType>(MemoryTierEnum::LTM);
    MemoryTier* dst = getTier(next);
    if (!dst)
        return sep::SEPResult::INVALID_ARGUMENT;
    out_block = dst->allocate(block->size);
    if (!out_block)
        return sep::SEPResult::ALLOCATION_FAILED;
    dst->moveData(out_block, block);  // block is already a pointer
    lookup_map_[out_block->ptr] = out_block;
    getTier(block->tier)->deallocate(block);
    lookup_map_.erase(block->ptr);
    out_block->tier = next;
    return sep::SEPResult::SUCCESS;
}

sep::SEPResult MemoryTierManager::demoteBlock(MemoryBlock* block, MemoryBlock*& out_block) {
    if (!block)
        return sep::SEPResult::INVALID_ARGUMENT;
    TierType next = block->tier == static_cast<TierType>(MemoryTierEnum::LTM)
                                     ? static_cast<TierType>(MemoryTierEnum::MTM)
                                     : static_cast<TierType>(MemoryTierEnum::STM);
    MemoryTier* dst = getTier(next);
    if (!dst)
        return sep::SEPResult::INVALID_ARGUMENT;
    out_block = dst->allocate(block->size);
    if (!out_block)
        return sep::SEPResult::ALLOCATION_FAILED;
    dst->moveData(out_block, block);  // block is already a pointer
    lookup_map_[out_block->ptr] = out_block;
    getTier(block->tier)->deallocate(block);
    lookup_map_.erase(block->ptr);
    out_block->tier = next;
    return sep::SEPResult::SUCCESS;
}

sep::SEPResult MemoryTierManager::launch_pattern_processing(pattern::PatternData* patterns,
                                                            pattern::PatternData* results,
                                                            const pattern::PatternConfig& config,
                                                            size_t pattern_count,
                                                            const pattern::PatternData* previous_patterns,
                                                            void* stream) {
#ifdef SEP_USE_CUDA
    cudaStream_t cuda_stream = static_cast<cudaStream_t>(stream);
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
    if (!block)
        return;
    block->coherence = coherence;
    block->stability = stability;  // Using stability parameter
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
    pattern_relationships_[id_a][id_b] = 1.0;  // Already using double
    pattern_relationships_[id_b][id_a] = 1.0;  // Already using double
    (void)type;  // Prevent unused parameter warning
}

void MemoryTierManager::removePattern(std::size_t id) {
    auto it = pattern_registry_.find(id);
    if (it == pattern_registry_.end())
        return;
    const sep::pattern::PatternData* p = it->second.get();
    if (MemoryTier* t = getTier(static_cast<TierType>(p->memory_tier)))
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

        auto pat = std::make_unique<pattern::PatternData>();
        pat->id = std::to_string(id);
        pat->generation = data_opt->generation_count;
        pat->position = glm::vec4(data_opt->position, 0.0f);
        pat->coherence = data_opt->coherence;
        pat->stability = data_opt->stability;
        pat->memory_tier = MemoryTierEnum::LTM;
        for (const auto& rel : data_opt->relationship_data) {
            sep::quantum::PatternRelationship pr;
            pr.targetId = std::to_string(rel.id);
            pr.type = static_cast<quantum::RelationshipType>(rel.type);
            pr.strength = rel.strength;
            pat->relationships.push_back(pr);
            pattern_relationships_[id][rel.id] = rel.strength;
        }
        pattern_registry_[id] = std::move(pat);
    }
}

void MemoryTierManager::storeLTMToPersistence(const quantum::Pattern& pattern) {
    if (!redis_manager_ || !redis_manager_->isConnected())
        return;

    persistence::PatternData data{};
    data.position = glm::vec3(pattern.position);
    data.coherence = pattern.quantum_state.coherence;
    data.stability = pattern.quantum_state.stability;
    data.generation_count = pattern.quantum_state.generation;
    data.access_frequency = pattern.quantum_state.access_frequency;
    data.timestamp = shim::chrono::system_clock::now();
    for (const auto& rel : pattern.relationships) {
        persistence::RelationshipData rd{};
        rd.id = rel.targetId.empty() ? 0 : std::stoull(rel.targetId);
        rd.type = static_cast<uint8_t>(rel.type);
        rd.strength = rel.strength;
        data.relationships.push_back(rd.id);
        data.relationship_data.push_back(rd);
    }
    std::size_t id = pattern.id.empty() ? 0 : std::stoull(pattern.id);
    redis_manager_->storePattern(id, data, "ltm");
}

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
    pattern->quantum_state.coherence = data->coherence;
    pattern->quantum_state.stability = data->stability;
    pattern->quantum_state.generation = data->generation;
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
    pattern->quantum_state.coherence = data->coherence;
    pattern->quantum_state.stability = data->stability;
    pattern->quantum_state.generation = data->generation;
    pattern->relationships = data->relationships;
    return pattern;
}

void MemoryTierManager::cleanupExpiredPatterns() {
    std::vector<std::size_t> to_remove;
    for (const auto& p : pattern_registry_) {
        if (p.second->coherence < static_cast<double>(config_.demote_threshold))
            to_remove.push_back(p.first);
    }
    for (std::size_t id : to_remove)
        removePattern(id);
}

void MemoryTierManager::prunePatternsByPriority(TierType tier, size_t max_count) {
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

void MemoryTierManager::registerPattern(std::size_t id, const pattern::PatternData& pattern) {
    pattern_registry_[id] = std::make_unique<pattern::PatternData>(pattern);
}

const pattern::PatternData* MemoryTierManager::getPatternData(std::size_t id) const {
    auto it = pattern_registry_.find(id);
    return it == pattern_registry_.end() ? nullptr : it->second.get();
}

}  // namespace sep::memory
