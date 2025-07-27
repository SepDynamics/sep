#include "alpha_demo_tab_controller.h"
#include "../core/workbench_core.hpp"

namespace sep::workbench {

AlphaDemoTabController::AlphaDemoTabController(WorkbenchEngine* engine) 
    : workbench_engine_(engine), alpha_tracker_(engine->getAlphaTracker()) {}

void AlphaDemoTabController::render() {
    if (!alpha_tracker_) {
        ImGui::Text("AlphaTracker not available.");
        return;
    }

    ImGui::Text("Strategy Pips Gained: %.2f", alpha_tracker_->getStrategyPipsGained());
    ImGui::Text("Benchmark Pips: %.2f", alpha_tracker_->getBenchmarkPips());
}

}
