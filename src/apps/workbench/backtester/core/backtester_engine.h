#pragma once

#include "apps/workbench/backtester/data/data_loader.h"
#include "apps/workbench/backtester/backtester.h"
#include "apps/workbench/backtester/core/performance_metrics.h"
#include "apps/workbench/backtester/strategies/base_strategy.h"
#include "quantum/pattern_metric_engine.h"
#include <string>
#include <vector>

class BacktesterEngine {
public:
    BacktesterEngine();
    ~BacktesterEngine();

    sep::workbench::backtester::BacktestResult run(const std::string& dataset_path);
    sep::workbench::backtester::BacktestResult run(const std::string& dataset_path,
                                                   sep::quantum::PatternMetricEngine* engine,
                                                   BaseStrategy* strategy);
    sep::workbench::backtester::BacktestResult run(const std::string& dataset_path,
                                                   sep::quantum::PatternMetricEngine* engine);
    sep::workbench::backtester::BacktestResult run(
        const std::vector<sep::quantum::Signal>& signals,
        const std::string& dataset_path);
};
