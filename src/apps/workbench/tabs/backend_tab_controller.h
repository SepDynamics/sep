#pragma once

#include <memory>
#include <string>
#include "../core/metrics_monitor.h"
#include "../backtester/backtester.h"
#include "../core/file_dialog.hpp"
#include "../core/service_connector.hpp"
#include "../core/trade_manager.h"
#include "../core/ui_layout_manager.h"
#include "connectors/oanda_connector.h"
#include "../backtester/data/data_loader.h"
#include "quantum/pattern_metric_engine.h"
#include "../core/service_proxy_engine.h"
#include <vector>
#include <fstream>

namespace sep::workbench {

class BackendTabController {
public:
    BackendTabController();
    ~BackendTabController();

    bool initialize();
    void render();
    void shutdown();

    void setMetricsMonitor(std::shared_ptr<MetricsMonitor> monitor);
    void setServiceConnector(ServiceConnector* connector);
    void setTradeManager(TradeManager* manager) { trade_manager_ = manager; }

private:
    ServiceConnector* service_connector_ = nullptr;
    TradeManager* trade_manager_ = nullptr;
    void renderDataSourceSelector();
    void renderBacktesterPanel();
    void renderOrderManagementPanel();
    void renderTradeHistoryPanel();
    void updateTradeHistory();

    void handleDataLoad();
    void handleClearData();
    void handleExportMetrics();
    void handleStartProcessing();
    void handleStopProcessing();

    std::shared_ptr<MetricsMonitor> monitor_;
    std::unique_ptr<backtester::Backtester> backtester_;
    std::unique_ptr<sep::workbench::backtester::DataLoader> data_loader_;
    std::unique_ptr<sep::quantum::PatternMetricEngine> pattern_engine_;
    FileDialog file_dialog_;

    // UI State
    char file_path_buffer_[512] = "eur_usd_m1_48h.json";
    char backtest_file_buffer_[512] = "eur_usd_m1_48h.json";
    int data_source_type_{0};  // 0=File, 1=Live Stream, 2=Generated
    char export_path_buffer_[512] = "metrics_export.json";
    float risk_percentage_ = 2.0f;
    float stop_loss_pips_ = 20.0f;
    float take_profit_pips_ = 40.0f;
    sep::workbench::SignalValidator::ValidationResult validation_result_;
    char instrument_buffer_[64] = "EUR_USD";
    int units_ = 1000;
    nlohmann::json open_positions_;
    nlohmann::json orders_;
    std::vector<sep::connectors::OrderInfo> order_cache_;
    bool paper_trading_ = false;

    std::vector<std::string> trade_history_;
    std::vector<float> equity_curve_;
};

} // namespace sep::workbench
