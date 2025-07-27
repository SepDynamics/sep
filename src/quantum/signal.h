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
    SignalType signal_type; // Alias for type
    double confidence;
    double signal_strength;
    double coherence;
    double stability;
};

} // namespace sep::quantum
