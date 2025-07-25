#pragma once

#include "core/metrics_monitor.h"
#include "signal_generator/signal_types.h"
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

    void run(const std::vector<float>& prices, const std::vector<MetricsMonitor::ThresholdSignal>& signals);
    const BacktestResult& getResult() const;

private:
    BacktestResult result_;
};

}