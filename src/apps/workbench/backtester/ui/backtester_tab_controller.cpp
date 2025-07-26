#include "backtester_tab_controller.h"

#include "imgui.h"
#include <iostream>

BacktesterTabController::BacktesterTabController()
    : engine_(std::make_unique<BacktesterEngine>()) {}

BacktesterTabController::~BacktesterTabController() {
}

void BacktesterTabController::render() {
    ImGui::Begin("Backtester");
    ImGui::InputText("Dataset", dataset_path_, sizeof(dataset_path_));
    if (ImGui::Button("Run")) {
        result_ = engine_->run(dataset_path_);
    }
    ImGui::Text("Total Trades: %d", result_.total_trades);
    ImGui::Text("Win Rate: %.2f", result_.win_rate);
    ImGui::Text("Total PnL: %.2f", result_.total_pnl);
    ImGui::Text("Sharpe Ratio: %.2f", result_.sharpe_ratio);
    ImGui::Text("Max Drawdown: %.2f", result_.max_drawdown);
    ImGui::End();
}
