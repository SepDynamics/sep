#include "annealing_demo.hpp"
#include <cmath>

namespace sep {
namespace workbench {

void AnnealingDemo::init() {
#ifdef SEP_WORKBENCH_DEMO
  // Use simplified parameters in demo mode
  const int particle_count = 16;
  temperature_ = 1.0f;
  cooling_rate_ = 0.05f;
#else
  const auto &cfg = getConfigManager().getEngineConfig().annealing();
  int particle_count = cfg.particle_count;
  temperature_ = cfg.initial_temperature;
  cooling_rate_ = cfg.cooling_rate;
#endif
  particles_.resize(particle_count);
  for (auto &p : particles_) {
    p.position = glm::vec3(static_cast<float>(rand()) / RAND_MAX,
                           static_cast<float>(rand()) / RAND_MAX,
                           static_cast<float>(rand()) / RAND_MAX);
    p.velocity = glm::vec3(0.0f);
  }
}

void AnnealingDemo::applyForces(float dt) {
  const float sigma = 1.0f;
  const float epsilon = 1.0f;
  for (size_t i = 0; i < particles_.size(); ++i) {
    for (size_t j = i + 1; j < particles_.size(); ++j) {
      glm::vec3 r = particles_[j].position - particles_[i].position;
      float dist = glm::length(r);
      if (dist < 1e-5f)
        continue;
      float inv_r = 1.0f / dist;
      float inv_r6 = std::pow(inv_r * sigma, 6);
      float inv_r12 = inv_r6 * inv_r6;
      float force_mag = 24.0f * epsilon * (2.0f * inv_r12 - inv_r6) * inv_r;
      glm::vec3 force = force_mag * (r / dist);
      particles_[i].velocity -= force * dt;
      particles_[j].velocity += force * dt;
    }
  }
}

void AnnealingDemo::update(float dt) {
  applyForces(dt);
  // Integrate velocities and positions
  for (auto &p : particles_) {
    p.position += p.velocity * dt;
  }
  // Cooling schedule
  temperature_ *= (1.0f - cooling_rate_ * dt);
  for (auto &p : particles_) {
    p.velocity *= temperature_;
  }
}

void AnnealingDemo::render() {
#ifdef SEP_WORKBENCH_DEMO
  if (renderer_) {
    std::vector<glm::vec3> points;
    for (const auto &p : particles_)
      points.push_back(p.position);
    renderer_->renderPatternState(points);
  }
#else
  if (!renderer_)
    return;
  std::vector<glm::vec3> points;
  for (const auto &p : particles_)
    points.push_back(p.position);
  renderer_->renderPatternState(points);
#endif
}

void AnnealingDemo::cleanup() { particles_.clear(); }

void AnnealingDemo::handleKeyboard(unsigned char key) {
  if (key == 'r') {
    init();
  }
}

void AnnealingDemo::handleMouse(int, int, int) {}

} // namespace workbench
} // namespace sep
