#pragma once

#include "apps/workbench/signal_generator/quantum_signal_generator.h"
#include "apps/workbench/signal_generator/signal_types.h"
#include "demo_panel.h"
#include "imgui.h"
#include <memory>

namespace sep {
namespace workbench {

class SignalTestingPanel : public demo::DemoPanel {
public:
    explicit SignalTestingPanel(
        std::shared_ptr<QuantumSignalGenerator> signal_generator);
    ~SignalTestingPanel() = default;

    void render();

    const char *getName() const { return "Signal Testing"; }

private:
    void renderControls();
    void renderSignalDisplay();
    void renderHistory();
    void renderPerformance();

    std::shared_ptr<QuantumSignalGenerator> signal_generator_;
    bool visible_ = true;
    bool testing_running_ = false;

    // Strategy configuration
    int current_strategy_idx_ = 0;
    // Parameters for LogisticChaosStrategy
    float chaos_r_ = 3.7f;

    // Performance metrics
    float win_rate_ = 0.0f;
    float pnl_ = 0.0f;
};

} // namespace workbench
} // namespace sep