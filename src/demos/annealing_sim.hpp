#pragma once

#include "demo_manager.hpp"
#include "pattern_processor.hpp"
#include <memory>
#include <vector>
#include <glm/vec3.hpp>

namespace sep {
namespace workbench {

class PatternProcessor;
class QuantumCoherenceManager;

class AnnealingSimDemo : public Demo {
public:
    void init() override;
    void update(float dt) override;
    void render() override;
    void cleanup() override;
    void handleKeyboard(unsigned char key) override;
    void handleMouse(int x, int y, int button) override;

private:
    struct Particle {
        glm::vec3 position{0.0f};
        glm::vec3 velocity{0.0f};
        glm::vec3 color{1.0f};
    };

    std::unique_ptr<PatternProcessor> processor_;
    std::unique_ptr<QuantumCoherenceManager> coherence_mgr_;
    std::vector<Particle> particles_;
    std::vector<float> temperature_schedule_;
    std::size_t current_step_{0};
    bool paused_{false};
};

} // namespace workbench
} // namespace sep
