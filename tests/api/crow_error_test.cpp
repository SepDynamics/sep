#include <gtest/gtest.h>
#include "crow/common.h"
#include "crow/crow_error.h"

TEST(CrowErrorTest, InvalidMethodReturnsError)
{
    sep::crow::error::Code ec = sep::crow::error::Code::None;
    auto m = method_from_string("INVALID", &ec);
    EXPECT_EQ(m, crow::HTTPMethod::Get);
    EXPECT_EQ(ec, sep::crow::error::Code::InvalidMethod);
}

TEST(CrowCommonEnum, HTTPMethodEnumValues)
{
    EXPECT_EQ(static_cast<int>(crow::HTTPMethod::Get), 1);
    EXPECT_EQ(static_cast<int>(crow::HTTPMethod::Post), 3);
    EXPECT_EQ(static_cast<int>(crow::HTTPMethod::InternalMethodCount) > 0, true);
}
