#pragma once

#include "apps/workbench/backtester/data/data_loader.h"
#include "apps/workbench/backtester/backtester.h"
#include "apps/workbench/backtester/core/performance_metrics.h"
#include "quantum/pattern_metric_engine.h"
#include <string>
#include <vector>

class BacktesterEngine {
public:
    BacktesterEngine();
    ~BacktesterEngine();

    sep::workbench::backtester::BacktestResult run(const std::string& dataset_path);
};
