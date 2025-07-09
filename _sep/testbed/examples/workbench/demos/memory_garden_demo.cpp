#include "memory_garden_demo.hpp"
#include <cmath>

namespace sep {
namespace workbench {

void MemoryGardenDemo::on_load() {
    nodes_.clear();
    for (int i = 0; i < 5; ++i) {
        float angle = i * 0.5f;
        nodes_.push_back({{tier_radius_[0] * std::cos(angle), 0.f, tier_radius_[0] * std::sin(angle)}});
    }
}

void MemoryGardenDemo::on_update(float dt) {
    (void)dt;
    // simple orbit update
    for (size_t i = 0; i < nodes_.size(); ++i) {
        float angle = static_cast<float>(i) * 0.5f + dt;
        nodes_[i].pos.x = tier_radius_[i % 3] * std::cos(angle);
        nodes_[i].pos.z = tier_radius_[i % 3] * std::sin(angle);
    }
}

void MemoryGardenDemo::on_render() {
    ImGui::Begin("Memory Garden");
    ImGui::SliderFloat3("Tier Radii", tier_radius_, 1.f, 20.f);
    ImGui::Text("Node count: %zu", nodes_.size());
    ImGui::End();
}

} // namespace workbench
} // namespace sep
