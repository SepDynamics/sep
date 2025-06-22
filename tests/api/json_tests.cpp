#include <gtest/gtest.h>
#ifdef CROW_DISABLE_RTTI
#include "sep/crow_isolation.h"
#endif
#include "crow/json.h"

TEST(CrowJsonParsing, SimpleObject) {
    auto v = crow::json::load("{\"a\":1}");
    ASSERT_EQ(v["a"].i(), 1);
}

TEST(CrowJsonCopy, CopyConstructor) {
    auto v = crow::json::load("[1,2]");
    crow::json::rvalue cpy(v);
    EXPECT_EQ(cpy.size(), 2u);
    EXPECT_EQ(cpy[1].i(), 2);
    EXPECT_NE(&cpy[0], &v[0]);
}

TEST(CrowJsonCopy, CopyAssignment) {
    auto v = crow::json::load("[3,4]");
    crow::json::rvalue other;
    other = v;
    EXPECT_EQ(other.size(), 2u);
    EXPECT_EQ(other[0].i(), 3);
    EXPECT_NE(&other[0], &v[0]);
}

TEST(CrowJsonMove, MoveConstructor) {
    auto v = crow::json::load("[5,6,7]");
    crow::json::rvalue moved(std::move(v));
    EXPECT_EQ(moved.size(), 3u);
    // moved-from value should be null
    EXPECT_EQ(v.size(), 0u);
    EXPECT_EQ(moved[2].i(), 7);
}
