#pragma once

#include "common/candle_data.h"
#include "engine/pattern_types.h"
#include "quantum/signal.h"

namespace sep::workbench {

class AlphaTracker {
public:
    void processSignal(const ::sep::quantum::Signal& signal, const common::CandleData& current_candle);
    double getStrategyPipsGained() const { return strategy_pips_gained_; }
    double getBenchmarkPips() const { return benchmark_pips_; }
    void reset();

private:
    double strategy_pips_gained_ = 0.0;
    double benchmark_pips_ = 0.0;
    double buy_price_ = 0.0;
    bool in_trade_ = false;
    double initial_price_ = 0.0;
};

} // namespace sep::workbench
