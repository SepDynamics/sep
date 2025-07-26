#pragma once

#include "quantum/pattern_metric_engine.h"
#include "data/data_loader.h"
#include <vector>
#include <string>

namespace sep::workbench::backtester {

struct Trade {
    SignalType type;
    float entry_price;
    float exit_price;
    int holding_period;
};

struct BacktestResult {
    int total_trades;
    float win_rate;
    float total_pnl;
    float sharpe_ratio;
    float max_drawdown;
    std::vector<Trade> trades;
};

class Backtester {
public:
    Backtester();

    void run(sep::quantum::PatternMetricEngine* engine, DataLoader* data_loader);
    const BacktestResult& getResult() const;

private:
    BacktestResult result_;
};

}