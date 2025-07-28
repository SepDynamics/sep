#include <gtest/gtest.h>
#include "quantum/quantum_processor.h"
#include <glm/glm.hpp>

using namespace sep::quantum;

TEST(QuantumProcessor, CoherenceIdenticalVectors) {
    QuantumProcessor::Config cfg{};
    auto processor = createQuantumProcessor(cfg);
    glm::vec3 a{1.0f, 0.0f, 0.0f};
    glm::vec3 b{1.0f, 0.0f, 0.0f};
    float coh = processor->calculateCoherence(a, b);
    EXPECT_NEAR(coh, 1.0f, 1e-5f);
}

TEST(QuantumProcessor, StabilityThreshold) {
    QuantumProcessor::Config cfg{};
    cfg.measurement_threshold = 0.5f;
    auto processor = createQuantumProcessor(cfg);
    EXPECT_TRUE(processor->isStable(0.6f));
    EXPECT_FALSE(processor->isStable(0.4f));
}
