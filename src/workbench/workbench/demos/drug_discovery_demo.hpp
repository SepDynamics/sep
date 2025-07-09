#pragma once

#include "../demo_manager.hpp"
#include <glm/vec3.hpp>
#include <vector>

namespace sep {
namespace workbench {

struct Pose {
    glm::vec3 position{0.0f};
    glm::vec3 orientation{0.0f};
    float binding_affinity{0.0f};
};

class DrugDiscoveryDemo : public Demo {
public:
    void init() override;
    void update(float dt) override;
    void render() override;
    void cleanup() override;
    void handleKeyboard(unsigned char key) override;
    void handleMouse(int x, int y, int button) override;

private:
    std::vector<Pose> candidate_poses_;
    int optimizer_iterations_{100};
    float mutation_rate_{0.1f};
};

} // namespace workbench
} // namespace sep
