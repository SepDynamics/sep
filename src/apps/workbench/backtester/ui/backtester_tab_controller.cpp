#include "backtester_tab_controller.h"

#include "imgui.h"
#include "implot.h"
#include <iostream>
#include "apps/workbench/backtester/strategies/sep_signal_strategy.h"

BacktesterTabController::BacktesterTabController()
    : engine_(std::make_unique<BacktesterEngine>()) {
    pattern_engine_.init(nullptr);
}

BacktesterTabController::~BacktesterTabController() {
}

void BacktesterTabController::render() {
    ImGui::Begin("Backtester");
    ImGui::InputText("Dataset", dataset_path_, sizeof(dataset_path_));
    ImGui::SameLine();
    if (ImGui::Button("Browse")) {
        file_dialog_.open();
    }
    std::string selected;
    if (file_dialog_.render(selected)) {
        strncpy(dataset_path_, selected.c_str(), sizeof(dataset_path_) - 1);
        dataset_path_[sizeof(dataset_path_) - 1] = '\0';
    }
    ImGui::Combo("Strategy", &strategy_index_,
                [](void* data, int idx, const char** out_text) {
                    auto* vec = static_cast<std::vector<std::string>*>(data);
                    if (idx < 0 || idx >= static_cast<int>(vec->size())) return false;
                    *out_text = (*vec)[idx].c_str();
                    return true;
                },
                &strategy_names_, strategy_names_.size());

    if (!running_) {
        if (ImGui::Button("Start")) {
            running_ = true;
            SEPSignalStrategy strategy;
            backtester::BaseStrategy* strat_ptr = nullptr;
            if (strategy_index_ == 0) {
                strat_ptr = &strategy;
            }
            result_ = engine_->run(dataset_path_, &pattern_engine_, strat_ptr);
            globalEventBus().publish(BacktestResultEvent{result_});
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
