#include "cosmo_demo.hpp"
#include <config.hpp>
#include <glm/gtc/random.hpp>
#include <glm/gtx/norm.hpp>
#include <cmath>

namespace sep {
namespace workbench {

void CosmoDemo::init() {
#ifdef SEP_WORKBENCH_DEMO
    box_size_ = 50.0f;
    time_step_ = 0.01f;
#else
    const auto& cfg = getConfigManager().getEngineConfig().cosmo_demo();
    box_size_ = cfg.box_size;
    time_step_ = cfg.time_step;
#endif
    initializeBodies();
}

void CosmoDemo::initializeBodies() {
    bodies_.clear();
    const int num_bodies = 10;
    for (int i = 0; i < num_bodies; ++i) {
        Body b;
        b.position = glm::linearRand(glm::vec3(-box_size_ * 0.5f), glm::vec3(box_size_ * 0.5f));
        b.velocity = glm::vec3(0.0f);
        b.mass = 1.0f;
        bodies_.push_back(b);
    }
}

void CosmoDemo::integrate(float dt) {
    std::vector<glm::vec3> forces(bodies_.size(), glm::vec3(0.0f));
    for (size_t i = 0; i < bodies_.size(); ++i) {
        for (size_t j = i + 1; j < bodies_.size(); ++j) {
            glm::vec3 diff = bodies_[j].position - bodies_[i].position;
            float dist2 = glm::length2(diff) + 1e-5f;
            float inv_dist = 1.0f / std::sqrt(dist2);
            glm::vec3 force = gravity_const_ * bodies_[i].mass * bodies_[j].mass * diff * inv_dist / dist2;
            forces[i] += force;
            forces[j] -= force;
        }
    }

    for (size_t i = 0; i < bodies_.size(); ++i) {
        bodies_[i].velocity += forces[i] / bodies_[i].mass * dt;
        bodies_[i].position += bodies_[i].velocity * dt;
        // Wrap around box boundaries
        for (int c = 0; c < 3; ++c) {
            if (bodies_[i].position[c] > box_size_ * 0.5f) bodies_[i].position[c] -= box_size_;
            if (bodies_[i].position[c] < -box_size_ * 0.5f) bodies_[i].position[c] += box_size_;
        }
    }
}

void CosmoDemo::update(float dt) {
    (void)dt; // dt provided by loop; we use configured timestep
    integrate(time_step_);
}

void CosmoDemo::render() {
    if (!renderer_) return;
    renderer_->setColorMode("density");
    renderer_->setEmissionMode("potential");
    std::vector<glm::vec3> points;
    for (const auto& b : bodies_) {
        points.push_back(b.position);
    }
    renderer_->renderPatternState(points);
}

void CosmoDemo::cleanup() {
    bodies_.clear();
}

void CosmoDemo::handleKeyboard(unsigned char key) {
    if (key == 'r') {
        initializeBodies();
    }
}

void CosmoDemo::handleMouse(int, int, int) {}

} // namespace workbench
} // namespace sep
