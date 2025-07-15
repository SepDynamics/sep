#pragma once

#include <glm/vec3.hpp>
#include <memory>
#include <vector>

#include "audio/capture.h"
#include "audio/pipeline.h"
#include "demo_manager.hpp"
#include "workbench/demos/demo_base.hpp"
#include "imgui.h"

namespace sep {
namespace workbench {

class AudioVisualizerDemo : public Demo {
public:
    void on_load(sep::core::Engine* engine, sep::blender::CyclesRenderer* renderer) override;
    void on_update(float dt) override;
    void on_render() override;
    void on_ui_render() override;
    void on_unload() override;
    void on_key_press(int key) override;
    void on_mouse(int x, int y, int button);

private:
    sep::core::Engine* engine_{nullptr};
    sep::blender::CyclesRenderer* renderer_{nullptr};
    std::unique_ptr<audio::AudioCapture> capture_;
    std::unique_ptr<audio::AudioPipeline> pipeline_;
    std::vector<glm::vec3> latest_patterns_;
    std::vector<glm::vec3> latest_visual_patterns_;

    struct {
        float frequency_scale{0.5f};
        float amplitude_scale{1.0f};
        float evolution_sensitivity{0.75f};
    } pattern_mapping_;
    
};

} // namespace workbench
} // namespace sep
