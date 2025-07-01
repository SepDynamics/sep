#include "quantum/quantum_processor.h"
#include <gtest/gtest.h>
#include <glm/glm.hpp>

using namespace sep::quantum;

TEST(QuantumProcessorMetrics, CoherenceOfIdenticalPatterns) {
    QuantumProcessor proc({});
    glm::vec3 a(1.0f, 0.0f, 0.0f);
    EXPECT_FLOAT_EQ(proc.calculateCoherence(a, a), 1.0f);
}

TEST(QuantumProcessorMetrics, CoherenceOfOrthogonalPatterns) {
    QuantumProcessor proc({});
    glm::vec3 a(1.0f, 0.0f, 0.0f);
    glm::vec3 b(0.0f, 1.0f, 0.0f);
    EXPECT_FLOAT_EQ(proc.calculateCoherence(a, b), 0.0f);
}

TEST(QuantumProcessorMetrics, StabilityCalculation) {
    QuantumProcessor proc({});
    float stability = proc.calculateStability(0.5f, 0.5f, 10.0f, 0.5f);
    EXPECT_GE(stability, 0.0f);
    EXPECT_LE(stability, 1.0f);
}
