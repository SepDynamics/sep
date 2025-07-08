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

    nlohmann::json j = cfg;
    namespace fs = std::filesystem;
    fs::path base = fs::temp_directory_path() / "sep_test";
    fs::create_directories(base);
    fs::path out_json = base / "config_out.json";
    fs::path back_json = base / "config_back.json";
    fs::path script = fs::path(__FILE__).parent_path() / "cross_language_memory.cjs";

    std::ofstream out(out_json);
    ASSERT_TRUE(out.is_open());
    out << j.dump();
    out.close();

    std::string cmd = std::string("node ") + script.string() + " " + out_json.string() + " " + back_json.string();
    int ret = std::system(cmd.c_str());
    ASSERT_EQ(ret, 0);

    std::ifstream in(back_json);
    ASSERT_TRUE(in.is_open());
    nlohmann::json j2;
    in >> j2;
    MemoryTierManager::Config cfg2 = j2.get<MemoryTierManager::Config>();

    EXPECT_EQ(cfg2.stm_size, cfg.stm_size * 2);
    EXPECT_EQ(cfg2.mtm_size, cfg.mtm_size);
    EXPECT_EQ(cfg2.ltm_size, cfg.ltm_size);
}
