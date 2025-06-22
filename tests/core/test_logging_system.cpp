#include <logging/manager.h>
#include <spdlog/sinks/ostream_sink.h>
#include "test_utils.hpp"
#include <filesystem>
#include <gtest/gtest.h>
#include <sstream>
#include <thread>

namespace sep::test {

class LoggingSystemTest : public TestWithMutation {
protected:
  void setupMutations() override {
    registerMutation("Log level filtering disabled", "logging/manager.cpp",
                     [this]() { return testLogLevelMutation(); });

    registerMutation("Timestamp generation disabled", "logging/manager.cpp",
                     [this]() { return testTimestampMutation(); });

    registerMutation("Thread ID logging disabled", "logging/manager.cpp",
                     [this]() { return testThreadIdMutation(); });
  }

  std::shared_ptr<spdlog::logger>
  createMemoryLogger(const std::string &name, std::stringstream &ss,
                     logging::Level level = logging::Level::INFO) {
    logging::LoggerConfig cfg;
    cfg.name = name;
    cfg.level = level;
    cfg.console.enabled = false;
    auto logger =
        logging::Manager::getInstance().createLogger(name, cfg);
    auto sink = std::make_shared<spdlog::sinks::ostream_sink_mt>(ss);
    sink->set_level(spdlog::level::trace);
    logger->sinks().clear();
    logger->sinks().push_back(sink);
    logger->set_pattern("[%Y-%m-%d %H:%M:%S] [TID:%t] [%l] %v");
    return logger;
  }

  bool testLogLevelMutation() {
    std::stringstream ss;
    auto logger = createMemoryLogger("log_level_mut", ss);

    logger->set_level(spdlog::level::warn);
    logger->log(spdlog::level::info, "This should not appear");
    logger->log(spdlog::level::err, "This should appear");

    std::string output = ss.str();
    return output.find("This should not appear") == std::string::npos &&
           output.find("This should appear") != std::string::npos;
  }

  bool testTimestampMutation() {
    std::stringstream ss;
    auto logger = createMemoryLogger("timestamp_mut", ss);

    logger->info("Test message");

    std::string output = ss.str();
    return output.find("[20") != std::string::npos; // Basic timestamp check
  }

  bool testThreadIdMutation() {
    std::stringstream ss;
    auto logger = createMemoryLogger("thread_id_mut", ss);

    std::thread t([&logger]() { logger->info("Thread message"); });
    t.join();

    std::string output = ss.str();
    return output.find("TID:") != std::string::npos;
  }

  void SetUp() override {
    TestWithMutation::SetUp();
    // Create test log directory
    std::filesystem::create_directories("test_logs");
  }

  void TearDown() override {
    // Cleanup test logs
    std::filesystem::remove_all("test_logs");
    TestWithMutation::TearDown();
  }
};

TEST_F(LoggingSystemTest, BasicLogging) {
  std::stringstream ss;
  auto logger = createMemoryLogger("basic", ss);

  logger->info("Test message");
  std::string output = ss.str();

  EXPECT_TRUE(output.find("Test message") != std::string::npos);
  EXPECT_TRUE(output.find("[INFO]") != std::string::npos);
}

TEST_F(LoggingSystemTest, LogLevelFiltering) {
  std::stringstream ss;
  auto logger = createMemoryLogger("level_filter", ss);

  logger->set_level(spdlog::level::warn);
  logger->info("Info message");
  logger->warn("Warning message");
  logger->error("Error message");

  std::string output = ss.str();
  EXPECT_TRUE(output.find("Info message") == std::string::npos);
  EXPECT_TRUE(output.find("Warning message") != std::string::npos);
  EXPECT_TRUE(output.find("Error message") != std::string::npos);
}

TEST_F(LoggingSystemTest, FileLogging) {
  const std::string logFile = "test_logs/test.log";
  logging::LoggerConfig cfg;
  cfg.name = "file_logger";
  cfg.level = logging::Level::INFO;
  cfg.file.path = logFile;
  cfg.console.enabled = false;
  auto logger = logging::Manager::getInstance().createLogger("file_logger", cfg);

  logger->info("File test message");

  // Read back the log file
  std::ifstream file(logFile);
  std::stringstream buffer;
  buffer << file.rdbuf();
  std::string output = buffer.str();

  EXPECT_TRUE(output.find("File test message") != std::string::npos);
}

TEST_F(LoggingSystemTest, ThreadSafety) {
  std::stringstream ss;
  auto logger = createMemoryLogger("thread_safe", ss);

  constexpr int numThreads = 10;
  constexpr int messagesPerThread = 100;

  std::vector<std::thread> threads;
  for (int i = 0; i < numThreads; i++) {
    threads.emplace_back([&logger, i]() {
      for (int j = 0; j < messagesPerThread; j++) {
        logger->info("Thread {} Message {}", i, j);
      }
    });
  }

  for (auto &thread : threads) {
    thread.join();
  }

  std::string output = ss.str();
  int messageCount = 0;
  std::string::size_type pos = 0;
  while ((pos = output.find("Thread", pos)) != std::string::npos) {
    messageCount++;
    pos++;
  }

  EXPECT_EQ(messageCount, numThreads * messagesPerThread);
}

TEST_F(LoggingSystemTest, LogRotation) {
  const std::string logFile = "test_logs/rotating.log";
  logging::LoggerConfig cfg;
  cfg.name = "rotation_logger";
  cfg.level = logging::Level::INFO;
  cfg.file.path = logFile;
  cfg.file.max_size = 1024; // 1KB
  cfg.file.max_files = 2;
  cfg.console.enabled = false;
  auto logger = logging::Manager::getInstance().createLogger("rotation_logger", cfg);

  std::string longMessage(256, 'X'); // 256 bytes message
  for (int i = 0; i < 10; i++) {
    logger->info("{}", longMessage);
  }

  // Check that rotation occurred
  EXPECT_TRUE(std::filesystem::exists(logFile + ".1"));
}

TEST_F(LoggingSystemTest, Formatting) {
  std::stringstream ss;
  auto logger = createMemoryLogger("formatting", ss);

  logger->info("Test {} with {}", "message", 42);
  std::string output = ss.str();

  EXPECT_TRUE(output.find("Test message with 42") != std::string::npos);
}

TEST_F(LoggingSystemTest, ContextualLogging) {
  std::stringstream ss;
  auto logger = createMemoryLogger("contextual", ss);

  logger->info("[Module1] Test message");

  std::string output = ss.str();
  EXPECT_TRUE(output.find("[Module1]") != std::string::npos);
}

TEST_F(LoggingSystemTest, MutationAnalysis) {
  auto results = runMutationAnalysis();

  for (const auto &result : results) {
    EXPECT_TRUE(result.killed) << "Mutation not killed: " << result.description
                               << " at " << result.location;
  }
}

} // namespace sep::test
