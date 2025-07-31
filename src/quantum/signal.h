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

struct QuantumSignalResult {
    double mean_price;
    float coherence;
    float stability;
    float confidence;
    float entropy;
    int rupture_count;
    int flip_count;
    float damped_coherence;
    float damped_stability;
};

} // namespace sep::quantum
