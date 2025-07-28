#pragma once

#include "common/candle_data.h"
#include "rolling_window_manager.h"
#include <vector>
#include <cstdint>
#include "common/financial_data_types.h"

#include <chrono>

struct SEPSignal {
    SEPSignal() = default;
        SEPSignal(const sep::common::SEPSignalData& data) {
        timestamp = data.timestamp;
        coherence = data.coherence;
        entropy = data.entropy;
        stability = data.stability;
    }
        std::chrono::system_clock::time_point timestamp;
    
    enum ActionRecommendation {
        NO_SIGNAL,
        STRONG_BUY,
        BUY,
        NEUTRAL,
        SELL,
        STRONG_SELL,
        HOLD
    } signal_type = NO_SIGNAL;
    float coherence;
    float entropy;
    float stability;
};

class SEPSignalGenerator {
public:
    SEPSignalGenerator();

    SEPSignal calculate_signal(const RollingWindowManager& window_manager);
};
