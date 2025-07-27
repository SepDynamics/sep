#pragma once

#include <string>

namespace sep::quantum {

enum class SignalType {
    BUY,
    SELL,
    HOLD
};

struct Signal {
    std::string pattern_id;
    SignalType type;
    double confidence;
};

} // namespace sep::quantum
