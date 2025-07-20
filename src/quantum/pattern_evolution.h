#pragma once

#include <nlohmann/json.hpp>

#include "engine/types.h"

// Forward declaration of PatternConfig
namespace sep {
namespace pattern {
struct PatternConfig {};
}
}
#include <vector>
#include <string>

namespace sep {
namespace quantum {
namespace mcp {

class PatternEvolution {
public:
    static sep::pattern::PatternData evolvePattern(const nlohmann::json& config,
                                                   const shim::string& patternId = "");

    static shim::vector<sep::pattern::PatternData> getPatterns(const nlohmann::json& args = {});

    static pattern::PatternResult processPatterns(
        const shim::vector<sep::pattern::PatternData>& input,
        const sep::pattern::PatternConfig& config, shim::vector<sep::pattern::PatternData>& output);

    static float calculateRelationshipStrength(const sep::pattern::PatternData& pattern1,
                                               const sep::pattern::PatternData& pattern2);

    static nlohmann::json toJson(const sep::pattern::PatternData& pattern);

    static sep::pattern::PatternData fromJson(const nlohmann::json& j);
};

} // namespace mcp
} // namespace quantum
} // namespace sep
