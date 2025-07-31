#pragma once

#include <nlohmann/json.hpp>

#include "engine/internal/types.h"

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
    static sep::compat::PatternData evolvePattern(const nlohmann::json& config,
                                                  const std::string& patternId = "");

    static std::vector<sep::compat::PatternData> getPatterns(const nlohmann::json& args = {});

    static sep::compat::PatternResult processPatterns(
        const std::vector<sep::compat::PatternData>& input,
        const sep::compat::PatternConfig& config, std::vector<sep::compat::PatternData>& output);

    static float calculateRelationshipStrength(const sep::compat::PatternData& pattern1,
                                               const sep::compat::PatternData& pattern2);

    static nlohmann::json toJson(const sep::compat::PatternData& pattern);

    static sep::compat::PatternData fromJson(const nlohmann::json& j);
};

} // namespace mcp
} // namespace quantum
} // namespace sep
