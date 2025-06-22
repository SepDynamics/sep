#include <gtest/gtest.h>
#include "quantum/qbsa.h"

using namespace sep::quantum;

namespace sep::quantum::test {

TEST(QBSAProcessorTest, AnalyzeBasic) {
    QBSAProcessor proc;
    std::vector<uint32_t> probes = {0,1,0,1};
    std::vector<uint32_t> expectations = {0,0,0,1};

    auto result = proc.analyze(probes, expectations);
    ASSERT_EQ(result.correction_count, 1u);
    ASSERT_EQ(result.correction_indices.size(), 1u);
    EXPECT_EQ(result.correction_indices[0], 1u);
}

TEST(QBSAProcessorTest, CollapseDetection) {
    QBSAOptions opts; opts.collapse_threshold = 0.5f;
    QBSAProcessor proc(opts);
    std::vector<uint32_t> probes = {1,1,1,1};
    std::vector<uint32_t> expectations = {0,0,0,0};

    auto result = proc.analyze(probes, expectations);
    EXPECT_TRUE(proc.detectCollapse(result, probes.size()));
}

} // namespace sep::quantum::test

