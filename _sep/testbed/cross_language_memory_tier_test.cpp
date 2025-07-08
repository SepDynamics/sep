#include <gtest/gtest.h>
#include "memory/memory_tier_manager.hpp"
#include <fstream>
#include <nlohmann/json.hpp>
#include <filesystem>

using namespace sep::memory;

TEST(CrossLanguage, MemoryTierConfigRoundTrip) {
    MemoryTierManager::Config cfg;
    cfg.stm_size = 1024;
    cfg.mtm_size = 2048;
    cfg.ltm_size = 4096;

    namespace fs = std::filesystem;

    nlohmann::json j = cfg;
    fs::path test_dir = fs::path(__FILE__).parent_path();
    std::ofstream out(test_dir / "config_out.json");
    ASSERT_TRUE(out.is_open());
    out << j.dump();
    out.close();

    int ret = std::system(("node " + (test_dir / "cross_language_memory.cjs").string() +
                          " " + (test_dir / "config_out.json").string() +
                          " " + (test_dir / "config_back.json").string()).c_str());
    ASSERT_EQ(ret, 0);

    std::ifstream in(test_dir / "config_back.json");
    ASSERT_TRUE(in.is_open());
    nlohmann::json j2;
    in >> j2;
    MemoryTierManager::Config cfg2 = j2.get<MemoryTierManager::Config>();

    EXPECT_EQ(cfg2.stm_size, cfg.stm_size * 2);
    EXPECT_EQ(cfg2.mtm_size, cfg.mtm_size);
    EXPECT_EQ(cfg2.ltm_size, cfg.ltm_size);
}
