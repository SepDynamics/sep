#include <iostream>
#include <vector>
#include <cstdlib>
#include <limits>
#include <glm/gtx/norm.hpp>

#include "quantum/quantum_manifold_optimizer.h"
#include "quantum/data.hpp"

// Simple pose representation for a ligand using PatternData
struct LigandPose {
    sep::pattern::PatternData pattern;
};

// Placeholder scoring function returning higher values for lower energies
static float scorePose(const LigandPose& pose) {
    return 1.0f / (1.0f + glm::length2(glm::vec3(pose.pattern.position)));
}

int main() {
    // Create a few random poses
    std::vector<LigandPose> poses(5);
    for (auto& p : poses) {
        p.pattern.position = glm::vec4(
            static_cast<float>(std::rand()) / RAND_MAX,
            static_cast<float>(std::rand()) / RAND_MAX,
            static_cast<float>(std::rand()) / RAND_MAX,
            1.0f);
        p.pattern.coherence = scorePose(p);
    }

    sep::quantum::manifold::QuantumManifoldOptimizer optimizer;
    float best_affinity = std::numeric_limits<float>::lowest();

    // Perform a few optimization steps
    for (int step = 0; step < 10; ++step) {
        std::vector<sep::quantum::Pattern> patterns;
        patterns.reserve(poses.size());
        for (const auto& p : poses) {
            sep::quantum::Pattern q;
            q.position = p.pattern.position;
            q.quantum_state.coherence = p.pattern.coherence;
            patterns.push_back(q);
        }

        auto optimized = optimizer.optimize(patterns);
        for (std::size_t i = 0; i < poses.size(); ++i) {
            poses[i].pattern.position = optimized[i].position;
            poses[i].pattern.coherence = optimized[i].quantum_state.coherence;
            float score = scorePose(poses[i]);
            if (score > best_affinity)
                best_affinity = score;
        }
    }

    std::cout << "Best affinity/coherence: " << best_affinity << "\n";
    return 0;
}

