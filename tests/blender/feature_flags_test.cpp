#include <gtest/gtest.h>

#ifndef WITH_NANOVDB
#error "WITH_NANOVDB not defined"
#endif

#ifndef WITH_OPENIMAGEDENOISE
#error "WITH_OPENIMAGEDENOISE not defined"
#endif

static_assert(WITH_NANOVDB == 0 || WITH_NANOVDB == 1,
              "WITH_NANOVDB must be 0 or 1");
static_assert(WITH_OPENIMAGEDENOISE == 0 || WITH_OPENIMAGEDENOISE == 1,
              "WITH_OPENIMAGEDENOISE must be 0 or 1");

TEST(FeatureFlagsTest, NanoVDBValue) {
#if WITH_NANOVDB == 1
    EXPECT_EQ(1, WITH_NANOVDB);
#else
    EXPECT_EQ(0, WITH_NANOVDB);
#endif
}

TEST(FeatureFlagsTest, OpenImageDenoiseValue) {
#if WITH_OPENIMAGEDENOISE == 1
    EXPECT_EQ(1, WITH_OPENIMAGEDENOISE);
#else
    EXPECT_EQ(0, WITH_OPENIMAGEDENOISE);
#endif
}
