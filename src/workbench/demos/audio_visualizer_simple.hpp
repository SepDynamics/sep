#pragma once

#include <vector>

#include "../../workbench_demo_adapter.hpp"
#include "audio/capture.h"
#include "demo_manager.hpp"
#include "imgui.h"

namespace sep {
namespace workbench {

class AudioVisualizerDemo : public Demo {
public:
    void on_load(sep::Engine* engine, sep::CyclesRenderer* renderer) override;
    void on_ui_render() override;
    void on_update(float dt) override;
    void on_render() override;
    void on_unload() override;
    void on_key_press(int key) override;
    void on_mouse(int x, int y, int button) override;

private:
    sep::audio::Capture* audio_capture_ = nullptr;
    std::vector<float> spectrum_;
    float sensitivity_{1.0f};
    float threshold_{1.0f};
    float decay_{0.1f};
    float input_strength_{0.5f};
    float learning_rate_{0.05f};
    float connection_prob_{0.2f};
};

} // namespace workbench
} // namespace sep