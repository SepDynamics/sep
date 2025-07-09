#pragma once

#include "demo_manager.hpp"
#include "audio/capture.h"
#include "audio/pipeline.h"
#include <memory>
#include <vector>
#include <glm/vec3.hpp>

namespace sep {
namespace workbench {


class AudioVisualizerDemo : public Demo {
public:
    void init() override;
    void update(float dt) override;
    void render() override;
    void cleanup() override;
    void handleKeyboard(unsigned char key) override;
    void handleMouse(int x, int y, int button) override;

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
