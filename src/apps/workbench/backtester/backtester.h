#pragma once

#include "quantum/pattern_metric_engine.h"
#include "data_loader.h"
#include "common/financial_data_types.h"
#include <vector>
#include <string>

namespace sep::workbench::backtester {

struct Trade {
    sep::quantum::SignalType type;
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
    std::vector<float> equity_curve;
    std::vector<Trade> trades;
};

class Backtester {
public:
    Backtester();

    void run(sep::quantum::PatternMetricEngine* engine, DataLoader* data_loader);
    void run(sep::quantum::PatternMetricEngine* engine,
             const std::vector<sep::common::CandleData>& data);
    void run(sep::quantum::PatternMetricEngine* engine,
             const std::string& dataset_path);
    void run(const std::vector<sep::quantum::Signal>& signals,
             const std::vector<sep::common::CandleData>& data);
    const BacktestResult& getResult() const;
    const std::vector<float>& getEquityCurve() const { return result_.equity_curve; }

private:
    BacktestResult result_;
};

}