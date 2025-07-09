#pragma once

#include "demo_manager.hpp"
#include "sep_engine_wrapper.h"
#include <vector>
#include <glm/vec3.hpp>

namespace sep {
namespace workbench {

struct Pose {
    glm::vec3 position{0.0f};
    glm::vec3 orientation{0.0f};
    float binding_affinity{0.0f};
};

class DrugDiscoveryDemo : public Demo {
public:
    void on_load() override;
    void on_update(float dt) override;
    void on_render() override;
    void on_unload() override;
    void on_key_press(int key) override;
    void on_mouse(int x, int y, int button) override;

private:
    void optimizePoses();

    std::vector<Pose> poses_;
    int iterations_{10};
    float mutation_rate_{0.05f};
};

} // namespace workbench
} // namespace sep

