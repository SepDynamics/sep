#include "drug_discovery_demo.hpp"
#include <config.hpp>
#include <glm/gtc/constants.hpp>
#include <cstdlib>

namespace sep {
namespace workbench {

void DrugDiscoveryDemo::init() {
#ifdef SEP_WORKBENCH_DEMO
    // Create a few placeholder poses
    for (int i = 0; i < 5; ++i) {
        Pose p;
        p.position = glm::vec3(static_cast<float>(std::rand()%100) / 100.f,
                               static_cast<float>(std::rand()%100) / 100.f,
                               static_cast<float>(std::rand()%100) / 100.f);
        p.orientation = glm::vec3(0.0f);
        p.affinity = 0.0f;
        poses_.push_back(p);
    }
#else
    const auto& cfg = getConfigManager().getEngineConfig().drug_discovery();
    optimizer_.iterations = cfg.optimizer.iterations;
    optimizer_.mutation_rate = cfg.optimizer.mutation_rate;
    // Initialize poses with dummy data
    for (int i = 0; i < 5; ++i) {
        Pose p;
        p.position = glm::vec3(static_cast<float>(std::rand()%100) / 100.f,
                               static_cast<float>(std::rand()%100) / 100.f,
                               static_cast<float>(std::rand()%100) / 100.f);
        poses_.push_back(p);
    }
#endif
}

void DrugDiscoveryDemo::update(float dt) {
    // Placeholder optimization of molecular poses
    for (auto& pose : poses_) {
        for (int i = 0; i < optimizer_.iterations; ++i) {
            // Randomly perturb position and orientation
            pose.position += optimizer_.mutation_rate *
                glm::vec3((std::rand()%200 - 100) / 100.0f) * dt;
            pose.orientation += optimizer_.mutation_rate *
                glm::vec3((std::rand()%200 - 100) / 100.0f) * dt;
        }
        // Mock binding affinity based on distance from origin
        float dist = glm::length(pose.position);
        pose.affinity = 1.0f - glm::clamp(dist, 0.0f, 1.0f);
    }
}

void DrugDiscoveryDemo::render() {
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

void DrugDiscoveryDemo::cleanup() {
    poses_.clear();
}

void DrugDiscoveryDemo::handleKeyboard(unsigned char key) {
    switch (key) {
        case '+':
            optimizer_.mutation_rate += 0.05f;
            break;
        case '-':
            optimizer_.mutation_rate = std::max(0.01f, optimizer_.mutation_rate - 0.05f);
            break;
    }
}

void DrugDiscoveryDemo::handleMouse(int, int, int) {}

} // namespace workbench
} // namespace sep
