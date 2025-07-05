#include "cosmo_demo.hpp"
#include <config.hpp>
#include <cstdlib>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

namespace sep {
namespace workbench {

void CosmoDemo::init() {
#ifdef SEP_WORKBENCH_DEMO
    // Default parameters
    box_size_ = 50.0f;
    time_step_ = 0.01f;
#else
    const auto& cfg = getConfigManager().getEngineConfig().cosmo();
    box_size_ = cfg.box_size;
    time_step_ = cfg.time_step;
#endif
    initParticles();
}

void CosmoDemo::initParticles() {
    particles_.clear();
    std::size_t count = 100;
    particles_.reserve(count);
    for (std::size_t i = 0; i < count; ++i) {
        Particle p;
        p.position = glm::vec3(
            static_cast<float>(std::rand()) / RAND_MAX * box_size_,
            static_cast<float>(std::rand()) / RAND_MAX * box_size_,
            static_cast<float>(std::rand()) / RAND_MAX * box_size_);
        p.velocity = glm::vec3(0.0f);
        p.mass = 1.0f;
        particles_.push_back(p);
    }
}

void CosmoDemo::integrate(float dt) {
    const float G = 1.0f;
    std::vector<glm::vec3> accelerations(particles_.size(), glm::vec3(0.0f));
    for (std::size_t i = 0; i < particles_.size(); ++i) {
        for (std::size_t j = i + 1; j < particles_.size(); ++j) {
            glm::vec3 diff = particles_[j].position - particles_[i].position;
            float dist2 = glm::dot(diff, diff) + 1e-5f;
            glm::vec3 force = G * particles_[i].mass * particles_[j].mass / (dist2 * glm::sqrt(dist2)) * diff;
            accelerations[i] += force / particles_[i].mass;
            accelerations[j] -= force / particles_[j].mass;
        }
    }

    for (std::size_t i = 0; i < particles_.size(); ++i) {
        particles_[i].velocity += accelerations[i] * dt;
        particles_[i].position += particles_[i].velocity * dt;
        // Wrap around box
        for (int k = 0; k < 3; ++k) {
            if (particles_[i].position[k] < 0.0f)
                particles_[i].position[k] += box_size_;
            else if (particles_[i].position[k] > box_size_)
                particles_[i].position[k] -= box_size_;
        }
    }
}

void CosmoDemo::update(float dt) {
    integrate(time_step_);
}

void CosmoDemo::render() {
    if (!renderer_) return;
    std::vector<glm::vec3> points;
    points.reserve(particles_.size());
    for (const auto& p : particles_) {
        points.push_back(p.position / box_size_); // normalize to unit cube
    }
    renderer_->renderPatternState(points);
}

void CosmoDemo::cleanup() {
    particles_.clear();
}

void CosmoDemo::handleKeyboard(unsigned char key) {
    if (key == 'r') {
        initParticles();
    }
}

void CosmoDemo::handleMouse(int, int, int) {}

} // namespace workbench
} // namespace sep
