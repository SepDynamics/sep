#include "api/js_integration.h"
#include "mock_bridge.h"
#include <atomic>
#include <gtest/gtest.h>
#include <string>
#include <thread>

using namespace sep::test;

class JSIntegrationTest : public ::testing::Test {
protected:
  void SetUp() override {
    // Reset mock state with thread safety
    MockGuard guard;
    g_mock_result.clear();
    g_mock_error.clear();
    g_mock_required_size = 0;
    g_should_fail = false;
    g_should_resize = false;
  }
};

// Basic successful processing test
TEST_F(JSIntegrationTest, BasicProcessing) {
  {
    MockGuard guard;
    g_mock_result = R"({"status": "success"})";
  }

  auto result = sep::api::JSIntegration::processContextCheck(
      R"({"input": "test"})", "test_layer");

  {
    MockGuard guard;
    EXPECT_EQ(result, g_mock_result);
  }
}

// Test buffer resizing with explicit size
TEST_F(JSIntegrationTest, BufferResizingExplicit) {
  std::string expected_result;
  {
    MockGuard guard;
    g_mock_result = std::string(1000, 'x'); // Large result
    g_should_resize = true;
    g_mock_required_size = 2000; // Explicitly request larger buffer
    expected_result = g_mock_result;
  }

  auto result = sep::api::JSIntegration::processContextCheck(
      R"({"input": "test"})", "test_layer");
  EXPECT_EQ(result, expected_result);
}

// Test buffer resizing with fallback doubling
TEST_F(JSIntegrationTest, BufferResizingFallback) {
  std::string expected_result;
  {
    MockGuard guard;
    g_mock_result = std::string(1000, 'x'); // Large result
    g_should_resize = true;
    g_mock_required_size = 0; // Force fallback to doubling
    expected_result = g_mock_result;
  }

  auto result = sep::api::JSIntegration::processContextCheck(
      R"({"input": "test"})", "test_layer");
  EXPECT_EQ(result, expected_result);
}

// Test error handling
TEST_F(JSIntegrationTest, ErrorHandling) {
  std::string expected_error;
  {
    MockGuard guard;
    g_should_fail = true;
    g_mock_error = "Test error message";
    expected_error = R"({"error":"Test error message"})";
  }

  auto result = sep::api::JSIntegration::processContextCheck(
      R"({"input": "test"})", "test_layer");
  EXPECT_EQ(result, expected_error);
}

// Test empty input
TEST_F(JSIntegrationTest, EmptyInput) {
  std::string expected_result;
  {
    MockGuard guard;
    g_mock_result = "{}";
    expected_result = g_mock_result;
  }

  auto result = sep::api::JSIntegration::processContextCheck("", "");
  EXPECT_EQ(result, expected_result);
}

// Test large input
TEST_F(JSIntegrationTest, LargeInput) {
  std::string large_input(10000, 'x');
  std::string expected_result;
  {
    MockGuard guard;
    g_mock_result = R"({"status": "success"})";
    expected_result = g_mock_result;
  }

  auto result =
      sep::api::JSIntegration::processContextCheck(large_input, "test_layer");
  EXPECT_EQ(result, expected_result);
}

// Test multiple resize attempts
TEST_F(JSIntegrationTest, MultipleResizeAttempts) {
  std::string expected_result;
  {
    MockGuard guard;
    g_mock_result = std::string(5000, 'x');
    g_should_resize = true;
    g_mock_required_size = 0; // Force multiple doubling attempts
    expected_result = g_mock_result;
  }

  auto result = sep::api::JSIntegration::processContextCheck(
      R"({"input": "test"})", "test_layer");
  EXPECT_EQ(result, expected_result);
}

// Test with embedded null characters
TEST_F(JSIntegrationTest, EmbeddedNulls) {
  std::string expected_result = "before";
  {
    MockGuard guard;
    g_mock_result = std::string("before\0after", 11); // Include null in middle
  }

  auto result = sep::api::JSIntegration::processContextCheck(
      R"({"input": "test"})", "test_layer");
  EXPECT_EQ(result, expected_result); // Should stop at first null
}

// Test with various layer names
TEST_F(JSIntegrationTest, LayerNames) {
  std::string expected_result;
  {
    MockGuard guard;
    g_mock_result = R"({"status": "success"})";
    expected_result = g_mock_result;
  }

  std::vector<std::string> layers = {
      "layer1", "deep.nested.layer", "",
      std::string(100, 'a') // Very long layer name
  };

  for (const auto &layer : layers) {
    auto result = sep::api::JSIntegration::processContextCheck(
        R"({"input": "test"})", layer);
    EXPECT_EQ(result, expected_result);
  }
}

// Test invalid JSON handling
TEST_F(JSIntegrationTest, InvalidJSON) {
  std::string expected_error;
  {
    MockGuard guard;
    g_should_fail = true;
    g_mock_error = "Invalid JSON";
    expected_error = R"({"error":"Invalid JSON"})";
  }

  auto result = sep::api::JSIntegration::processContextCheck("invalid json",
                                                             "test_layer");
  EXPECT_EQ(result, expected_error);
}

// Test maximum buffer size handling
TEST_F(JSIntegrationTest, MaxBufferSize) {
  {
    MockGuard guard;
    g_should_resize = true;
    g_mock_required_size =
        1024 * 1024 * 1024; // 1GB, more reasonable than size_t::max()
  }

  auto result = sep::api::JSIntegration::processContextCheck(
      R"({"input": "test"})", "test_layer");
  EXPECT_TRUE(result.find("error") != std::string::npos);
}

// Test concurrent processing
TEST_F(JSIntegrationTest, ConcurrentProcessing) {
  std::string expected_result;
  {
    MockGuard guard;
    g_mock_result = R"({"status": "success"})";
    expected_result = g_mock_result;
    g_should_fail = false;
    g_should_resize = false;
    g_mock_required_size = 0;
  }

  std::vector<std::thread> threads;
  std::atomic<int> success_count{0};

  for (int i = 0; i < 10; ++i) {
    threads.emplace_back([&success_count, expected_result]() {
      auto result = sep::api::JSIntegration::processContextCheck(
          R"({"input": "test"})", "test_layer");
      if (result == expected_result) {
        success_count++;
      }
    });
  }

  for (auto &thread : threads) {
    thread.join();
  }

  EXPECT_EQ(success_count.load(), 10);
}
