#include <gtest/gtest.h>
#include "memory/memory_tier_manager.hpp"
#include <fstream>
#include <nlohmann/json.hpp>

using namespace sep::memory;

TEST(CrossLanguage, MemoryTierConfigRoundTrip) {
    MemoryTierManager::Config cfg;
    cfg.stm_size = 1024;
    cfg.mtm_size = 2048;
    cfg.ltm_size = 4096;

    nlohmann::json j = cfg;
    std::string base = SEP_SOURCE_DIR;
    std::ofstream out(base + "/_sep/testbed/config_out.json");
    ASSERT_TRUE(out.is_open());
    out << j.dump();
    out.close();

    std::string cmd = "node " + base + "/_sep/testbed/cross_language_memory.cjs " + base + "/_sep/testbed/config_out.json " + base + "/_sep/testbed/config_back.json";
    int ret = std::system(cmd.c_str());
    ASSERT_EQ(ret, 0);

    std::ifstream in(base + "/_sep/testbed/config_back.json");
    ASSERT_TRUE(in.is_open());
    nlohmann::json j2;
    in >> j2;
    MemoryTierManager::Config cfg2 = j2.get<MemoryTierManager::Config>();

    EXPECT_EQ(cfg2.stm_size, cfg.stm_size * 2);
    EXPECT_EQ(cfg2.mtm_size, cfg.mtm_size);
    EXPECT_EQ(cfg2.ltm_size, cfg.ltm_size);
}
