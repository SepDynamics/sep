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
    static Pattern::PatternData evolvePattern(const nlohmann::json& config,
                                         const std::string& patternId = "");

    static std::vector<Pattern::PatternData> getPatterns(const nlohmann::json& args = {});

    static SEPResult processPatterns(const std::vector<Pattern::PatternData>& input,
                                          const ::sep::Pattern::PatternConfig& config,
                                          std::vector<Pattern::PatternData>& output);

    static float calculateRelationshipStrength(const Pattern::PatternData& pattern1,
                                               const Pattern::PatternData& pattern2);

    static nlohmann::json toJson(const Pattern::PatternData& pattern);

    static Pattern::PatternData fromJson(const nlohmann::json& j);
};

} // namespace mcp
} // namespace quantum
} // namespace sep