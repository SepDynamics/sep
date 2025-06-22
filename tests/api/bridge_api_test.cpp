#include <gtest/gtest.h>
#include "api/bridge.h"
#include "api/bridge.hpp"

TEST(BridgeCAPI, InitProcessCleanup) {
    ASSERT_EQ(sep_bridge_init(), 0);
    const char* ctx = "{\"type\":\"test\",\"content\":{}}";
    char buffer[256];
    ASSERT_EQ(sep_process_context(ctx, "layer", buffer, sizeof(buffer)), 0);
    ASSERT_EQ(sep_bridge_cleanup(), 0);
}
