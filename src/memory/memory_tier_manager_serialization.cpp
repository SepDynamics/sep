#include <nlohmann/json.hpp>

#include "memory/memory_tier_manager.hpp"
#include "memory/persistent_pattern_data.hpp"

namespace sep {
namespace config {

    void to_json(nlohmann::json& j, const sep::memory::MemoryThresholdConfig& c)
    {
        j = nlohmann::json{{"stm_size", c.stm_size},
                           {"mtm_size", c.mtm_size},
                           {"ltm_size", c.ltm_size},
                           {"promote_stm_to_mtm", c.promote_stm_to_mtm},
                           {"promote_mtm_to_ltm", c.promote_mtm_to_ltm},
                           {"demote_threshold", c.demote_threshold},
                           {"fragmentation_threshold", c.fragmentation_threshold},
                           {"use_unified_memory", c.use_unified_memory},
                           {"enable_compression", c.enable_compression},
                           {"stm_to_mtm_min_gen", c.stm_to_mtm_min_gen}};
    }

    void from_json(const nlohmann::json& j, sep::memory::MemoryThresholdConfig& c)
    {
        c.stm_size = j.value("stm_size", static_cast<std::size_t>(1 << 20));
        c.mtm_size = j.value("mtm_size", static_cast<std::size_t>(4 << 20));
        c.ltm_size = j.value("ltm_size", static_cast<std::size_t>(16 << 20));
        c.promote_stm_to_mtm = j.value("promote_stm_to_mtm", 0.7f);
        c.promote_mtm_to_ltm = j.value("promote_mtm_to_ltm", 0.9f);
        c.demote_threshold = j.value("demote_threshold", 0.3f);
        c.fragmentation_threshold = j.value("fragmentation_threshold", 0.3f);
        c.use_unified_memory = j.value("use_unified_memory", true);
        c.enable_compression = j.value("enable_compression", true);
        c.stm_to_mtm_min_gen = j.value("stm_to_mtm_min_gen", 5u);
    }

} // namespace config
} // namespace sep
