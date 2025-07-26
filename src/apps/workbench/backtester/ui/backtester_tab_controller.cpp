#include "backtester_tab_controller.h"

#include "imgui.h"
#include "implot.h"
#include <iostream>

BacktesterTabController::BacktesterTabController()
    : engine_(std::make_unique<BacktesterEngine>()) {}

BacktesterTabController::~BacktesterTabController() {
}

void BacktesterTabController::render() {
    ImGui::Begin("Backtester");
    ImGui::InputText("Dataset", dataset_path_, sizeof(dataset_path_));

    if (!running_) {
        if (ImGui::Button("Start")) {
            running_ = true;
            result_ = engine_->run(dataset_path_);
            running_ = false;
        }
    } else {
        if (ImGui::Button("Stop")) {
            running_ = false;
        }
    }

    ImGui::Separator();
    ImGui::Text("Total Trades: %d", result_.total_trades);
    ImGui::Text("Win Rate: %.2f", result_.win_rate);
    ImGui::Text("Total PnL: %.2f", result_.total_pnl);
    ImGui::Text("Sharpe Ratio: %.2f", result_.sharpe_ratio);
    ImGui::Text("Max Drawdown: %.2f", result_.max_drawdown);

    if (!result_.trades.empty()) {
        std::vector<double> xs(result_.trades.size());
        std::vector<double> curve(result_.trades.size());
        double cumulative = 0.0;
        for (size_t i = 0; i < result_.trades.size(); ++i) {
            const auto& t = result_.trades[i];
            double pnl = (t.type == sep::quantum::SignalType::BUY)
                            ? t.exit_price - t.entry_price
                            : t.entry_price - t.exit_price;
            cumulative += pnl;
            xs[i] = static_cast<double>(i);
            curve[i] = cumulative;
        }

        if (ImPlot::BeginPlot("Profit Curve", ImVec2(-1,150))) {
            ImPlot::PlotLine("PnL", xs.data(), curve.data(), static_cast<int>(curve.size()));
            ImPlot::EndPlot();
        }

        if (ImGui::BeginTable("Trades", 5, ImGuiTableFlags_Borders)) {
            ImGui::TableSetupColumn("#");
            ImGui::TableSetupColumn("Type");
            ImGui::TableSetupColumn("Entry");
            ImGui::TableSetupColumn("Exit");
            ImGui::TableSetupColumn("PnL");
            ImGui::TableHeadersRow();
            for (size_t i = 0; i < result_.trades.size(); ++i) {
                const auto& t = result_.trades[i];
                double pnl = (t.type == sep::quantum::SignalType::BUY)
                                ? t.exit_price - t.entry_price
                                : t.entry_price - t.exit_price;
                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);
                ImGui::Text("%zu", i);
                ImGui::TableSetColumnIndex(1);
                ImGui::Text("%s", t.type == sep::quantum::SignalType::BUY ? "BUY" : "SELL");
                ImGui::TableSetColumnIndex(2);
                ImGui::Text("%.4f", t.entry_price);
                ImGui::TableSetColumnIndex(3);
                ImGui::Text("%.4f", t.exit_price);
                ImGui::TableSetColumnIndex(4);
                ImGui::Text("%.4f", pnl);
            }
            ImGui::EndTable();
        }
    }

    ImGui::End();
}
