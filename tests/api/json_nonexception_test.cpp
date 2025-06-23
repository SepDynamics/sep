#include <gtest/gtest.h>
#ifdef CROW_DISABLE_RTTI
#include "crow/crow_isolation.h"
#endif
#include "crow/json.h"

TEST(CrowJsonNonException, BeginReturnsNullOnNonContainer) {
    auto v = crow::json::load("5");
    EXPECT_EQ(v.begin(), nullptr);
}

TEST(CrowJsonNonException, IndexOutOfRangeReturnsNullValue) {
    auto v = crow::json::load("[1,2]");
    const auto& r = v[5];
    EXPECT_EQ(r.t(), crow::json::type::Null);
}

TEST(CrowJsonNonException, MissingKeyReturnsNullValue) {
    auto v = crow::json::load("{\"a\":1}");
    const auto& r = v["b"];
    EXPECT_EQ(r.t(), crow::json::type::Null);
}
