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

inline std::chrono::system_clock::time_point parseTimestamp(const std::string& ts) {
    std::tm tm = {};
    std::istringstream ss(ts);
    ss >> std::get_time(&tm, "%Y-%m-%dT%H:%M:%S");
    if (ss.fail()) {
        return std::chrono::system_clock::now();
    }

    auto tp = std::chrono::system_clock::from_time_t(std::mktime(&tm));

    if (ss.peek() == '.') {
        ss.get();
        std::string fractional;
        std::getline(ss, fractional, 'Z');
        while (fractional.size() < 9) fractional.push_back('0');
        int nanoseconds = 0;
        try {
            nanoseconds = std::stoi(fractional.substr(0, 9));
        } catch (...) {
            nanoseconds = 0;
        }
        tp += std::chrono::nanoseconds(nanoseconds);
    }

    return tp;
}
}  // namespace sep::common
