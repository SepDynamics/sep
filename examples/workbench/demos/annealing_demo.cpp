#include "annealing_demo.hpp"
#include <config.hpp>
#include <random>
#include <glm/glm.hpp>

namespace sep {
namespace workbench {

void AnnealingDemo::init() {
#ifdef SEP_WORKBENCH_DEMO
    int count = 50;
    temperature_ = 1.0f;
    cooling_rate_ = 0.99f;
#else
    const auto& cfg = sep::core::config::ConfigManager::getInstance().getEngineConfig().annealing();
    int count = cfg.particle_count;
    temperature_ = cfg.initial_temperature;
    cooling_rate_ = cfg.cooling_rate;
#endif

    particles_.resize(count);
    std::mt19937 rng{std::random_device{}()};
    std::uniform_real_distribution<float> dist(-1.0f, 1.0f);
    for (auto& p : particles_) {
        p.position = {dist(rng), dist(rng), dist(rng)};
        p.velocity = {0.0f, 0.0f, 0.0f};
    }
}

void AnnealingDemo::applyForces(float dt) {
    const float epsilon = 1.0f;
    const float sigma = 1.0f;
    for (size_t i = 0; i < particles_.size(); ++i) {
        for (size_t j = i + 1; j < particles_.size(); ++j) {
            glm::vec3 diff = particles_[j].position - particles_[i].position;
            float r2 = glm::dot(diff, diff) + 1e-6f;
            float inv_r6 = 1.0f / (r2 * r2 * r2);
            float f = 24.0f * epsilon * inv_r6 * (2.0f * inv_r6 - 1.0f);
            glm::vec3 force = f * diff;
            particles_[i].velocity += force * dt;
            particles_[j].velocity -= force * dt;
        }
    }

    for (auto& p : particles_) {
        p.position += p.velocity * dt;
        p.velocity *= 0.99f;
    }
}

void AnnealingDemo::coolSystem() {
    temperature_ *= cooling_rate_;
}

void AnnealingDemo::update(float dt) {
    applyForces(dt);
    coolSystem();
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

void AnnealingDemo::handleKeyboard(unsigned char) {}
void AnnealingDemo::handleMouse(int, int, int) {}

} // namespace workbench
} // namespace sep
