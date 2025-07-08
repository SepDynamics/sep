#pragma once

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <vector>

#include "../demo_manager.hpp"

namespace sep {
namespace workbench {

// Simple Pattern type for demo mode
struct Pattern {
    glm::vec4 position{0.0f};
    glm::vec4 velocity{0.0f};
    glm::vec4 attributes{1.0f};  // x = mass, y = temperature
    float coherence{1.0f};
};

class CosmoSim : public Demo {
public:
    void init() override;
    void update(float dt) override;
    void render() override;
    void cleanup() override;
    void handleKeyboard(unsigned char key) override;
    void handleMouse(int x, int y, int button) override;

private:
    std::vector<Pattern> bodies_;
    float box_size_{100.0f};
    float time_step_{0.01f};
    std::size_t particle_count_{100};

    void initBodies();
    void integrate(float dt);
};

}  // namespace workbench
}  // namespace sep
