#include "api/bridge.h"

#include <gtest/gtest.h>
#include <cstring>

#include <atomic>
#include <memory>
#include <nlohmann/json.hpp>
#include <string>
#include <thread>
#include <cstring>

#include "api/bridge.hpp"
#include "context/mock_processor.hpp"
#include "context/processor.h"

using namespace sep::api::bridge;
using json = nlohmann::json;

class BridgeTest : public ::testing::Test {
 protected:
  void SetUp() override { ASSERT_EQ(sep_bridge_init(), sep::SEPResult::SUCCESS); }

  void TearDown() override { sep_bridge_cleanup(); }

  // Helper to create a test context
  sep::context::Context createTestContext() {
    sep::context::Context ctx;
    ctx.type = "test_type";
    ctx.content = "key:value";
    ctx.relationships = {"rel1", "rel2"};
    ctx.tags = {"tag1", "tag2"};
    ctx.metadata = "meta_key:meta_value";
    ctx.processorResult = "result_key:result_value";
    return ctx;
  }

  // Helper to create a test check result
  sep::context::CheckResult createTestCheckResult() {
    sep::context::CheckResult result;
    result.status = sep::context::CheckResult::Status::VALID;
    result.score = 0.95f;
    result.error = "";
    return result;
  }
};

// JSON Conversion Tests
TEST_F(BridgeTest, ContextJsonConversion) {
  auto original = createTestContext();
  auto json = contextToJson(original);
  auto converted = jsonToContext(json);

  EXPECT_EQ(converted.type, original.type);
  EXPECT_EQ(converted.content, original.content);
  EXPECT_EQ(converted.relationships, original.relationships);
  EXPECT_EQ(converted.tags, original.tags);
  EXPECT_EQ(converted.metadata, original.metadata);
  EXPECT_EQ(converted.processorResult, original.processorResult);
}

TEST_F(BridgeTest, CheckResultJsonConversion) {
  auto original = createTestCheckResult();
  auto json = resultToJson(original);
  auto converted = jsonToCheckResult(json);

  EXPECT_EQ(converted.status, original.status);
  EXPECT_FLOAT_EQ(converted.score, original.score);
  EXPECT_EQ(converted.error, original.error);
}

// Context Processing Tests
TEST_F(BridgeTest, BasicContextProcessing) {
  json context = {{"type", "test"},
                  {"content", {{"data", "test"}}},
                  {"relationships", json::array()},
                  {"tags", {"test"}},
                  {"metadata", json::object()}};

  char result_buffer[1024];
  sep::SEPResult result = sep_process_context(context.dump().c_str(), "test_layer", result_buffer,
                                   sizeof(result_buffer));
  EXPECT_EQ(result, sep::SEPResult::SUCCESS) << "Process context failed with code " << static_cast<int>(result);

  json result_json = json::parse(result_buffer);
  EXPECT_TRUE(result_json["success"].get<bool>());
  EXPECT_TRUE(result_json["results"].is_array());
}

TEST_F(BridgeTest, InvalidJsonProcessing) {
  const char* invalid_json = "invalid json";
  char result_buffer[1024];

  EXPECT_EQ(sep_process_context(invalid_json, "test_layer", result_buffer, sizeof(result_buffer)),
            sep::SEPResult::PROCESSING_ERROR);

  char error_buffer[1024];
  sep_bridge_get_last_error(error_buffer, sizeof(error_buffer));
  EXPECT_TRUE(std::string(error_buffer).find("JSON parsing error") != std::string::npos);
}

// Buffer Management Tests
TEST_F(BridgeTest, BufferSizeHandling) {
  json large_context = {{"type", "test"},
                        {"content", {{"data", "large content"}}},
                        {"relationships", {{"type", "large"}, {"data", "more content"}}},
                        {"tags", {"tag1", "tag2", "tag3"}},
                        {"metadata", {{"key1", "value1"}, {"key2", "value2"}}}};

  char small_buffer[10];
  EXPECT_EQ(sep_process_context(large_context.dump().c_str(), "test_layer", small_buffer,
                                sizeof(small_buffer)),
            sep::SEPResult::BUFFER_TOO_SMALL);

  size_t required_size = sep_get_required_buffer_size();
  EXPECT_GT(required_size, sizeof(small_buffer));
}

// Error Handling Tests
TEST_F(BridgeTest, NullParameterHandling) {
  char buffer[1024];
  EXPECT_EQ(sep_process_context(nullptr, "layer", buffer, sizeof(buffer)), sep::SEPResult::INVALID_ARGUMENT);
  EXPECT_EQ(sep_process_context("test", nullptr, buffer, sizeof(buffer)), sep::SEPResult::INVALID_ARGUMENT);
  EXPECT_EQ(sep_process_context("test", "layer", nullptr, sizeof(buffer)), sep::SEPResult::INVALID_ARGUMENT);
}

TEST_F(BridgeTest, ErrorBufferHandling) {
  char error_buffer[1024];

  // Test with null buffer
  EXPECT_EQ(sep_bridge_get_last_error(nullptr, 1024), sep::SEPResult::INVALID_ARGUMENT);

  // Test with zero size
  EXPECT_EQ(sep_bridge_get_last_error(error_buffer, 0), sep::SEPResult::INVALID_ARGUMENT);

  // Test with small buffer
  detail::setLastError("Long error message for testing buffer handling");
  char small_buffer[5];
  sep_bridge_get_last_error(small_buffer, sizeof(small_buffer));
  EXPECT_EQ(std::strlen(small_buffer), 4);  // 4 chars + null terminator
}

// Configuration Tests
TEST_F(BridgeTest, ConfigurationManagement) {
  char buffer[1024];

  // Test invalid parameters
  EXPECT_EQ(sep_bridge_set_config(nullptr, "value"), sep::SEPResult::UNKNOWN_ERROR);
  EXPECT_EQ(sep_bridge_set_config("key", nullptr), sep::SEPResult::UNKNOWN_ERROR);
  EXPECT_EQ(sep_bridge_get_config(nullptr, buffer, sizeof(buffer)), sep::SEPResult::UNKNOWN_ERROR);
  EXPECT_EQ(sep_bridge_get_config("key", nullptr, sizeof(buffer)), sep::SEPResult::UNKNOWN_ERROR);

  // Basic set and get using ConfigManager
  EXPECT_EQ(sep_bridge_set_config("api.host", "example.com"), sep::SEPResult::SUCCESS);
  EXPECT_EQ(sep_bridge_get_config("api.host", buffer, sizeof(buffer)), sep::SEPResult::SUCCESS);
  EXPECT_STREQ(buffer, "example.com");

  // Non-existent key
  EXPECT_EQ(sep_bridge_get_config("api.missing", buffer, sizeof(buffer)), sep::SEPResult::INVALID_ARGUMENT);
}

// Callback Registration Tests
TEST_F(BridgeTest, CallbackRegistration) {
  static bool called = false;
  static std::string data;
  auto callback = [](const char* d) {
    called = true;
    data = d ? d : "";
  };

  // Test invalid parameters
  EXPECT_EQ(sep_bridge_register_callback(nullptr, callback), sep::SEPResult::UNKNOWN_ERROR);
  EXPECT_EQ(sep_bridge_register_callback("event", nullptr), sep::SEPResult::UNKNOWN_ERROR);

  // Successful registration and invocation
  EXPECT_EQ(sep_bridge_register_callback("test_event", callback), sep::SEPResult::SUCCESS);
  detail::invokeCallbacks("test_event", "payload");
  EXPECT_TRUE(called);
  EXPECT_EQ(data, "payload");
}

// Thread Safety Tests
TEST_F(BridgeTest, ConcurrentProcessing) {
  const int num_threads = 10;
  std::vector<std::thread> threads;
  std::atomic<int> success_count{0};

  json context = {{"type", "test"},
                  {"content", json::object()},
                  {"relationships", json::array()},
                  {"tags", json::array()},
                  {"metadata", json::object()}};
  const std::string context_json = context.dump();

  for (int i = 0; i < num_threads; ++i) {
    threads.emplace_back([&]() {
      char buffer[1024];
      if (sep_process_context(context_json.c_str(), "test_layer", buffer, sizeof(buffer)) == sep::SEPResult::SUCCESS) {
        success_count++;
      }
    });
  }

  for (auto& thread : threads) {
    thread.join();
  }

  EXPECT_EQ(success_count, num_threads);
}

// Cleanup Tests
TEST_F(BridgeTest, MultipleInitCleanup) {
  // Test multiple init/cleanup cycles
  for (int i = 0; i < 5; ++i) {
    EXPECT_EQ(sep_bridge_cleanup(), sep::SEPResult::SUCCESS);
    EXPECT_EQ(sep_bridge_init(), sep::SEPResult::SUCCESS);
  }
}

TEST_F(BridgeTest, ProcessingAfterCleanup) {
  sep_bridge_cleanup();

  char buffer[1024];
  EXPECT_EQ(sep_process_context(R"({"type": "test"})", "layer", buffer, sizeof(buffer)), sep::SEPResult::UNKNOWN_ERROR);

  char error_buffer[1024];
  sep_bridge_get_last_error(error_buffer, sizeof(error_buffer));
  EXPECT_STREQ(error_buffer, "Context processor not initialized");
}
