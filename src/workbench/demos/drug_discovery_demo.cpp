#include "drug_discovery_demo.hpp"
#include <config.hpp>
#include <glm/glm.hpp>
#include <cstdlib>

namespace sep {
namespace workbench {

void DrugDiscoveryDemo::on_load() {
#ifdef SEP_WORKBENCH_DEMO
    // Default parameters for demo mode
    iterations_ = 10;
    mutation_rate_ = 0.05f;
#else
    const auto& cfg = ConfigManager::getInstance().getEngineConfig().drug_discovery();
    iterations_ = cfg.iterations;
    mutation_rate_ = cfg.mutation_rate;
#endif

    // Create a few initial poses
    for (int i = 0; i < 5; ++i) {
        Pose p;
        p.position = glm::vec3((std::rand() % 100) * 0.01f);
        p.orientation = glm::vec3(0.0f);
        p.binding_affinity = 0.0f;
        poses_.push_back(p);
    }
}

void DrugDiscoveryDemo::optimizePoses() {
    for (int it = 0; it < iterations_; ++it) {
        for (auto& p : poses_) {
            // Simple random mutation of pose variables
            p.position += mutation_rate_ * glm::vec3(
                ((std::rand() % 200) - 100) * 0.001f,
                ((std::rand() % 200) - 100) * 0.001f,
                ((std::rand() % 200) - 100) * 0.001f);
            p.orientation += mutation_rate_ * glm::vec3(
                ((std::rand() % 200) - 100) * 0.001f,
                ((std::rand() % 200) - 100) * 0.001f,
                ((std::rand() % 200) - 100) * 0.001f);

            // Mock binding affinity computation
            float dist = glm::length(p.position);
            p.binding_affinity = glm::clamp(1.0f - dist, 0.0f, 1.0f);
        }
    }
}

void DrugDiscoveryDemo::on_update(float) {
    optimizePoses();
}

void DrugDiscoveryDemo::on_render() {
#ifdef SEP_WORKBENCH_DEMO
    if (renderer_) {
        std::vector<glm::vec3> points;
        for (const auto& p : poses_) {
            points.push_back(p.position);
        }
        renderer_->renderPatternState(points);
    }
#else
    if (!renderer_) return;
    std::vector<glm::vec3> points;
    for (const auto& p : poses_) {
        points.push_back(p.position);
    }
    renderer_->renderPatternState(points);
#endif
}

void DrugDiscoveryDemo::on_unload() {
    poses_.clear();
}

void DrugDiscoveryDemo::on_key_press(int key) { (void)key; }
void DrugDiscoveryDemo::on_mouse(int, int, int) {}

} // namespace workbench
} // namespace sep

