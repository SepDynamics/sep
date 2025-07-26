#pragma once
#include <chrono>
#include <string>

namespace sep::common {

struct CandleData {
    std::chrono::system_clock::time_point time;
    double open, high, low, close;
    long long volume;
};

struct SEPSignalData {
    std::chrono::system_clock::time_point timestamp;
    double price;
    int signal_type; // e.g., 1 for buy, -1 for sell
    double confidence;
    float coherence;
    float stability;
    float entropy;
    float alpha_signal;
    float trend_strength;

    enum SignalType {
        STRONG_BUY,
        BUY,
        NEUTRAL,
        SELL,
        STRONG_SELL
    };
};

}  // namespace sep::common
