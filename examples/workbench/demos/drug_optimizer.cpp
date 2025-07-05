#include "drug_optimizer.hpp"
#include <cstdlib>
#include <glm/gtx/norm.hpp>

namespace sep {
namespace workbench {

namespace {
// Placeholder scoring stub. In a real system this would call an external
// chemistry library to evaluate binding affinity.
float scorePose(const sep::quantum::Pattern &p) {
    // Higher coherence indicates better binding affinity in this simple stub
    return p.quantum_state.coherence;
}
} // namespace

void DrugOptimizerDemo::init() {
#ifdef SEP_WORKBENCH_DEMO
    const int pose_count = 5;
#else
    const auto &cfg = getConfigManager().optimizer();
    const int pose_count = cfg.iterations;
#endif
    poses_.resize(pose_count);
    for (auto &pose : poses_) {
        pose.pattern.position = glm::vec4(static_cast<float>(std::rand()) / RAND_MAX,
                                          static_cast<float>(std::rand()) / RAND_MAX,
                                          static_cast<float>(std::rand()) / RAND_MAX,
                                          1.0f);
        pose.pattern.quantum_state.coherence = static_cast<float>(std::rand()) / RAND_MAX;
        pose.score = 0.0f;
    }
}

void DrugOptimizerDemo::update(float) {
    std::vector<sep::quantum::Pattern> patterns;
    patterns.reserve(poses_.size());
    for (const auto &p : poses_) patterns.push_back(p.pattern);
    auto optimized = optimizer_.optimize(patterns);
    for (size_t i = 0; i < poses_.size(); ++i) {
        poses_[i].pattern = optimized[i];
        poses_[i].score = scorePose(poses_[i].pattern);
    }
}

void DrugOptimizerDemo::render() {
    if (!renderer_) return;
    std::vector<glm::vec3> points;
    for (const auto &p : poses_) points.emplace_back(p.pattern.position);
    renderer_->renderPatternState(points);
}

void DrugOptimizerDemo::cleanup() {
    poses_.clear();
}

void DrugOptimizerDemo::handleKeyboard(unsigned char) {}
void DrugOptimizerDemo::handleMouse(int, int, int) {}

} // namespace workbench
} // namespace sep
