#pragma once

#include <nlohmann/json.hpp>
#include "quantum/types.h"

namespace sep::compat {
    void to_json(nlohmann::json& j, const PatternData& data);
    void from_json(const nlohmann::json& j, PatternData& data);
}

namespace sep::quantum {
    void to_json(nlohmann::json& j, const QuantumState& state);
    void from_json(const nlohmann::json& j, QuantumState& state);
    
    void to_json(nlohmann::json& j, const PatternRelationship& rel);
    void from_json(const nlohmann::json& j, PatternRelationship& rel);
    
    void to_json(nlohmann::json& j, const Pattern& pattern);
    void from_json(const nlohmann::json& j, Pattern& pattern);
}
