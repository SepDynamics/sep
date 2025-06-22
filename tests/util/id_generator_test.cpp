#include <gtest/gtest.h>
#include "utils/id_generator.h"

TEST(IdGenerator, GeneratesUniqueIds) {
    std::string a = sep::utils::generate_unique_id();
    std::string b = sep::utils::generate_unique_id();
    EXPECT_NE(a, b);
    EXPECT_FALSE(a.empty());
    EXPECT_FALSE(b.empty());
}
