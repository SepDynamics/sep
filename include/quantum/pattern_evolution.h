#pragma once

#include "quantum/types.h"
#include <nlohmann/json.hpp>
#include <vector>
#include <string>

namespace sep {
namespace quantum {
namespace mcp {

class PatternEvolution {
public:
    static pattern::PatternData evolvePattern(const nlohmann::json& config,
                                         const std::string& patternId = "");

    static std::vector<pattern::PatternData> getPatterns(const nlohmann::json& args = {});

    static SEPResult processPatterns(const std::vector<pattern::PatternData>& input,
                                          const ::sep::pattern::PatternConfig& config,
                                          std::vector<pattern::PatternData>& output);

    static float calculateRelationshipStrength(const pattern::PatternData& pattern1,
                                               const pattern::PatternData& pattern2);

    static nlohmann::json toJson(const pattern::PatternData& pattern);

    static pattern::PatternData fromJson(const nlohmann::json& j);
};

} // namespace mcp
} // namespace quantum
} // namespace sep