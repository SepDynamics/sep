
#include <gtest/gtest.h>

using namespace sep;

TEST(ApiHelpers, MakeErrorResponse)
{
    auto obj = api::make_error_response(ErrorCode::InvalidArgument, "bad");
    EXPECT_FALSE(obj["success"].get<bool>());
    EXPECT_EQ(obj["error"]["code"].get<int>(), static_cast<int>(ErrorCode::InvalidArgument));
    EXPECT_EQ(obj["error"]["message"].get<std::string>(), "bad");
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
