#include "annealing_sim.hpp"
#include "sep_engine_wrapper.h"
#include <glm/gtc/random.hpp>
#include <cmath>

namespace sep {
namespace workbench {

void AnnealingSimDemo::init() {
    const auto& cfg = getConfigManager().annealing_sim();
    temperature_schedule_ = cfg.temperature_schedule;
    if (temperature_schedule_.empty()) temperature_schedule_.push_back(1.0f);
    particles_.resize(static_cast<std::size_t>(cfg.particle_count));
    for (auto& p : particles_) {
        p.position = glm::linearRand(glm::vec3(-1.f), glm::vec3(1.f));
        p.velocity = glm::vec3(0.f);
        p.color = glm::vec3(1.f);
    }
    processor_ = std::make_unique<PatternProcessor>(engine_);
    coherence_mgr_ = std::make_unique<QuantumCoherenceManager>();
}

void AnnealingSimDemo::update(float dt) {
    if (paused_ || particles_.empty()) return;
    float temperature = temperature_schedule_[current_step_ % temperature_schedule_.size()];
    const float eps = 1e-3f;
    for (size_t i = 0; i < particles_.size(); ++i) {
        for (size_t j = i + 1; j < particles_.size(); ++j) {
            glm::vec3 diff = particles_[j].position - particles_[i].position;
            float dist = glm::length(diff) + eps;
            glm::vec3 dir = diff / dist;
            float invDist6 = 1.0f / std::pow(dist, 6);
            float fmag = 24.0f * invDist6 * (2.0f * invDist6 - 1.0f) / dist;
            glm::vec3 force = dir * fmag * temperature;
            particles_[i].velocity += force * dt;
            particles_[j].velocity -= force * dt;
        }
    }
    for (auto& p : particles_) {
        p.position += p.velocity * dt;
    }
    for (const auto& p : particles_) {
        std::vector<float> data = {p.position.x, p.position.y, p.position.z};
        processor_->processPattern(data);
    }
    auto result = processor_->evolvePatterns(dt);
    coherence_mgr_->updateCoherence(result);
    float coherence = coherence_mgr_->getAverageCoherence();
    glm::vec3 bright(1.f, 1.f, 0.2f);
    glm::vec3 dark(0.2f, 0.2f, 0.5f);
    glm::vec3 col = glm::mix(dark, bright, coherence);
    for (auto& p : particles_) p.color = col;
    if (current_step_ + 1 < temperature_schedule_.size()) ++current_step_;
}

void AnnealingSimDemo::render() {
    if (!renderer_) return;
    std::vector<glm::vec3> points;
    for (const auto& p : particles_) points.push_back(p.position);
    renderer_->renderPatternState(points);
}

void AnnealingSimDemo::cleanup() {
    particles_.clear();
    processor_.reset();
    coherence_mgr_.reset();
}

void AnnealingSimDemo::handleKeyboard(unsigned char key) {
    if (key == 'p') paused_ = !paused_;
}

void AnnealingSimDemo::handleMouse(int, int, int) {}

} // namespace workbench
} // namespace sep
