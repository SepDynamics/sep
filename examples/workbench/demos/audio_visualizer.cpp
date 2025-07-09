#include "audio_visualizer.hpp"
#include <imgui.h>
#include <cmath>

namespace sep {
namespace workbench {

void AudioVisualizerDemo::on_load() {
    spectrum_.assign(64, 0.f);
}

void AudioVisualizerDemo::on_update(float dt) {
    for (size_t i = 0; i < spectrum_.size(); ++i) {
        spectrum_[i] = 0.5f + 0.5f*std::sin(static_cast<float>(i) + dt);
    }
}

void AudioVisualizerDemo::on_render() {
    ImGui::Begin("Audio Visualizer");
    ImGui::SliderFloat("Sensitivity", &sensitivity_, 0.1f, 5.0f);
    ImGui::Text("Spectrum bins: %zu", spectrum_.size());
    ImGui::End();
}

} // namespace workbench
} // namespace sep
