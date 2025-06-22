#include <config/manager.h>
#include <config/types.h>
#include <gtest/gtest.h>

using namespace sep::config;

class ConfigManagerTest : public ::testing::Test {
protected:
  void SetUp() override {
    // Reset to defaults before each test
    ConfigManager::getInstance().resetToDefaults();
  }
};

TEST_F(ConfigManagerTest, InitializeDefault) {
  auto &cm = ConfigManager::getInstance();
  const char *argv[] = {"test_program"};

  // First initialization
  cm.initialize(1, const_cast<char **>(argv));
  auto config1 = cm.getConfig();
  EXPECT_FALSE(config1.api.host.empty());

  // Second initialization should work
  cm.initialize(1, const_cast<char **>(argv));
  auto config2 = cm.getConfig();
  EXPECT_FALSE(config2.api.host.empty());
}

TEST_F(ConfigManagerTest, LoadFromFile) {
  auto &cm = ConfigManager::getInstance();

  // Try loading from non-existent file
  EXPECT_FALSE(cm.loadFromFile("non_existent_config.json"));

  // Load from test data config should work
  EXPECT_TRUE(cm.loadFromFile("testdata/config/default_config.json"));

  auto config = cm.getConfig();
  EXPECT_FALSE(config.api.host.empty());
  EXPECT_EQ(config.api.port, 8080);
}

TEST_F(ConfigManagerTest, DISABLED_LoadFromEnvironment) {
  auto &cm = ConfigManager::getInstance();

  // Load from environment variables - disabled since environment not set up in
  // test
  EXPECT_TRUE(cm.loadFromEnvironment());

  auto config = cm.getConfig();
  EXPECT_FALSE(config.api.host.empty());
}

TEST_F(ConfigManagerTest, ComponentSpecificAccess) {
  auto &cm = ConfigManager::getInstance();

  // Get component configs
  const auto &api_config = cm.getAPIConfig();
  const auto &cuda_config = cm.getCUDAConfig();
  const auto &log_config = cm.getLogConfig();

  // Verify default values
  EXPECT_FALSE(api_config.host.empty());
  EXPECT_GT(cuda_config.max_memory_mb, 0);
  EXPECT_FALSE(log_config.log_file.empty());
}

TEST_F(ConfigManagerTest, LoadFromCommandLine) {
  auto &cm = ConfigManager::getInstance();

  const char *argv[] = {"test_program", "--port", "8080"};

  EXPECT_TRUE(cm.loadFromCommandLine(3, const_cast<char **>(argv)));

  const auto &api_config = cm.getAPIConfig();
  EXPECT_EQ(api_config.port, 8080);
}

TEST_F(ConfigManagerTest, UpdateConfigs) {
  auto &cm = ConfigManager::getInstance();

  // Update API config
  APIConfig api_config = cm.getAPIConfig();
  api_config.host = "new_host";
  cm.updateAPIConfig(api_config);
  EXPECT_EQ(cm.getAPIConfig().host, "new_host");

  // Update CUDA config
  CUDAConfig cuda_config = cm.getCUDAConfig();
  cuda_config.max_memory_mb = 2048;
  cm.updateCUDAConfig(cuda_config);
  EXPECT_EQ(cm.getCUDAConfig().max_memory_mb, 2048);

  // Update Log config
  LogConfig log_config = cm.getLogConfig();
  log_config.level = "DEBUG";
  cm.updateLogConfig(log_config);
  EXPECT_EQ(cm.getLogConfig().level, "DEBUG");
}

TEST_F(ConfigManagerTest, ResetToDefaults) {
  auto &cm = ConfigManager::getInstance();

  // Modify some settings
  APIConfig api_config = cm.getAPIConfig();
  api_config.host = "modified_host";
  cm.updateAPIConfig(api_config);

  // Reset to defaults
  cm.resetToDefaults();

  // Verify settings are back to defaults
  EXPECT_NE(cm.getAPIConfig().host, "modified_host");
}
