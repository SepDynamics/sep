#include "backend_tab_controller.h"

#include <filesystem>
#include <iostream>

#include "imgui.h"

namespace sep::workbench {

BackendTabController::BackendTabController()
    : backtester_(std::make_unique<backtester::Backtester>()) {}

BackendTabController::~BackendTabController() { shutdown(); }
bool BackendTabController::initialize() {
    std::cout << "[BackendTabController] Initializing..." << std::endl;
    return true;
}

void BackendTabController::render() {
    renderDataSourceSelector();
    ImGui::Separator();
    renderBacktesterPanel();

    std::string selected;
    if (file_dialog_.render(selected)) {
        strncpy(file_path_buffer_, selected.c_str(), sizeof(file_path_buffer_) - 1);
        file_path_buffer_[sizeof(file_path_buffer_) - 1] = '\0';
    }
}

void BackendTabController::shutdown() {}

void BackendTabController::setMetricsMonitor(std::shared_ptr<MetricsMonitor> monitor) {
    monitor_ = monitor;
}

void BackendTabController::setServiceConnector(ServiceConnector* connector) {
    service_connector_ = connector;
}

void BackendTabController::renderDataSourceSelector() {
    const char* source_types[] = {"File", "Live Stream", "Generated Data"};
    ImGui::Combo("Source Type", &data_source_type_, source_types, 3);

    switch (data_source_type_) {
        case 0: // File
            ImGui::InputText("File Path", file_path_buffer_, sizeof(file_path_buffer_));
            ImGui::SameLine();
            if (ImGui::Button("Browse")) {
                file_dialog_.open(std::filesystem::path(file_path_buffer_).empty() ? "." : std::filesystem::path(file_path_buffer_).parent_path().string());
            }
            if (ImGui::Button("Load File", ImVec2(-1, 0))) {
                handleDataLoad();
            }
            break;
        case 1: // Live Stream
            ImGui::Text("Live stream not yet implemented");
            break;
        case 2: // Generated
            if (ImGui::Button("Generate Test Data", ImVec2(-1, 0))) {
                std::vector<uint8_t> test_data(1024);
                for (size_t i = 0; i < test_data.size(); ++i) {
                    test_data[i] = static_cast<uint8_t>(i % 256);
                }
                if(monitor_) monitor_->ingestData(test_data.data(), test_data.size());
            }
            break;
    }
}

void BackendTabController::renderBacktesterPanel() {
    ImGui::Begin("Backtester");
    if (ImGui::Button("Run Backtest")) {
        std::vector<float> prices = {1.0, 1.1, 1.2, 1.1, 1.0};
        std::vector<MetricsMonitor::ThresholdSignal> signals;
        backtester_->run(prices, signals);
    }

    const auto& result = backtester_->getResult();
    ImGui::Text("Total Trades: %d", result.total_trades);
    ImGui::Text("Win Rate: %.2f", result.win_rate);
    ImGui::Text("Total PnL: %.2f", result.total_pnl);
    ImGui::Text("Sharpe Ratio: %.2f", result.sharpe_ratio);
    ImGui::Text("Max Drawdown: %.2f", result.max_drawdown);

    ImGui::End();
}

void BackendTabController::handleDataLoad() {
    if (monitor_ && strlen(file_path_buffer_) > 0) {
        monitor_->ingestFile(file_path_buffer_);
    }
}

void BackendTabController::handleClearData() {
    if(monitor_) monitor_->clear();
}

void BackendTabController::handleExportMetrics() {
    if(monitor_) monitor_->saveMetricsToFile(export_path_buffer_);
}

void BackendTabController::handleStartProcessing() {
    if(monitor_) monitor_->startProcessing();
}

void BackendTabController::handleStopProcessing() {
    if(monitor_) monitor_->stopProcessing();
}

} // namespace sep::workbench
