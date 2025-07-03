#pragma once
#include "../demo_manager.hpp"
#include "config.hpp"
#include <memory>
#include <glm/glm.hpp>

namespace sep {
namespace workbench {

class MemoryGardenDemo : public Demo {
public:
    void init() override;
    void update(float dt) override;
    void render() override;
    void cleanup() override;
    void handleKeyboard(unsigned char key) override;
    void handleMouse(int x, int y, int button) override;

private:
    void spawnInitial();
    std::unique_ptr<sep::pattern::PatternProcessor> pattern_processor_;
    std::unique_ptr<sep::memory::QuantumCoherenceManager> coherence_manager_;
    bool auto_evolve_{true};
};

} // namespace workbench
} // namespace sep
