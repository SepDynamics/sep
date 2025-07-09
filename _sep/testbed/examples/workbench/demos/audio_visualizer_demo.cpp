#include "audio_visualizer_demo.hpp"

namespace sep {
namespace workbench {

void AudioVisualizerDemo::on_load() {
    sensitivity_ = 1.0f;
}

void AudioVisualizerDemo::on_update(float dt) {
    (void)dt; // placeholder
}

void AudioVisualizerDemo::on_render() {
    ImGui::Begin("Audio Visualizer");
    ImGui::SliderFloat("Sensitivity", &sensitivity_, 0.1f, 5.0f);
    ImGui::End();
}

} // namespace workbench
} // namespace sep
