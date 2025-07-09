#include "memory_garden.hpp"
#include <imgui.h>
#include <cstdlib>

namespace sep {
namespace workbench {

void MemoryGardenDemo::on_load() {
    nodes_.clear();
    for (int i=0;i<5;++i) nodes_.push_back({static_cast<float>(i)/5.f});
}

void MemoryGardenDemo::on_update(float dt) {
    for (auto& n : nodes_) {
        n.coherence += ((std::rand()%100)/100.f - 0.5f)*dt;
    }
}

void MemoryGardenDemo::on_render() {
    ImGui::Begin("Memory Garden");
    ImGui::Text("Node count: %zu", nodes_.size());
    ImGui::SliderFloat("STM radius", &stm_radius_, 5.f, 50.f);
    ImGui::End();
}

} // namespace workbench
} // namespace sep
