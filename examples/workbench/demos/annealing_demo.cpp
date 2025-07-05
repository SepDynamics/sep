#include "annealing_demo.hpp"
#include <config.hpp>
#include <glm/gtc/random.hpp>
#include <glm/glm.hpp>
#include <algorithm>
#include <cmath>

namespace sep {
namespace workbench {

void AnnealingDemo::init() {
    const auto& cfg = getConfigManager().getEngineConfig().annealing();
    temperature_ = cfg.initial_temperature;
    cooling_rate_ = cfg.cooling_rate;

    particles_.clear();
    particles_.resize(cfg.particle_count);
    for (auto& p : particles_) {
        p.data.position = glm::linearRand(glm::vec3(-1.0f), glm::vec3(1.0f));
        p.velocity = glm::vec3(0.0f);
    }
}

void AnnealingDemo::update(float dt) {
    const float epsilon = 1.0f;
    for (std::size_t i = 0; i < particles_.size(); ++i) {
        for (std::size_t j = i + 1; j < particles_.size(); ++j) {
            glm::vec3 r = particles_[i].data.position - particles_[j].data.position;
            float dist2 = glm::dot(r, r) + 1e-6f;
            float inv_r2 = 1.0f / dist2;
            float inv_r6 = inv_r2 * inv_r2 * inv_r2;
            float inv_r12 = inv_r6 * inv_r6;
            float magnitude = 24.0f * epsilon * (2.0f * inv_r12 - inv_r6) * inv_r2;
            glm::vec3 f = magnitude * r;
            particles_[i].velocity += f * dt;
            particles_[j].velocity -= f * dt;
        }
    }

    for (auto& p : particles_) {
        p.data.position += p.velocity * dt;
        p.velocity *= temperature_;
    }

    temperature_ *= cooling_rate_;
}

void AnnealingDemo::render() {
    if (!renderer_) return;
    std::vector<glm::vec3> points;
    points.reserve(particles_.size());
    for (const auto& p : particles_) {
        points.push_back(p.data.position);
    }
    renderer_->renderPatternState(points);
}

void AnnealingDemo::cleanup() {
    particles_.clear();
}

void AnnealingDemo::handleKeyboard(unsigned char key) {
    if (key == 'r') {
        init();
    }
}

void AnnealingDemo::handleMouse(int, int, int) {}

} // namespace workbench
} // namespace sep

