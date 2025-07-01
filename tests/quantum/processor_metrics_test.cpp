#include "quantum/quantum_processor.h"
#include <gtest/gtest.h>

using namespace sep::quantum;

TEST(QuantumProcessorMetrics, CoherenceAndStability) {
    QuantumProcessor::Config cfg{};
    auto processor = std::make_unique<QuantumProcessor>(cfg);

    glm::vec3 a{1.0f, 0.0f, 0.0f};
    glm::vec3 b{1.0f, 0.0f, 0.0f};

    float coherence = processor->calculateCoherence(a, b);
    EXPECT_FLOAT_EQ(coherence, 1.0f);

    float stability = processor->calculateStability(coherence, 0.5f, 1.0f, 0.5f);
    EXPECT_GE(stability, 0.0f);
    EXPECT_LE(stability, 1.0f);
}
