#include <gtest/gtest.h>
#include "memory/memory_tier_manager.h"

TEST(MemoryHeaders, Compile)
{
    sep::memory::MemoryTierManager mgr;
    (void)mgr;
    SUCCEED();
}
