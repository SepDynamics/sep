#include "signal_testing_panel.h"
#include "apps/sep_demo/sep_demo_app.hpp"
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
    ImGui::Text("Market Data Signals");
    ImGui::Text("Signals generated from real OANDA market data processing");
    ImGui::Text("through SEP quantum pattern analysis");
    
    // Real market data signal parameters - not fake generation
    if (ImGui::CollapsingHeader("Signal Thresholds")) {
        ImGui::SliderFloat("Coherence Threshold", &coherence_threshold_, 0.0f, 1.0f);
        ImGui::SliderFloat("Stability Threshold", &stability_threshold_, 0.0f, 1.0f);
        ImGui::SliderFloat("Entropy Threshold", &entropy_threshold_, 0.0f, 1.0f);
        ImGui::Text("These thresholds control when patterns in real market data");
        ImGui::Text("trigger BUY/SELL/HOLD signals");
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
    
    // Collect recent signals for visualization
    static std::vector<float> signal_values;
    static std::vector<float> confidence_values;
    static std::vector<const char*> signal_labels;
    static size_t history_size = 50;
    
    // Get current signal from real market data processing
    auto signal = signal_generator_->getCurrentSignal();
    if (signal.confidence > 0.0f) {
        
        // Convert signal type to numeric value for plotting
        float signal_value = 0.0f;
        const char* signal_label = "HOLD";
        switch (signal.type) {
            case SignalType::BUY:
                signal_value = 1.0f;
                signal_label = "BUY";
                break;
            case SignalType::SELL:
                signal_value = -1.0f;
                signal_label = "SELL";
                break;
            case SignalType::HOLD:
                signal_value = 0.0f;
                signal_label = "HOLD";
                break;
        }
        
        signal_values.push_back(signal_value);
        confidence_values.push_back(signal.confidence);
        signal_labels.push_back(signal_label);
        
        // Maintain history size
        while (signal_values.size() > history_size) {
            signal_values.erase(signal_values.begin());
            confidence_values.erase(confidence_values.begin());
            signal_labels.erase(signal_labels.begin());
        }
    }
    
    // Display signal trend as text-based chart (simple implementation)
    if (!signal_values.empty()) {
        ImGui::Separator();
        ImGui::Text("Recent Signals (newest first):");
        
        // Show last 10 signals with colors
        size_t start_idx = signal_values.size() > 10 ? signal_values.size() - 10 : 0;
        for (size_t i = signal_values.size(); i-- > start_idx;) {
            ImVec4 color;
            const char* symbol;
            
            if (signal_values[i] > 0.5f) {
                color = ImVec4(0.0f, 0.8f, 0.0f, 1.0f); // Green for BUY
                symbol = "▲";
            } else if (signal_values[i] < -0.5f) {
                color = ImVec4(0.8f, 0.0f, 0.0f, 1.0f); // Red for SELL
                symbol = "▼";
            } else {
                color = ImVec4(0.7f, 0.7f, 0.7f, 1.0f); // Gray for HOLD
                symbol = "■";
            }
            
            ImGui::TextColored(color, "%s %s (Conf: %.2f)", 
                              symbol, signal_labels[i], confidence_values[i]);
        }
        
        // Simple ASCII chart of signal strength over time
        ImGui::Separator();
        ImGui::Text("Signal Strength Chart (last 20):");
        
        size_t chart_start = signal_values.size() > 20 ? signal_values.size() - 20 : 0;
        std::string chart_line = "";

        for (size_t i = chart_start; i < signal_values.size(); ++i) {
            if (signal_values[i] > 0.5f) {
                chart_line += "▲"; // BUY
            } else if (signal_values[i] < -0.5f) {
                chart_line += "▼"; // SELL
            } else {
                chart_line += "■"; // HOLD
            }
        }
        
        ImGui::Text("%s", chart_line.c_str());
        
        // Confidence trend
        ImGui::Text("Confidence Trend:");
        std::string confidence_chart = "";
        for (size_t i = chart_start; i < confidence_values.size(); ++i) {
            if (confidence_values[i] > 0.8f) {
                confidence_chart += "█"; // High confidence
            } else if (confidence_values[i] > 0.6f) {
                confidence_chart += "▓"; // Medium confidence
            } else if (confidence_values[i] > 0.4f) {
                confidence_chart += "▒"; // Low confidence
            } else {
                confidence_chart += "░"; // Very low confidence
            }
        }
        ImGui::Text("%s", confidence_chart.c_str());
        
        // Statistics
        if (signal_values.size() >= 3) {
            int buy_count = 0, sell_count = 0, hold_count = 0;
            float avg_confidence = 0.0f;
            
            for (size_t i = 0; i < signal_values.size(); ++i) {
                if (signal_values[i] > 0.5f) buy_count++;
                else if (signal_values[i] < -0.5f) sell_count++;
                else hold_count++;
                avg_confidence += confidence_values[i];
            }
            avg_confidence /= signal_values.size();
            
            ImGui::Separator();
            ImGui::Text("Statistics:");
            ImGui::Text("BUY: %d, SELL: %d, HOLD: %d", buy_count, sell_count, hold_count);
            ImGui::Text("Average Confidence: %.3f", avg_confidence);
        }
    } else {
        ImGui::Text("No signal history available. Generate signals to see chart.");
    }
}

void SignalTestingPanel::renderPerformance() {
    ImGui::Text("Performance Metrics");
    ImGui::Text("Win Rate: %.2f%%", win_rate_ * 100.0f);
    ImGui::Text("P&L: $%.2f", pnl_);
}

} // namespace workbench
} // namespace sep