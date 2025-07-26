#pragma once

#include "apps/workbench/backtester/core/backtester_engine.h"
#include <memory>

class BacktesterTabController {
public:
    BacktesterTabController();
    ~BacktesterTabController();

    void render();

private:
    std::unique_ptr<BacktesterEngine> engine_;
    sep::workbench::backtester::BacktestResult result_{};
    char dataset_path_[512] = "eur_usd_m1_48h.json";
};
