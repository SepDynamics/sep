#include <gtest/gtest.h>
#include "config/manager.h"

TEST(ConfigManagerEnv, LoadMetricsAndKeepAlive) {
    using sep::config::ConfigManager;
    setenv("SEP_API_ENABLE_METRICS", "0", 1);
    setenv("SEP_API_KEEP_ALIVE_TIMEOUT_MS", "7000", 1);

    auto &cm = ConfigManager::getInstance();
    cm.resetToDefaults();
    cm.loadFromEnvironment();

    const auto &api = cm.getAPIConfig();
    EXPECT_FALSE(api.enable_metrics);
    EXPECT_EQ(api.keep_alive_timeout_ms, static_cast<size_t>(7000));

    unsetenv("SEP_API_ENABLE_METRICS");
    unsetenv("SEP_API_KEEP_ALIVE_TIMEOUT_MS");
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
