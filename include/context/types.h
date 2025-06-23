#pragma once
#include <nlohmann/json.hpp>
#include "compat/shim.h"
#include <vector>
#include <string>

namespace sep::context {

struct CheckResult {
    enum class Status { STABLE = 0, INVALID };
    Status status{Status::STABLE};
    float score{0.0f};
    sep::shim::string error;
};

struct Context {
    sep::shim::string type;
    nlohmann::json content;
    std::vector<nlohmann::json> relationships;
    ::sep::shim::vector<sep::shim::string> tags;
    nlohmann::json metadata;
    nlohmann::json processorResult;
};

struct Batch {
    std::string layer;
    ::sep::shim::vector<Context> contexts;
};

} // namespace sep::context
