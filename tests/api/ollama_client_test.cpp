#include "api/ollama_client.h"
#include "api/ollama_types.h"
#include <cstdio>
#include <fstream>
#include <gtest/gtest.h>
#include <nlohmann/json.hpp>
#include <unistd.h>

namespace {

using namespace sep::api;
using namespace sep::ollama;
using json = nlohmann::json;

// Test client construction and basic move semantics
TEST(OllamaClientTest, CanConstructClient) {
  OllamaConfig config;
  config.host = "http://127.0.0.1";
  config.model = "test-model";
  config.batch_size = 1;
  config.context_window = 512;
  config.gpu.enabled = false;
  config.gpu.memory_fraction = 0.0f;
  EXPECT_NO_THROW({ OllamaClient client(config); });
}

TEST(OllamaClientTest, MoveConstructorWorks) {
  OllamaConfig config;
  config.host = "http://127.0.0.1";
  config.model = "test-model";
  config.batch_size = 1;
  config.context_window = 512;
  config.gpu.enabled = false;
  config.gpu.memory_fraction = 0.0f;

  OllamaClient original(config);
  EXPECT_NO_THROW({ OllamaClient moved(std::move(original)); });
}

// Test error handling when connecting to an invalid host
TEST(OllamaClientTest, GetInvalidHostReturnsError) {
  OllamaConfig config;
  config.host = "http://nonexistent.local";
  config.model = "test-model";
  config.batch_size = 1;
  config.context_window = 512;
  config.gpu.enabled = false;
  config.gpu.memory_fraction = 0.0f;

  OllamaClient client(config);
  std::string result;
  EXPECT_EQ(client.get("endpoint", result), sep::SEPResult::PROCESSING_ERROR);
}

TEST(OllamaClientTest, PostInvalidHostReturnsError) {
  OllamaConfig config;
  config.host = "http://nonexistent.local";
  config.model = "test-model";
  config.batch_size = 1;
  config.context_window = 512;
  config.gpu.enabled = false;
  config.gpu.memory_fraction = 0.0f;

  OllamaClient client(config);
  json payload = {{"prompt", "hello"}};
  std::string result;
  EXPECT_EQ(client.post("endpoint", payload, result), sep::SEPResult::PROCESSING_ERROR);
}

// Test request/response formatting using file:// protocol
TEST(OllamaClientTest, GetFileProtocolReturnsContent) {
  // Create a temporary file with known content
  char tmpfile[] = "/tmp/ollama_test_XXXXXX";
  int fd = mkstemp(tmpfile);
  ASSERT_GE(fd, 0) << "Failed to create temporary file";
  std::string filename(tmpfile);
  const std::string fileContent = "hello file protocol";

  // Write content to temp file
  write(fd, fileContent.c_str(), fileContent.length());
  close(fd);

  // Configure client to use file:// URL
  OllamaConfig config;
  config.host = std::string("file://") + filename;
  config.model = "";
  config.batch_size = 1;
  config.context_window = 512;
  config.gpu.enabled = false;
  config.gpu.memory_fraction = 0.0f;

  OllamaClient client(config);
  std::string result;
  EXPECT_NO_THROW({ result = client.get(""); });
  EXPECT_EQ(result, fileContent);

  // Clean up temporary file
  std::remove(filename.c_str());
}

} // namespace
