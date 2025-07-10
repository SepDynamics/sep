#include "cosmo_sim.hpp"
#include <config.hpp>
#include <glm/gtx/norm.hpp>
#include <cstdlib>

namespace sep {
namespace workbench {

void CosmoSim::on_load() {
#ifdef SEP_WORKBENCH_DEMO
    box_size_ = 50.0f;
    time_step_ = 0.01f;
#else
    const auto& cfg = getConfigManager().getEngineConfig().cosmo();
    box_size_ = cfg.box_size;
    time_step_ = cfg.time_step;
#endif
    initBodies();
}

void CosmoSim::initBodies() {
    bodies_.clear();
    std::size_t count = 100;
    bodies_.reserve(count);
    for (std::size_t i = 0; i < count; ++i) {
        sep::pattern::PatternData d{};
        d.position = glm::vec4(
            static_cast<float>(std::rand()) / RAND_MAX * box_size_,
            static_cast<float>(std::rand()) / RAND_MAX * box_size_,
            static_cast<float>(std::rand()) / RAND_MAX * box_size_,
            1.0f);
        d.velocity = glm::vec4(0.0f);
        d.attributes.x = 1.0f; // mass
        bodies_.push_back(d);
    }
}

void CosmoSim::integrate(float dt) {
    const float eps = 1e-5f;
    std::vector<glm::vec3> acc(bodies_.size(), glm::vec3(0.0f));
    for (std::size_t i = 0; i < bodies_.size(); ++i) {
        for (std::size_t j = i + 1; j < bodies_.size(); ++j) {
            glm::vec3 diff = glm::vec3(bodies_[j].position) - glm::vec3(bodies_[i].position);
            float dist2 = glm::dot(diff, diff) + eps;
            float invDist3 = 1.0f / (glm::sqrt(dist2) * dist2);
            float m1 = bodies_[i].attributes.x;
            float m2 = bodies_[j].attributes.x;
            glm::vec3 force = G_ * m1 * m2 * invDist3 * diff;
            acc[i] += force / m1;
            acc[j] -= force / m2;
        }
    }
    for (std::size_t i = 0; i < bodies_.size(); ++i) {
        glm::vec3 vel = glm::vec3(bodies_[i].velocity) + acc[i] * dt;
        glm::vec3 pos = glm::vec3(bodies_[i].position) + vel * dt;
        bodies_[i].velocity = glm::vec4(vel, 0.0f);
        bodies_[i].position = glm::vec4(pos, 1.0f);
    }
}

void CosmoSim::updateCoherence() {
    const float eps = 1e-5f;
    for (std::size_t i = 0; i < bodies_.size(); ++i) {
        float m = bodies_[i].attributes.x;
        glm::vec3 vel = glm::vec3(bodies_[i].velocity);
        float kinetic = 0.5f * m * glm::length2(vel);
        float potential = 0.0f;
        for (std::size_t j = 0; j < bodies_.size(); ++j) {
            if (i == j) continue;
            glm::vec3 diff = glm::vec3(bodies_[j].position) - glm::vec3(bodies_[i].position);
            float dist = glm::length(diff) + eps;
            float mj = bodies_[j].attributes.x;
            potential -= G_ * m * mj / dist;
        }
        float total = kinetic + std::abs(potential);
        bodies_[i].coherence = total > 0.0f ? std::abs(potential) / total : 0.0f;
    }
}

void CosmoSim::on_update(float) {
    integrate(time_step_);
    updateCoherence();
}

void CosmoSim::on_render() {
    if (!renderer_) return;
    renderer_->setColorMode("temperature");
    renderer_->setEmissionMode("density");
    std::vector<glm::vec3> points;
    points.reserve(bodies_.size());
    for (const auto& b : bodies_) {
        points.emplace_back(b.position.x / box_size_, b.position.y / box_size_, b.position.z / box_size_);
    }
    renderer_->renderPatternState(points);
}

void CosmoSim::on_unload() {
    bodies_.clear();
}

void CosmoSim::on_key_press(int key)
{
    if (key == 'r') {
        initBodies();
    }
}

void CosmoSim::on_mouse(int, int, int) {}

} // namespace workbench
} // namespace sep
