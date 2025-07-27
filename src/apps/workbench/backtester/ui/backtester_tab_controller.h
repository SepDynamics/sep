#pragma once

#include "apps/workbench/backtester/core/backtester_engine.h"
#include "apps/workbench/backtester/strategies/base_strategy.h"
#include "apps/workbench/core/file_dialog.hpp"
#include "apps/workbench/core/ui_layout_manager.h"
#include "connectors/oanda_connector.h"
#include <memory>
#include <vector>

class BacktesterTabController {
public:
    BacktesterTabController();
    ~BacktesterTabController();

    void render();

    void setPatternMetricEngine(sep::quantum::PatternMetricEngine* engine);
    void setOandaConnector(sep::connectors::OandaConnector* connector);

private:
    std::unique_ptr<BacktesterEngine> engine_;
    sep::quantum::PatternMetricEngine pattern_engine_;
    sep::quantum::PatternMetricEngine* pattern_engine_ptr_ = nullptr;
    sep::connectors::OandaConnector* oanda_connector_ = nullptr;
    sep::quantum::GPUContext gpu_context_{};
    bool use_gpu_ = false;
    std::vector<std::string> strategy_names_{"SEP Signal"};
    int strategy_index_ = 0;
    sep::workbench::FileDialog file_dialog_;
    sep::workbench::backtester::BacktestResult result_{};
    bool running_ = false;
    char dataset_path_[512] = "eur_usd_m1_48h.json";
};
