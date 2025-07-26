#pragma once

#include "base_strategy.h"
#include "quantum/pattern_metric_engine.h"

// Strategy that generates trading signals using the PatternMetricEngine.
class SEPSignalStrategy : public BaseStrategy {
public:
    SEPSignalStrategy();
    ~SEPSignalStrategy();

    std::vector<sep::quantum::Signal>
    execute(const std::vector<sep::common::CandleData>& candles,
            const std::vector<sep::quantum::Signal>& engine_signals) override;

private:
    sep::quantum::PatternMetricEngine engine_;
};
