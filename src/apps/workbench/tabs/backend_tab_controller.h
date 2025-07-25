#pragma once

#include <memory>
#include <string>
#include "../core/metrics_monitor.h"
#include "../backtester/backtester.h"
#include "../core/file_dialog.hpp"

namespace sep::workbench {

class BackendTabController {
public:
    BackendTabController();
    ~BackendTabController();

    void initialize();
    void render();
    void shutdown();

    void setMetricsMonitor(std::shared_ptr<MetricsMonitor> monitor);

private:
    void renderDataSourceSelector();
    void renderBacktesterPanel();

    void handleDataLoad();
    void handleClearData();
    void handleExportMetrics();
    void handleStartProcessing();
    void handleStopProcessing();

    std::shared_ptr<MetricsMonitor> monitor_;
    std::unique_ptr<backtester::Backtester> backtester_;
    FileDialog file_dialog_;

    // UI State
    char file_path_buffer_[512] = "";
    int data_source_type_{0};  // 0=File, 1=Live Stream, 2=Generated
    char export_path_buffer_[512] = "metrics_export.json";
};

} // namespace sep::workbench
