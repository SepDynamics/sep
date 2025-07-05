#include "drug_discovery_demo.hpp"
#include <config.hpp>
#include <glm/geometric.hpp>
#include <cstdlib>

namespace sep {
namespace workbench {

void DrugDiscoveryDemo::init() {
#ifdef SEP_WORKBENCH_DEMO
    iterations_ = 100;
    mutation_rate_ = 0.05f;
#else
    const auto& cfg = getConfigManager().getEngineConfig().drug_discovery();
    iterations_ = cfg.optimizer.iterations;
    mutation_rate_ = cfg.optimizer.mutation_rate;
#endif

    // Initialize some candidate poses
    for (int i = 0; i < 10; ++i) {
        Pose p;
        p.position = glm::vec3(static_cast<float>(std::rand()) / RAND_MAX,
                               static_cast<float>(std::rand()) / RAND_MAX,
                               static_cast<float>(std::rand()) / RAND_MAX);
        p.orientation = glm::vec3(0.0f);
        poses_.push_back(p);
    }
}

void DrugDiscoveryDemo::runOptimizationStep() {
    for (auto& p : poses_) {
        glm::vec3 delta(
            ((std::rand() % 100) / 100.0f - 0.5f) * mutation_rate_,
            ((std::rand() % 100) / 100.0f - 0.5f) * mutation_rate_,
            ((std::rand() % 100) / 100.0f - 0.5f) * mutation_rate_);
        p.position += delta;
        p.position = glm::clamp(p.position, glm::vec3(0.0f), glm::vec3(1.0f));

        // Mock binding affinity: higher when closer to origin
        p.binding_affinity = 1.0f / (1.0f + glm::length(p.position));
    }
}

void DrugDiscoveryDemo::update(float) {
    for (int i = 0; i < iterations_; ++i) {
        runOptimizationStep();
    }
}

void DrugDiscoveryDemo::render() {
    if (!renderer_) return;
    std::vector<glm::vec3> points;
    for (const auto& p : poses_) {
        points.push_back(p.position);
    }
    renderer_->renderPatternState(points);
}

void DrugDiscoveryDemo::cleanup() {
    poses_.clear();
}

void DrugDiscoveryDemo::handleKeyboard(unsigned char) {}
void DrugDiscoveryDemo::handleMouse(int, int, int) {}

} // namespace workbench
} // namespace sep
