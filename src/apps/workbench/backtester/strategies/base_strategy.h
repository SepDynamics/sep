#pragma once

#include <vector>

#include "quantum/pattern_metric_engine.h"
#include "common/financial_data_types.h"

class BaseStrategy {
public:
    virtual ~BaseStrategy() = default;

    /**
     * Execute the strategy on a sequence of candle data.
     *
     * @param candles Historical candle data to process.
     * @return Generated buy/sell/hold signals for each candle.
     */
    virtual std::vector<sep::quantum::Signal>
    execute(const std::vector<sep::common::CandleData>& candles) = 0;
};
