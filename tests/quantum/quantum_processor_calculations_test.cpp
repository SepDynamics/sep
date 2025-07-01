#include "quantum/quantum_processor.h"
#include <gtest/gtest.h>
#include <glm/vec3.hpp>

using namespace sep::quantum;

TEST(QuantumProcessorCalculations, CoherenceBasics) {
    QuantumProcessor::Config cfg{};
    QuantumProcessor proc(cfg);
    glm::vec3 a(1.0f, 0.0f, 0.0f);
    glm::vec3 b(1.0f, 0.0f, 0.0f);
    EXPECT_FLOAT_EQ(proc.calculateCoherence(a, b), 1.0f);
    glm::vec3 c(0.0f, 1.0f, 0.0f);
    EXPECT_FLOAT_EQ(proc.calculateCoherence(a, c), 0.0f);
}

TEST(QuantumProcessorCalculations, StabilityRange) {
    QuantumProcessor::Config cfg{};
    QuantumProcessor proc(cfg);
    float stability = proc.calculateStability(0.5f, 0.5f, 10.0f, 0.5f);
    EXPECT_GE(stability, 0.0f);
    EXPECT_LE(stability, 1.0f);
}
