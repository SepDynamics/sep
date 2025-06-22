#include <gtest/gtest.h>


using namespace sep;

TEST(ApiHelpers, ValidateFieldsSuccess) {
    nlohmann::json data = { {"a", 1}, {"b", 2} };
    nlohmann::json error;
    EXPECT_TRUE(api::validate_fields(data, {"a","b"}, error));
}

TEST(ApiHelpers, ValidateFieldsMissing) {
    nlohmann::json data = { {"a", 1} };
    nlohmann::json error;
    EXPECT_FALSE(api::validate_fields(data, {"a","b"}, error));
    EXPECT_EQ(error["error"]["code"], static_cast<int>(ErrorCode::InvalidArgument));
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
