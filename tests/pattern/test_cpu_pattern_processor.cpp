#include <gtest/gtest.h>

#include <vector>

TEST(CpuPatternProcessor, Initialize) {
    sep::pattern::PatternProcessor processor(sep::pattern::PatternProcessor::Implementation::CPU);
    EXPECT_EQ(processor.init(nullptr), sep::SEPResult::SUCCESS);
}

TEST(CpuPatternProcessor, ProcessPattern) {
    sep::pattern::PatternProcessor processor(sep::pattern::PatternProcessor::Implementation::CPU);
    ASSERT_EQ(processor.init(nullptr), sep::SEPResult::SUCCESS);
    sep::pattern::PatternData pat{};
    pat.id = "test";
    processor.addPattern(pat);
    EXPECT_EQ(processor.getPatterns().size(), 1u);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
