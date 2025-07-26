#include "sep_signal_strategy.h"

#include <iostream>

SEPSignalStrategy::SEPSignalStrategy() {
    if (engine_.init(nullptr) != sep::SEPResult::SUCCESS) {
        std::cerr << "Failed to initialize PatternMetricEngine" << std::endl;
    }
}

SEPSignalStrategy::~SEPSignalStrategy() = default;

std::vector<sep::quantum::Signal>
SEPSignalStrategy::execute(const std::vector<sep::common::CandleData>& candles,
                           const std::vector<sep::quantum::Signal>& engine_signals) {
    std::vector<sep::quantum::Signal> signals = engine_signals;
    if (!signals.empty()) {
        return signals;
    }
    if (candles.empty()) {
        return signals;
    }

    std::vector<uint8_t> byte_stream;
    byte_stream.reserve(candles.size() * sizeof(sep::common::CandleData));
    for (const auto& c : candles) {
        const uint8_t* ptr = reinterpret_cast<const uint8_t*>(&c);
        byte_stream.insert(byte_stream.end(), ptr,
                           ptr + sizeof(sep::common::CandleData));
    }

    engine_.ingestData(byte_stream.data(), byte_stream.size());
    engine_.evolvePatterns();
    engine_.computeMetrics();
    signals = engine_.getSignals();
    return signals;
}
