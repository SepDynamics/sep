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
    std::vector<sep::quantum::Signal> base_signals;
    if (!engine_signals.empty()) {
        base_signals = engine_signals;
    } else if (!candles.empty()) {
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
        base_signals = engine_.getSignals();
    }

    std::vector<sep::quantum::Signal> signals;
    signals.reserve(base_signals.size());
    for (size_t i = 0; i < base_signals.size(); ++i) {
        auto s = base_signals[i];
        if (s.confidence < 0.6f) {
            s.type = sep::quantum::SignalType::HOLD;
        } else if (i >= 3 && i < candles.size()) {
            float avg = (static_cast<float>(candles[i - 1].close) +
                         static_cast<float>(candles[i - 2].close) +
                         static_cast<float>(candles[i - 3].close)) /
                        3.0f;
            float price = static_cast<float>(candles[i].close);
            if (s.type == sep::quantum::SignalType::BUY && price < avg) {
                s.type = sep::quantum::SignalType::HOLD;
            } else if (s.type == sep::quantum::SignalType::SELL && price > avg) {
                s.type = sep::quantum::SignalType::HOLD;
            }
        }
        signals.push_back(s);
    }
    return signals;
}
