#ifndef SEP_MEMORY_TIER_MANAGER_SERIALIZATION_HPP
#define SEP_MEMORY_TIER_MANAGER_SERIALIZATION_HPP

#include <nlohmann/json.hpp>

#include "memory/memory_tier_manager.hpp"

namespace sep {
namespace config {

struct MemoryThresholdConfig {
    uint32_t stm_size;
    uint32_t mtm_size;
    uint32_t ltm_size;
    float promote_stm_to_mtm;
    float promote_mtm_to_ltm;
    float demote_threshold;
    float fragmentation_threshold;
    bool use_unified_memory;
    bool enable_compression;
    uint32_t stm_to_mtm_min_gen;
    uint32_t mtm_to_ltm_min_gen;
};

inline void to_json(nlohmann::json& j, const MemoryThresholdConfig& c) {
    j = nlohmann::json{
        {"stm_size", c.stm_size},
        {"mtm_size", c.mtm_size},
        {"ltm_size", c.ltm_size},
        {"promote_stm_to_mtm", c.promote_stm_to_mtm},
        {"promote_mtm_to_ltm", c.promote_mtm_to_ltm},
        {"demote_threshold", c.demote_threshold},
        {"fragmentation_threshold", c.fragmentation_threshold},
        {"use_unified_memory", c.use_unified_memory},
        {"enable_compression", c.enable_compression},
        {"stm_to_mtm_min_gen", c.stm_to_mtm_min_gen},
        {"mtm_to_ltm_min_gen", c.mtm_to_ltm_min_gen}
    };
}

inline void from_json(const nlohmann::json& j, const MemoryThresholdConfig& c) {
    auto& cfg = const_cast<MemoryThresholdConfig&>(c);
    cfg.stm_size = j.value("stm_size", static_cast<uint32_t>(1 << 20));
    cfg.mtm_size = j.value("mtm_size", static_cast<uint32_t>(4 << 20));
    cfg.ltm_size = j.value("ltm_size", static_cast<uint32_t>(16 << 20));
    cfg.promote_stm_to_mtm = j.value("promote_stm_to_mtm", 0.7f);
    cfg.promote_mtm_to_ltm = j.value("promote_mtm_to_ltm", 0.9f);
    cfg.demote_threshold = j.value("demote_threshold", 0.3f);
    cfg.fragmentation_threshold = j.value("fragmentation_threshold", 0.3f);
    cfg.use_unified_memory = j.value("use_unified_memory", true);
    cfg.enable_compression = j.value("enable_compression", true);
    cfg.stm_to_mtm_min_gen = j.value("stm_to_mtm_min_gen", 5u);
    cfg.mtm_to_ltm_min_gen = j.value("mtm_to_ltm_min_gen", 100u);
}

} // namespace config
} // namespace sep

#endif // SEP_MEMORY_TIER_MANAGER_SERIALIZATION_HPP
