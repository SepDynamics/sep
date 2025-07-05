#include <iostream>
#include <vector>
#include <random>
#include <glm/vec3.hpp>
#include <glm/gtx/norm.hpp>
#include "quantum/data.hpp"
#include "quantum/quantum_manifold_optimizer.h"

using sep::pattern::PatternData;
using sep::quantum::Pattern;
using sep::quantum::manifold::QuantumManifoldOptimizer;

struct LigandPose {
    PatternData data;
};

static float computeBindingAffinity(const LigandPose& pose) {
    glm::vec3 pos = glm::vec3(pose.data.position);
    return 1.0f / (1.0f + glm::length2(pos));
}

int main() {
    const int numPoses = 5;
    std::mt19937 rng(42);
    std::uniform_real_distribution<float> dist(-1.0f, 1.0f);

    std::vector<LigandPose> poses(numPoses);
    for (auto& p : poses) {
        p.data.position = {dist(rng), dist(rng), dist(rng), 1.0f};
        p.data.coherence = computeBindingAffinity(p);
    }

    QuantumManifoldOptimizer optimizer;
    std::vector<Pattern> patterns;
    patterns.reserve(numPoses);
    for (const auto& p : poses) {
        Pattern pat;
        pat.position = p.data.position;
        pat.quantum_state.coherence = p.data.coherence;
        patterns.push_back(pat);
    }

    auto optimized = optimizer.optimize(patterns);

    float bestAffinity = 0.0f;
    float bestCoherence = 0.0f;
    for (size_t i = 0; i < poses.size(); ++i) {
        poses[i].data.position = optimized[i].position;
        poses[i].data.coherence = optimized[i].quantum_state.coherence;
        float aff = computeBindingAffinity(poses[i]);
        if (aff > bestAffinity) {
            bestAffinity = aff;
            bestCoherence = poses[i].data.coherence;
        }
    }

    std::cout << "Best binding affinity: " << bestAffinity
              << " with coherence: " << bestCoherence << std::endl;
    return 0;
}
