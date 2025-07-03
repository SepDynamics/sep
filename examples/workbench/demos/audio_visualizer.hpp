#pragma once

#include "examples/workbench/demo_manager.hpp"
#include <memory>
#include <vector>

namespace sep {
namespace workbench {

namespace audio {
class AudioCapture;
class AudioPipeline;
}

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
};

} // namespace workbench
} // namespace sep
