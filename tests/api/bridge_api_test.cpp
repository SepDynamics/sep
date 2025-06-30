#include <gtest/gtest.h>
#include "api/bridge.h"
#include "api/bridge.hpp"

TEST(BridgeCAPI, InitProcessCleanup) {
    ASSERT_EQ(sep_bridge_init(), sep::SEPResult::SUCCESS);
    const char* ctx = "{\"type\":\"test\",\"content\":{}}";
    char buffer[256];
    ASSERT_EQ(sep_process_context(ctx, "layer", buffer, sizeof(buffer)), sep::SEPResult::SUCCESS);
    ASSERT_EQ(sep_bridge_cleanup(), sep::SEPResult::SUCCESS);
}
