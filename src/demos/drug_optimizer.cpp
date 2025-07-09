#include "drug_optimizer.hpp"
#include <glm/gtx/norm.hpp>
#include <cstdlib>

namespace sep {
namespace workbench {

void DrugOptimizerDemo::init() {
    poses_.resize(5);
    for (auto& p : poses_) {
        p.position = glm::vec3(static_cast<float>(std::rand()) / RAND_MAX,
                               static_cast<float>(std::rand()) / RAND_MAX,
                               static_cast<float>(std::rand()) / RAND_MAX);
        p.orientation = glm::vec3(0.0f);
        p.binding_affinity = computeBindingScore(p);
    }
}

float DrugOptimizerDemo::computeBindingScore(const MoleculePose& pose) {
#ifdef SEP_EXT_CHEM
    // Placeholder for external chemistry library integration
    return external_chemistry_score(pose.position.x, pose.position.y, pose.position.z);
#else
    return 1.0f / (1.0f + glm::length2(pose.position));
#endif
}

void DrugOptimizerDemo::update(float) {
    std::vector<sep::quantum::Pattern> patterns;
    patterns.reserve(poses_.size());
    for (const auto& p : poses_) {
        sep::quantum::Pattern pattern;
        pattern.position = glm::vec4(p.position, 1.0f);
        pattern.quantum_state.coherence = p.binding_affinity;
        patterns.push_back(pattern);
    }

    auto optimized = optimizer_.optimize(patterns);
    for (size_t i = 0; i < poses_.size(); ++i) {
        poses_[i].position = glm::vec3(optimized[i].position);
        poses_[i].binding_affinity = optimized[i].quantum_state.coherence;
    }
}

void DrugOptimizerDemo::render() {
    if (!renderer_) return;
    std::vector<glm::vec3> points;
    for (const auto& p : poses_) points.push_back(p.position);
    renderer_->renderPatternState(points);
}

void DrugOptimizerDemo::cleanup() {
    poses_.clear();
}

void DrugOptimizerDemo::handleKeyboard(unsigned char) {}
void DrugOptimizerDemo::handleMouse(int, int, int) {}

} // namespace workbench
} // namespace sep
