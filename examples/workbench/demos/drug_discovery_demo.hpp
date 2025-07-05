#pragma once

#include "demo_manager.hpp"
#include "sep_engine_wrapper.h"
#include <glm/vec3.hpp>
#include <vector>
#include <memory>

namespace sep {
namespace workbench {

class DrugDiscoveryDemo : public Demo {
public:
    void init() override;
    void update(float dt) override;
    void render() override;
    void cleanup() override;
    void handleKeyboard(unsigned char key) override;
    void handleMouse(int x, int y, int button) override;

private:
    struct Pose {
        glm::vec3 position{0.0f};
        glm::vec3 orientation{0.0f};
        float affinity{0.0f}; // Mock binding affinity
    };

    std::vector<Pose> poses_;

    struct OptimizerParams {
        int iterations{1};
        float mutation_rate{0.1f};
    } optimizer_;
};

} // namespace workbench
} // namespace sep
