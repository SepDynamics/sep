#include "cosmo_sim.hpp"
#include <cstdlib>
#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>

namespace sep {
namespace workbench {

void CosmoSim::init() {
#ifdef SEP_WORKBENCH_DEMO
    box_size_ = 50.0f;
    time_step_ = 0.01f;
    particle_count_ = 100;
#else
    // When integrated with full engine config, load cosmo_sim settings
    const auto& cfg = getConfigManager().getEngineConfig().cosmo();
    box_size_ = cfg.box_size;
    time_step_ = cfg.time_step;
#endif
    initBodies();
}

void CosmoSim::initBodies() {
    bodies_.clear();
    bodies_.resize(particle_count_);
    for (auto& b : bodies_) {
        b.position = glm::vec4(static_cast<float>(std::rand()) / RAND_MAX * box_size_,
                               static_cast<float>(std::rand()) / RAND_MAX * box_size_,
                               static_cast<float>(std::rand()) / RAND_MAX * box_size_,
                               1.0f);
        b.velocity = glm::vec4(0.0f);
        b.attributes.x = 1.0f; // mass
        b.coherence = 1.0f;
    }
}

void CosmoSim::integrate(float dt) {
    const float G = 1.0f;
    std::vector<glm::vec3> acc(bodies_.size(), glm::vec3(0.0f));

    for (std::size_t i = 0; i < bodies_.size(); ++i) {
        for (std::size_t j = i + 1; j < bodies_.size(); ++j) {
            glm::vec3 diff = glm::vec3(bodies_[j].position) - glm::vec3(bodies_[i].position);
            float dist2 = glm::dot(diff, diff) + 1e-5f;
            glm::vec3 force = G * bodies_[i].attributes.x * bodies_[j].attributes.x /
                              (dist2 * glm::sqrt(dist2)) * diff;
            acc[i] += force / bodies_[i].attributes.x;
            acc[j] -= force / bodies_[j].attributes.x;
        }
    }

    for (std::size_t i = 0; i < bodies_.size(); ++i) {
        glm::vec3 vel = glm::vec3(bodies_[i].velocity) + acc[i] * dt;
        glm::vec3 pos = glm::vec3(bodies_[i].position) + vel * dt;
        for (int k = 0; k < 3; ++k) {
            if (pos[k] < 0.0f)
                pos[k] += box_size_;
            else if (pos[k] > box_size_)
                pos[k] -= box_size_;
        }
        bodies_[i].velocity = glm::vec4(vel, 0.0f);
        bodies_[i].position = glm::vec4(pos, 1.0f);
    }

    // Compute center of mass
    glm::vec3 com(0.0f);
    float total_mass = 0.0f;
    for (const auto& b : bodies_) {
        com += glm::vec3(b.position) * b.attributes.x;
        total_mass += b.attributes.x;
    }
    if (total_mass > 0.0f) com /= total_mass;

    // Update coherence and temperature (speed)
    for (auto& b : bodies_) {
        glm::vec3 pos = glm::vec3(b.position);
        glm::vec3 vel = glm::vec3(b.velocity);
        float radius = glm::length(pos - com) + 1e-5f;
        float vesc2 = 2.f * G * total_mass / radius;
        float speed2 = glm::length2(vel);
        b.coherence = speed2 < vesc2 ? 1.0f : 0.0f;
        b.attributes.y = glm::sqrt(speed2); // temperature proxy
    }
}

void CosmoSim::update(float) {
    integrate(time_step_);
}

void CosmoSim::render() {
    if (!renderer_) return;
    renderer_->setColorMode("density");
    renderer_->setEmissionMode("temperature");
    std::vector<glm::vec3> points;
    points.reserve(bodies_.size());
    for (const auto& b : bodies_) {
        points.emplace_back(glm::vec3(b.position) / box_size_);
    }
    renderer_->renderPatternState(points);
}

void CosmoSim::cleanup() {
    bodies_.clear();
}

void CosmoSim::handleKeyboard(unsigned char key) {
    if (key == 'r') initBodies();
}

void CosmoSim::handleMouse(int, int, int) {}

} // namespace workbench
} // namespace sep
