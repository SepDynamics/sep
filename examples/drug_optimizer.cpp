#include <iostream>
#include <vector>
#include <random>
#include <glm/glm.hpp>

#include "quantum/quantum_manifold_optimizer.h"
#include "quantum/data.hpp"

using namespace sep;
using namespace sep::quantum;
using namespace sep::quantum::manifold;

struct LigandPose {
    pattern::PatternData pattern;
    float affinity{0.0f};
};

static float scorePose(const LigandPose& pose) {
    glm::vec3 pos(pose.pattern.position);
    return 1.0f / (1.0f + glm::length(pos));
}

int main() {
    std::mt19937 rng(42);
    std::uniform_real_distribution<float> dist(-1.0f, 1.0f);

    std::vector<LigandPose> poses(5);
    for (auto& p : poses) {
        p.pattern.position = {dist(rng), dist(rng), dist(rng), 1.0f};
        p.pattern.quantum_state.coherence = 0.5f;
        p.affinity = scorePose(p);
    }

    QuantumManifoldOptimizer optimizer;

    for (int iter = 0; iter < 10; ++iter) {
        std::vector<Pattern> patterns;
        patterns.reserve(poses.size());
        for (const auto& p : poses) {
            Pattern q;
            q.position = p.pattern.position;
            q.quantum_state.coherence = p.affinity;
            patterns.push_back(q);
        }

        auto optimized = optimizer.optimize(patterns);

        for (size_t i = 0; i < poses.size(); ++i) {
            poses[i].pattern.position = optimized[i].position;
            poses[i].pattern.quantum_state = optimized[i].quantum_state;
            poses[i].affinity = scorePose(poses[i]);
        }
    }

    const LigandPose* best = &poses.front();
    for (const auto& p : poses) {
        if (p.affinity > best->affinity) best = &p;
    }

    std::cout << "Best affinity: " << best->affinity
              << " Coherence: " << best->pattern.quantum_state.coherence
              << std::endl;

    return 0;
}

