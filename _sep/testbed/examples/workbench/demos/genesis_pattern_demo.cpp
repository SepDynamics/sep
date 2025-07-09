#include "genesis_pattern_demo.hpp"

namespace sep {
namespace workbench {

void GenesisPatternDemo::on_load() {
    auto_evolve_ = true;
    evolution_rate_ = 0.1f;
}

void GenesisPatternDemo::on_update(float dt) {
    if (auto_evolve_) {
        // placeholder for evolution logic
        (void)dt; // suppress unused warning
    }
}

void GenesisPatternDemo::on_render() {
    ImGui::Begin("Genesis Controls");
    ImGui::Checkbox("Auto Evolve", &auto_evolve_);
    ImGui::SliderFloat("Evolution Rate", &evolution_rate_, 0.01f, 1.0f);
    ImGui::End();
}

void GenesisPatternDemo::on_key_press(int key) {
    if (key == ' ') auto_evolve_ = !auto_evolve_;
}

} // namespace workbench
} // namespace sep
