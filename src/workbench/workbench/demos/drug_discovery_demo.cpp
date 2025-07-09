#include "drug_discovery_demo.hpp"
#include <config.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/norm.hpp>
#include <cstdlib>

namespace sep {
namespace workbench {

void DrugDiscoveryDemo::init() {
#ifdef SEP_WORKBENCH_DEMO
    optimizer_iterations_ = 100;
    mutation_rate_ = 0.1f;
#else
    const auto& cfg = getConfigManager().optimizer();
    optimizer_iterations_ = cfg.iterations;
    mutation_rate_ = cfg.mutation_rate;
#endif

    candidate_poses_.resize(5);
    for (auto& pose : candidate_poses_) {
        pose.position = glm::vec3(static_cast<float>(std::rand()) / RAND_MAX,
                                  static_cast<float>(std::rand()) / RAND_MAX,
                                  static_cast<float>(std::rand()) / RAND_MAX);
        pose.orientation = glm::vec3(static_cast<float>(std::rand()) / RAND_MAX,
                                     static_cast<float>(std::rand()) / RAND_MAX,
                                     static_cast<float>(std::rand()) / RAND_MAX);
        pose.binding_affinity = 0.0f;
    }
}

void DrugDiscoveryDemo::update(float) {
    for (int i = 0; i < optimizer_iterations_; ++i) {
        for (auto& pose : candidate_poses_) {
            glm::vec3 delta((std::rand() % 100 - 50) / 1000.0f * mutation_rate_,
                            (std::rand() % 100 - 50) / 1000.0f * mutation_rate_,
                            (std::rand() % 100 - 50) / 1000.0f * mutation_rate_);
            pose.position += delta;
            pose.orientation += delta * 0.5f;
            pose.binding_affinity = 1.0f / (1.0f + glm::length2(pose.position));
        }
    }
}

void DrugDiscoveryDemo::render() {
#ifdef SEP_WORKBENCH_DEMO
    if (renderer_) {
        std::vector<glm::vec3> pts;
        for (const auto& p : candidate_poses_) pts.push_back(p.position);
        renderer_->renderPatternState(pts);
    }
#else
    if (!renderer_) return;
    std::vector<glm::vec3> pts;
    for (const auto& p : candidate_poses_) pts.push_back(p.position);
    renderer_->renderPatternState(pts);
#endif
}

void DrugDiscoveryDemo::cleanup() {
    candidate_poses_.clear();
}

void DrugDiscoveryDemo::handleKeyboard(unsigned char) {}
void DrugDiscoveryDemo::handleMouse(int, int, int) {}

} // namespace workbench
} // namespace sep
