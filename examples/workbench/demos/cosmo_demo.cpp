#include "cosmo_demo.hpp"
#include <cstdlib>
#include <glm/gtx/norm.hpp>
#include <cmath>

namespace sep {
namespace workbench {

void CosmoDemo::init() {
    const auto& cfg = getConfigManager().getEngineConfig().cosmo();
    box_size_ = cfg.box_size;
    timestep_ = cfg.timestep;

    const size_t count = 64;
    parcels_.resize(count);
    for (auto& p : parcels_) {
        float sx = static_cast<float>(std::rand()) / RAND_MAX - 0.5f;
        float sy = static_cast<float>(std::rand()) / RAND_MAX - 0.5f;
        float sz = static_cast<float>(std::rand()) / RAND_MAX - 0.5f;
        p.position = glm::vec3(sx, sy, sz) * box_size_;
        p.velocity = glm::vec3(0.0f);
        p.mass = 1.0f;
    }
}

void CosmoDemo::applyGravity() {
    const float G = 1.0f;
    for (size_t i = 0; i < parcels_.size(); ++i) {
        for (size_t j = i + 1; j < parcels_.size(); ++j) {
            glm::vec3 dir = parcels_[j].position - parcels_[i].position;
            float dist2 = glm::length2(dir) + 1e-6f;
            glm::vec3 force = G * parcels_[i].mass * parcels_[j].mass * dir / (dist2 * std::sqrt(dist2));
            parcels_[i].velocity += force / parcels_[i].mass * timestep_;
            parcels_[j].velocity -= force / parcels_[j].mass * timestep_;
        }
    }
}

void CosmoDemo::enforceBounds() {
    float half = box_size_ * 0.5f;
    for (auto& p : parcels_) {
        for (int k = 0; k < 3; ++k) {
            if (p.position[k] > half) {
                p.position[k] = half;
                p.velocity[k] *= -1.0f;
            } else if (p.position[k] < -half) {
                p.position[k] = -half;
                p.velocity[k] *= -1.0f;
            }
        }
    }
}

void CosmoDemo::update(float) {
    applyGravity();
    for (auto& p : parcels_) {
        p.position += p.velocity * timestep_;
    }
    enforceBounds();
}

void CosmoDemo::render() {
    if (!renderer_) return;

    std::vector<glm::vec3> points;
    points.reserve(parcels_.size());
    for (const auto& p : parcels_) {
        points.push_back(p.position);
    }
    renderer_->setColorMode(show_potential_ ? "potential" : "density");
    renderer_->renderPatternState(points);
}

void CosmoDemo::cleanup() {
    parcels_.clear();
}

void CosmoDemo::handleKeyboard(unsigned char key) {
    if (key == 'v') {
        show_potential_ = !show_potential_;
    }
}

void CosmoDemo::handleMouse(int, int, int) {}

} // namespace workbench
} // namespace sep
