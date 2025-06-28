#include "quantum/quantum_manifold_optimizer.h"
#include <gtest/gtest.h>

using namespace sep;

TEST(QuantumManifoldOptimizerTest, MirrorsInputPatterns) {
    std::vector<pattern::PatternData> patterns(2);
    patterns[0].id = "p1";
    patterns[1].id = "p2";

    auto result = quantum::QuantumManifoldOptimizer::optimize(patterns);

    ASSERT_EQ(result.size(), patterns.size());
    EXPECT_EQ(result[0].id, "p1");
    EXPECT_EQ(result[1].id, "p2");
}

