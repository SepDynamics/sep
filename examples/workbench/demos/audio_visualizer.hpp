#pragma once

#include "demo_manager.hpp"
#include "config.hpp"
#include "audio/pipeline.h"
#include "quantum/pattern_processor.hpp"

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
    std::unique_ptr<audio::AudioPipeline> audio_pipeline_;
    std::unique_ptr<pattern::PatternProcessor> pattern_processor_;
    std::queue<glm::vec3> pattern_queue_;
    float evolution_rate_{1.0f};
};

} // namespace workbench
} // namespace sep
