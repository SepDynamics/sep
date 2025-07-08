#include <gtest/gtest.h>
#include <nlohmann/json.hpp>
#include "core/types.h"
#include <fstream>
#include <cstdlib>

using nlohmann::json;
using namespace sep;

TEST(CrossLangAlignment, MemoryTierJSONRoundtrip) {
    struct MemoryTierRecord { MemoryTierEnum tier; std::size_t size; } rec{MemoryTierEnum::STM, 4096};

    json j;
    j["tier"] = static_cast<int>(rec.tier);
    j["size"] = rec.size;

    std::ofstream("tier_record.json") << j.dump();
    int ret = std::system("node _sep/testbed/memory_tier_roundtrip.js tier_record.json tier_record_out.json");
    ASSERT_EQ(ret, 0);
    std::ifstream f("tier_record_out.json");
    json j2; f >> j2;
    EXPECT_EQ(j, j2);
}
