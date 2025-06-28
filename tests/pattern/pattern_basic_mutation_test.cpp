#include <gtest/gtest.h>


using namespace sep;

TEST(PatternProcessorBasic, MutationCreatesChild)
{
    pattern::PatternProcessor proc(pattern::PatternProcessor::Implementation::CPU);
    ASSERT_EQ(proc.init(nullptr), sep::SEPResult::SUCCESS);
    pattern::PatternData parent{};
    parent.id = "parent";
    parent.coherence = 0.5f;
    parent.stability = 0.5f;
    parent.entropy = 0.5f;
    parent.mutation_rate = 0.1f;
    auto child = proc.mutatePattern(parent);
    EXPECT_NE(child.id, parent.id);
    EXPECT_EQ(child.generation, parent.generation + 1);
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
