#include "genesis_pattern.hpp"
#include <imgui.h>

namespace sep {
namespace workbench {

void GenesisPatternDemo::on_load() {
    dummy_state_.assign(100, 0.5f);
}

void GenesisPatternDemo::evolve(float dt) {
    for (auto& v : dummy_state_) v += dt * evolution_rate_;
}

void GenesisPatternDemo::on_update(float dt) {
    if (auto_evolve_) evolve(dt);
}

void GenesisPatternDemo::on_render() {
    ImGui::Begin("Genesis Pattern");
    ImGui::SliderFloat("Evolution Rate", &evolution_rate_, 0.01f, 1.0f);
    ImGui::Checkbox("Auto evolve", &auto_evolve_);
    ImGui::Text("Patterns: %zu", dummy_state_.size());
    ImGui::End();
}

} // namespace workbench
} // namespace sep
