#include "quantum/evolution.h"
#include <gtest/gtest.h>

TEST(NeuroSim, SpikingPropagation) {
    sep::dag::DagGraph g;
    std::vector<sep::pattern::PatternData> neurons(2);
    uint64_t a = g.addNode(glm::vec3(0.0f), 0.0f, {});
    uint64_t b = g.addNode(glm::vec3(0.0f), 0.0f, {a});
    neurons[0].coherence = 1.0f; // already firing
    neurons[1].coherence = 0.0f;

    sep::quantum::evolution::applySpikingLearning(neurons, g, 1.0f, 0.1f);
    EXPECT_GT(neurons[1].coherence, 0.0f);
}
