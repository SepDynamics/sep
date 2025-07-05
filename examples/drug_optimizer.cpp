#include "quantum/quantum_manifold_optimizer.h"
#include "quantum/data.hpp"
#include <glm/glm.hpp>
#include <iostream>
#include <vector>
#include <cstdlib>

using sep::quantum::manifold::QuantumManifoldOptimizer;
using sep::pattern::PatternData;
using sep::quantum::Pattern;

static float computeAffinity(const PatternData& pose) {
#ifdef SEP_EXT_CHEM
    return external_chemistry_score(pose.position.x, pose.position.y, pose.position.z);
#else
    glm::vec3 p = glm::vec3(pose.position);
    return 1.0f / (1.0f + glm::length(p));
#endif
}

int main() {
    QuantumManifoldOptimizer optimizer;
    std::vector<PatternData> poses(5);
    std::vector<Pattern> patterns;
    patterns.reserve(poses.size());

    for (size_t i = 0; i < poses.size(); ++i) {
        auto& pd = poses[i];
        pd.id = "pose_" + std::to_string(i);
        pd.position = glm::vec4(static_cast<float>(std::rand()) / RAND_MAX,
                                static_cast<float>(std::rand()) / RAND_MAX,
                                static_cast<float>(std::rand()) / RAND_MAX,
                                1.0f);
        pd.coherence = computeAffinity(pd);

        Pattern pat;
        pat.id = pd.id;
        pat.position = pd.position;
        pat.quantum_state.coherence = pd.coherence;
        patterns.push_back(pat);
    }

    auto optimized = optimizer.optimize(patterns);
    for (size_t i = 0; i < poses.size(); ++i) {
        poses[i].coherence = optimized[i].quantum_state.coherence;
    }

    const PatternData* best = nullptr;
    for (const auto& p : poses) {
        if (!best || p.coherence > best->coherence) best = &p;
    }

    if (best) {
        std::cout << "Best affinity/coherence: " << best->coherence
                  << " at position (" << best->position.x << ", "
                  << best->position.y << ", " << best->position.z << ")\n";
    }
    return 0;
}
