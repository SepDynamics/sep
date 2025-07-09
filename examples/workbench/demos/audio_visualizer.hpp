#pragma once

#include "demo_manager.hpp"
#include <memory>
#include <vector>
#include <glm/vec3.hpp>
#include "audio/capture.h"
#include "audio/pipeline.h"

namespace sep {
namespace workbench {

class AudioVisualizerDemo : public Demo {
public:
    void on_load() override;
    void on_update(float dt) override;
    void on_render() override;
    void on_unload() override;
    void on_key_press(unsigned char key) override;
    void on_mouse(int x, int y, int button);

private:
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
