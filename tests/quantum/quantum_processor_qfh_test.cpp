#include "quantum/quantum_processor_qfh.h"
#include <gtest/gtest.h>

using namespace sep::quantum;

TEST(QuantumProcessorQFHTest, DetermineMemoryTier) {
    QuantumProcessorQFH proc;
    EXPECT_EQ(proc.determineMemoryTier(0.95f, 0.9f, 150), sep::memory::TierType::LTM);
    EXPECT_EQ(proc.determineMemoryTier(0.75f, 0.5f, 10), sep::memory::TierType::MTM);
    EXPECT_EQ(proc.determineMemoryTier(0.2f, 0.1f, 1), sep::memory::TierType::STM);
}

TEST(QuantumProcessorQFHTest, MutationRateInRange) {
    QuantumProcessorQFHCommon base;
    float rate = base.calculateMutationRate(0.1f, 2, 3);
    EXPECT_GE(rate, 0.0f);
    EXPECT_LE(rate, 1.0f);
}
