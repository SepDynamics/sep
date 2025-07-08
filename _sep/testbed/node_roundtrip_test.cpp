#include <gtest/gtest.h>
#include <cstdlib>

TEST(NodeRoundTrip, MemoryConfig) {
    int ret = std::system("node _sep/testbed/memory_config_roundtrip.cjs _sep/testbed/memory_config.json > /dev/null");
    ASSERT_EQ(ret, 0);
}
