#include "cosmo_demo.hpp"
#include <config.hpp>
#include "sep_engine_wrapper.h"
#include <cmath>
#include <cstdlib>

namespace sep {
namespace workbench {

void CosmoDemo::init() {
#ifdef SEP_WORKBENCH_DEMO
    const auto& cfg = Config::getInstance().cosmo();
    box_size_ = cfg.box_size;
    timestep_ = cfg.timestep;
#else
    const auto& cfg = getConfigManager().getEngineConfig().cosmo();
    box_size_ = cfg.box_size;
    timestep_ = cfg.timestep;
#endif
    parcels_.clear();
    const int count = 32;
    for (int i = 0; i < count; ++i) {
        MassParcel m;
        float half = box_size_ * 0.5f;
        m.position = glm::vec3(
            static_cast<float>(rand()) / RAND_MAX * box_size_ - half,
            static_cast<float>(rand()) / RAND_MAX * box_size_ - half,
            static_cast<float>(rand()) / RAND_MAX * box_size_ - half);
        m.velocity = glm::vec3(0.0f);
        parcels_.push_back(m);
    }
}

void CosmoDemo::update(float dt) {
    const float G = 1.0f;
    size_t n = parcels_.size();
    std::vector<glm::vec3> acc(n, glm::vec3(0.0f));
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < n; ++j) {
            if (i == j) continue;
            glm::vec3 r = parcels_[j].position - parcels_[i].position;
            float dist2 = glm::dot(r, r) + 1e-5f;
            float invDist3 = 1.0f / (std::sqrt(dist2) * dist2);
            acc[i] += G * parcels_[j].mass * r * invDist3;
        }
    }
    float step = timestep_ * dt;
    for (size_t i = 0; i < n; ++i) {
        parcels_[i].velocity += acc[i] * step;
        parcels_[i].position += parcels_[i].velocity * step;
        float half = box_size_ * 0.5f;
        for (int k=0;k<3;++k) {
            if (parcels_[i].position[k] > half) parcels_[i].position[k] -= box_size_;
            if (parcels_[i].position[k] < -half) parcels_[i].position[k] += box_size_;
        }
    }
}

void CosmoDemo::render() {
    if (!renderer_) return;
    renderer_->setColorMode(show_density_ ? "density" : "potential");
    std::vector<glm::vec3> pts;
    pts.reserve(parcels_.size());
    for (const auto& p : parcels_) pts.push_back(p.position);
    renderer_->renderPatternState(pts);
}

void CosmoDemo::cleanup() {
    parcels_.clear();
}

void CosmoDemo::handleKeyboard(unsigned char key) {
    if (key == 'v') {
        show_density_ = !show_density_;
    }
}

void CosmoDemo::handleMouse(int, int, int) {}

} // namespace workbench
} // namespace sep
