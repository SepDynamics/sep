#include "annealing_demo.hpp"
#include "sep_engine_wrapper.h"
#include <cmath>
#include <cstdlib>

namespace sep {
namespace workbench {

void AnnealingDemo::init() {
#ifndef SEP_WORKBENCH_DEMO
    const auto& cfg = sep::core::config::ConfigManager::getInstance().getEngineConfig();
    temperature_ = cfg.genesis_pattern().initial_pattern.evolution_rate; // placeholder
    cooling_rate_ = 0.99f;
    int count = 10;
#else
    // Default parameters for demo mode
    temperature_ = 1.0f;
    cooling_rate_ = 0.99f;
    int count = 10;
#endif
    particles_.resize(count);
    for (auto& p : particles_) {
        p.position = glm::vec3(static_cast<float>(std::rand()) / RAND_MAX,
                               static_cast<float>(std::rand()) / RAND_MAX,
                               static_cast<float>(std::rand()) / RAND_MAX);
        p.velocity = glm::vec3(0.0f);
    }
}

void AnnealingDemo::update(float dt) {
    if (paused_) return;

    const float eps = 1e-3f;
    for (size_t i = 0; i < particles_.size(); ++i) {
        for (size_t j = i + 1; j < particles_.size(); ++j) {
            glm::vec3 diff = particles_[j].position - particles_[i].position;
            float dist = glm::length(diff) + eps;
            glm::vec3 dir = diff / dist;
            float invDist6 = 1.0f / std::pow(dist, 6);
            float fmag = 24.0f * invDist6 * (2.0f * invDist6 - 1.0f) / dist;
            glm::vec3 force = dir * fmag * temperature_;
            particles_[i].velocity += force * dt;
            particles_[j].velocity -= force * dt;
        }
    }

    for (auto& p : particles_) {
        p.position += p.velocity * dt;
    }

    temperature_ *= cooling_rate_;
}

void AnnealingDemo::render() {
    if (!renderer_) return;
    std::vector<glm::vec3> points;
    for (const auto& p : particles_) {
        points.push_back(p.position);
    }
    renderer_->renderPatternState(points);
}

void AnnealingDemo::cleanup() {
    particles_.clear();
}

void AnnealingDemo::handleKeyboard(unsigned char key) {
    if (key == 'p') {
        paused_ = !paused_;
    }
}

void AnnealingDemo::handleMouse(int, int, int) {}

} // namespace workbench
} // namespace sep
