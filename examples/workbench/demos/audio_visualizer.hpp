#pragma once

#include <memory>
#include <cstddef>
#include "../demo_manager.hpp"
#include "../config.hpp"
#include "audio/capture.h"
#include "audio/factory.h"
#include "quantum/processor.h"

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
    void processAudio(const float* data, size_t frames);

    std::unique_ptr<audio::AudioCapture>     audio_capture_;
    std::unique_ptr<pattern::PatternProcessor> pattern_processor_;
    float evolution_rate_{1.0f};
};

} // namespace workbench
} // namespace sep
