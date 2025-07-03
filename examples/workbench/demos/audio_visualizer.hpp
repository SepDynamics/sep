#pragma once
#include "../demo_manager.hpp"
#include "config.hpp"
#include <queue>
#include <memory>
#include <glm/glm.hpp>

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
    void processQueue();
    std::queue<glm::vec3> pattern_queue_;
    audio::AudioPipeline audio_pipeline_;
    std::unique_ptr<sep::pattern::PatternProcessor> pattern_processor_;
    std::unique_ptr<sep::memory::QuantumCoherenceManager> coherence_manager_;
    bool auto_evolve_{true};
};

} // namespace workbench
} // namespace sep
