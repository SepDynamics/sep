#include "memory/memory_tier_manager.hpp"

namespace sep::memory {

void __attribute__((weak)) MemoryTierManager::cleanupExpiredPatterns() {
    for (auto it = pattern_registry_.begin(); it != pattern_registry_.end();) {
        if (it->second && it->second->coherence < 0.5f) {
            it = pattern_registry_.erase(it);
        } else {
            ++it;
        }
    }
}

void __attribute__((weak)) MemoryTierManager::prunePatternsByPriority(MemoryTierEnum, size_t max_count) {
    if (pattern_registry_.size() <= max_count) return;
    std::vector<std::pair<size_t, std::unique_ptr<::sep::pattern::PatternData>>> items;
    items.reserve(pattern_registry_.size());
    for (auto &kv : pattern_registry_) items.emplace_back(kv.first, std::move(kv.second));
    std::sort(items.begin(), items.end(), [](auto &a, auto &b){return a.second->coherence > b.second->coherence;});
    pattern_registry_.clear();
    for (size_t i=0;i<max_count && i<items.size();++i) {
        pattern_registry_[items[i].first] = std::move(items[i].second);
    }
}

void __attribute__((weak)) MemoryTierManager::calculateRelationshipCoherence() {
    for (auto &[id, ptr] : pattern_registry_) {
        (void)id;
        (void)pattern_relationships_;
        ptr->coherence = 1.0f;
    }
}

}
