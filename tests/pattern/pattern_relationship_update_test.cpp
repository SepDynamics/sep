#include <gtest/gtest.h>


using namespace sep;

TEST(PatternProcessorRelationships, UpdatesBidirectional)
{
    pattern::PatternProcessor proc(pattern::PatternProcessor::Implementation::CPU);
    ASSERT_EQ(proc.init(nullptr), sep::SEPResult::SUCCESS);

    pattern::PatternData a{};
    a.id = "a";
    a.coherence = 0.8f;
    a.stability = 0.9f;
    a.entropy = 0.2f;

    pattern::PatternData b = a;
    b.id = "b";

    proc.addPattern(a);
    proc.addPattern(b);

    proc.updateRelationships();

    const auto& patterns = proc.getPatterns();
    ASSERT_EQ(patterns.size(), 2u);
    EXPECT_FALSE(patterns[0].relationships.empty());
    EXPECT_FALSE(patterns[1].relationships.empty());
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
