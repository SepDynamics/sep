/**
 * Unit tests for the context processor implementation
 */

#include "context/processor.h"
#include "context/types.h"

#include <gtest/gtest.h>
#include <memory>
#include <nlohmann/json.hpp>
#include <string>
#include <vector>

namespace sep::test {


// Test fixture for the context processor
class ContextProcessorTest : public ::testing::Test {
protected:
  void SetUp() override {
    // Initialize with default options
    context::ProcessOptions options;
    options.use_gpu = true;
    options.batch_size = 64;
    options.stability_threshold = 0.8f;
    processor = sep::context::createProcessor(options);
  }

  void TearDown() override { processor.reset(); }

  // Helper to create a basic context for testing
  context::Context createTestContext(const std::string &type,
                                     const nlohmann::json &content) {
    context::Context context;
    context.type = type;
    context.content = content;
    context.metadata["timestamp"] = "2023-10-10T12:00:00Z";
    context.metadata["source"] = "test";
    context.metadata["version"] = "1.0";
    context.tags = {"test", "sample"};
    return context;
  }

  // Helper to create a test context with embeddings
  context::Context createEmbeddingContext(const std::vector<float> &embedding) {
    context::Context context;
    context.type = "embedding";
    context.content = embedding;
    context.metadata["timestamp"] = "2023-10-10T12:00:00Z";
    context.metadata["source"] = "test";
    context.metadata["version"] = "1.0";
    return context;
  }

  // Helper to create a batch of test contexts
  context::Batch createTestBatch(const std::string &layer, int count) {
    context::Batch batch;
    batch.layer = layer;
    for (int i = 0; i < count; i++) {
      auto content = nlohmann::json::object();
      content["text"] = "Test message " + std::to_string(i);
      content["value"] = i;
      batch.contexts.push_back(createTestContext("message", content));
    }
    return batch;
  }

  std::unique_ptr<context::Processor> processor;
};

// Test initialization
TEST_F(ContextProcessorTest, Initialization) {
  EXPECT_NE(processor, nullptr);
}

// Test context validation
TEST_F(ContextProcessorTest, ContextValidation) {
  // Valid context
  auto valid_context =
      createTestContext("message", {{"text", "This is a valid message"}});
  auto valid_result = processor->validateContext(valid_context);
  EXPECT_TRUE(valid_result.valid);
  EXPECT_TRUE(valid_result.error.empty());

  // Invalid context (missing timestamp)
  auto invalid_context = valid_context;
  invalid_context.metadata.erase("timestamp");
  auto invalid_result = processor->validateContext(invalid_context);
  EXPECT_FALSE(invalid_result.valid);
  EXPECT_FALSE(invalid_result.error.empty());
}

// Test batch processing
TEST_F(ContextProcessorTest, BatchProcessing) {
  // Create batch with embedding contexts
  context::Batch batch;
  batch.layer = "working";

  for (int i = 0; i < 3; i++) {
    std::vector<float> embedding(
        5, 0.1f * (i + 1)); // Different values for each context
    batch.contexts.push_back(createEmbeddingContext(embedding));
  }

  // Process batch
  auto result = processor->processBatch(batch);

  EXPECT_TRUE(result.success);
  EXPECT_EQ(result.value.size(), batch.contexts.size());

  // Check that all contexts were processed successfully
  for (const auto &check_result : result.value) {
    EXPECT_EQ(check_result.status, context::CheckResult::Status::STABLE);
    EXPECT_GE(check_result.score, 0.0);
    EXPECT_LE(check_result.score, 1.0);
  }
}

// Test embedding extraction
TEST_F(ContextProcessorTest, EmbeddingExtraction) {
  // Create context with direct embeddings
  std::vector<float> test_embedding = {0.1f, 0.2f, 0.3f, 0.4f, 0.5f};
  auto context = createEmbeddingContext(test_embedding);

  // Extract embeddings
  auto result = processor->extractEmbeddings(context);
  EXPECT_TRUE(result.success);
  EXPECT_EQ(result.value.size(), test_embedding.size());
  for (size_t i = 0; i < test_embedding.size(); i++) {
    EXPECT_FLOAT_EQ(result.value[i], test_embedding[i]);
  }
}

// Test similarity calculation
TEST_F(ContextProcessorTest, SimilarityCalculation) {
  // Create two similar contexts
  std::vector<float> embedding1 = {0.1f, 0.2f, 0.3f, 0.4f, 0.5f};
  std::vector<float> embedding2 = {0.11f, 0.21f, 0.31f, 0.41f,
                                   0.51f}; // Slightly different

  auto context1 = createEmbeddingContext(embedding1);
  auto context2 = createEmbeddingContext(embedding2);

  // Calculate similarities
  auto similar_result = processor->calculateSimilarity(context1, context2);
  EXPECT_TRUE(similar_result.success);
  EXPECT_GT(similar_result.value, 0.9f);

  // Self-similarity should be 1.0
  auto self_result = processor->calculateSimilarity(context1, context1);
  EXPECT_TRUE(self_result.success);
  EXPECT_FLOAT_EQ(self_result.value, 1.0f);
}

// Test context blending
TEST_F(ContextProcessorTest, ContextBlending) {
  // Create contexts to blend
  sep::shim::vector<context::Context> contexts;
  std::vector<float> embedding1 = {0.1f, 0.2f, 0.3f, 0.4f, 0.5f};
  std::vector<float> embedding2 = {0.5f, 0.4f, 0.3f, 0.2f, 0.1f};

  contexts.push_back(createEmbeddingContext(embedding1));
  contexts.push_back(createEmbeddingContext(embedding2));

  // Equal weights
  sep::shim::vector<float> weights = {0.5f, 0.5f};

  // Blend contexts
  auto result = processor->blendContexts(contexts, weights);

  EXPECT_TRUE(result.success);
  EXPECT_EQ(result.value.type, "embedding");
  auto blended = result.value.content.get<std::vector<float>>();
  ASSERT_EQ(blended.size(), embedding1.size());
  for (float v : blended) {
    EXPECT_NEAR(v, 0.3f, 1e-5f);
  }
}

} // namespace sep::test
