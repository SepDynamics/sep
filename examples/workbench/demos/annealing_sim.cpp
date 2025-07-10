#include "annealing_sim.hpp"
#include "sep_engine_wrapper.h"
#include <glm/gtc/random.hpp>
#include <cmath>

namespace sep {
namespace workbench {

void AnnealingSimDemo::on_load() {
    // Hardcoded config values as a temporary workaround
    temperature_schedule_ = {1.0f, 0.9f, 0.8f, 0.7f, 0.6f, 0.5f, 0.4f, 0.3f, 0.2f, 0.1f};
    if (temperature_schedule_.empty()) temperature_schedule_.push_back(1.0f);
    particles_.resize(100); // Default particle count
    for (auto& p : particles_) {
        p.position = glm::linearRand(glm::vec3(-1.f), glm::vec3(1.f));
        p.velocity = glm::vec3(0.f);
        p.color = glm::vec3(1.f);
    }
    processor_ = std::make_unique<sep::pattern::PatternProcessor>();
    coherence_mgr_ = std::make_unique<sep::memory::QuantumCoherenceManager>(sep::memory::QuantumCoherenceManager::Config{});
}

void AnnealingSimDemo::on_update(float dt) {
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
        sep::pattern::PatternData pattern;
        pattern.data = {p.position.x, p.position.y, p.position.z};
        processor_->addPattern(pattern);
    }
    processor_->evolvePatterns();
    const auto& patterns = processor_->getPatterns();
    std::vector<sep::quantum::Pattern> quantum_patterns;
    for (const auto& p : patterns) {
        sep::quantum::Pattern qp;
        qp.data = p.data;
        quantum_patterns.push_back(qp);
    }
    auto result = coherence_mgr_->updateCoherence(quantum_patterns);
    float coherence = result.global_coherence;
    glm::vec3 bright(1.f, 1.f, 0.2f);
    glm::vec3 dark(0.2f, 0.2f, 0.5f);
    glm::vec3 col = glm::mix(dark, bright, coherence);
    for (auto& p : particles_) p.color = col;
    if (current_step_ + 1 < temperature_schedule_.size()) ++current_step_;
}

void AnnealingSimDemo::on_render() {
    if (!renderer_) return;
    std::vector<glm::vec3> points;
    for (const auto& p : particles_) points.push_back(p.position);
    renderer_->renderPatternState(points);
}

void AnnealingSimDemo::on_unload() {
    particles_.clear();
    processor_.reset();
    coherence_mgr_.reset();
}

void AnnealingSimDemo::on_key_press(int key)
{
    if (key == 'p') paused_ = !paused_;
}

void AnnealingSimDemo::on_mouse(int, int, int) {}

} // namespace workbench
} // namespace sep
