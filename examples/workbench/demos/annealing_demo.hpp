#pragma once

#include "demo_manager.hpp"
#include "sep_engine_wrapper.h"
#include <vector>
#include <glm/vec3.hpp>
#include <memory/types.h>

namespace sep {
namespace workbench {

class AnnealingDemo : public Demo {
public:
    void init() override;
    void update(float dt) override;
    void render() override;
    void cleanup() override;
    void handleKeyboard(unsigned char key) override;
    void handleMouse(int x, int y, int button) override;

private:
    struct Particle {
        persistence::PatternData data;
        glm::vec3 velocity{0.0f};
    };

    std::vector<Particle> particles_;
    float temperature_{1.0f};
    float cooling_rate_{0.99f};
};

} // namespace workbench
} // namespace sep

