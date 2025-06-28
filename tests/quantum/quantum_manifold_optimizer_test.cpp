#include "quantum/quantum_manifold_optimizer.h"
#include <gtest/gtest.h>

using namespace sep::quantum;

TEST(QuantumManifoldOptimizerTest, OptimizeReturnsInput) {
    QuantumManifoldOptimizer optimizer;
    std::vector<Pattern> patterns(1);
    auto result = optimizer.optimize(patterns);
    EXPECT_EQ(result.size(), patterns.size());
}
