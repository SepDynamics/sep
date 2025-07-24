#include "signal_testing_panel.h"
#include "apps/sep_demo/sep_demo_app.hpp"
#include "apps/workbench/signal_generator/logistic_chaos_strategy.h"
#include "apps/workbench/signal_generator/signal_types.h"
#include <vector>
#include <string>

namespace sep {
namespace workbench {

SignalTestingPanel::SignalTestingPanel(
    std::shared_ptr<QuantumSignalGenerator> signal_generator)
    : signal_generator_(std::move(signal_generator)) {}

void SignalTestingPanel::render() {
    if (!ImGui::Begin(getName(), &visible_)) {
        ImGui::End();
        return;
    }

    if (testing_running_) {
        signal_generator_->tick();
    }

    renderControls();
    ImGui::Separator();
    renderSignalDisplay();
    ImGui::Separator();
    renderHistory();
    ImGui::Separator();
    renderPerformance();

    ImGui::End();
}

void SignalTestingPanel::renderControls() {
    ImGui::Text("Controls");

    if (ImGui::Button(testing_running_ ? "Stop Testing" : "Start Testing")) {
        testing_running_ = !testing_running_;
    }

    ImGui::Separator();
    ImGui::Text("Strategy");

    const char *strategies[] = {"Logistic Chaos"};
    if (ImGui::Combo("Strategy", &current_strategy_idx_, strategies,
                   IM_ARRAYSIZE(strategies))) {
        // In a real application, you would change the strategy in the
        // QuantumSignalGenerator.
    }

    if (current_strategy_idx_ == 0) { // LogisticChaosStrategy
        ImGui::SliderFloat("Chaos Parameter (r)", &chaos_r_, 0.0f, 4.0f);
        auto strategy = dynamic_cast<LogisticChaosStrategy*>(
            signal_generator_->getStrategy());
        if (strategy) {
            strategy->setR(chaos_r_);
        }
    }
}

void SignalTestingPanel::renderSignalDisplay() {
    ImGui::Text("Real-Time Signal");

    auto signal = signal_generator_->getCurrentSignal();

    if (!testing_running_) {
        ImGui::Text("Testing is stopped.");
        return;
    }

    const char *signal_text;
    ImVec4 signal_color;

    switch (signal.type) {
    case SignalType::BUY:
        signal_text = "BUY";
        signal_color = ImVec4(0.0f, 1.0f, 0.0f, 1.0f);
        break;
    case SignalType::SELL:
        signal_text = "SELL";
        signal_color = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
        break;
    default:
        signal_text = "HOLD";
        signal_color = ImVec4(1.0f, 1.0f, 0.0f, 1.0f);
        break;
    }

    ImGui::Text("Signal: ");
    ImGui::SameLine();
    ImGui::TextColored(signal_color, "%s", signal_text);

    ImGui::Text("Confidence: %.3f", signal.confidence);
}

void SignalTestingPanel::renderHistory() {
    ImGui::Text("Signal History");
    // Placeholder for history chart
    ImGui::Text("History chart will be implemented here.");
}

void SignalTestingPanel::renderPerformance() {
    ImGui::Text("Performance Metrics");
    ImGui::Text("Win Rate: %.2f%%", win_rate_ * 100.0f);
    ImGui::Text("P&L: $%.2f", pnl_);
}

} // namespace workbench
} // namespace sep