#include "core/engine.h"

#include <gtest/gtest.h>

#include <vector>

#include "core/types.h"
#include "cuda/types.h"

#include "core/error_handler.h"

using namespace sep;
using namespace sep::cuda;
using namespace sep::core;

class EngineTest : public ::testing::Test {
 protected:
  void SetUp() override {
    engine = std::make_unique<Engine>();
    ASSERT_TRUE(engine->init(sep::config::APIConfig{}));
  }

  void TearDown() override { engine.reset(); }

  // Helper to create test input states
  std::vector<PinState> createTestStates(size_t count) {
    std::vector<PinState> states(count);
    for (size_t i = 0; i < count; i++) {
      states[i].state = i;
      states[i].flags = ~0u;  // All flags set
    }
    return states;
  }

  std::unique_ptr<Engine> engine;
};

// Basic initialization tests
TEST_F(EngineTest, InitializationSucceeds) {
  EXPECT_NO_THROW({
    Engine engine;
    EXPECT_TRUE(engine.init(sep::config::APIConfig{}));
  });
}

TEST_F(EngineTest, MultipleInstancesInitialization) {
  EXPECT_NO_THROW({
    Engine engine1;
    Engine engine2;
    EXPECT_TRUE(engine1.init(sep::config::APIConfig{}));
    EXPECT_TRUE(engine2.init(sep::config::APIConfig{}));
  });
}

// QBSA Processing Tests
TEST_F(EngineTest, QBSAProcessingBasic) {
  auto inputs = createTestStates(10);
  std::vector<uint32_t> indices;
  std::vector<uint32_t> expectations;

  EXPECT_NO_THROW(engine->generate_probes(inputs, indices, expectations, 0));

  EXPECT_EQ(indices.size(), inputs.size());
  EXPECT_EQ(expectations.size(), inputs.size());
}

TEST_F(EngineTest, QBSAProcessingLargeBatch) {
  auto inputs = createTestStates(1024);  // Max batch size
  std::vector<uint32_t> indices;
  std::vector<uint32_t> expectations;

  EXPECT_NO_THROW(engine->generate_probes(inputs, indices, expectations, 0));

  EXPECT_EQ(indices.size(), inputs.size());
  EXPECT_EQ(expectations.size(), inputs.size());
}

TEST_F(EngineTest, QBSAProcessingEmptyInput) {
  std::vector<PinState> empty_inputs;
  std::vector<uint32_t> indices;
  std::vector<uint32_t> expectations;

  core::ErrorHandler::instance().clearErrors();
  engine->generate_probes(empty_inputs, indices, expectations, 0);
  EXPECT_TRUE(core::ErrorHandler::instance().hasErrors());
  core::ErrorHandler::instance().clearErrors();
}

// QSH Processing Tests
TEST_F(EngineTest, QSHProcessingBasic) {
  auto inputs = createTestStates(10);
  sep::quantum::QBSAResult qbsa_result;
  sep::cuda::QSHResult qsh_result;

  EXPECT_NO_THROW(engine->process_batch(inputs, 0, qbsa_result, qsh_result));

  EXPECT_EQ(qsh_result.collapse_indices.size(), inputs.size());
  EXPECT_EQ(qsh_result.collapse_counts.size(), inputs.size());
  EXPECT_GE(qsh_result.total_collapses, 0);
}

TEST_F(EngineTest, QSHProcessingLargeBatch) {
  auto inputs = createTestStates(1024);
  sep::quantum::QBSAResult qbsa_result;
  sep::cuda::QSHResult qsh_result;

  EXPECT_NO_THROW(engine->process_batch(inputs, 0, qbsa_result, qsh_result));

  EXPECT_EQ(qsh_result.collapse_indices.size(), inputs.size());
  EXPECT_EQ(qsh_result.collapse_counts.size(), inputs.size());
}

TEST_F(EngineTest, QSHProcessingEmptyInput) {
  std::vector<PinState> empty_inputs;
  sep::quantum::QBSAResult qbsa_result;
  sep::cuda::QSHResult qsh_result;

  core::ErrorHandler::instance().clearErrors();
  engine->process_batch(empty_inputs, 0, qbsa_result, qsh_result);
  EXPECT_TRUE(core::ErrorHandler::instance().hasErrors());
  core::ErrorHandler::instance().clearErrors();
}

// Combined Processing Tests
TEST_F(EngineTest, CombinedProcessingFlow) {
  auto inputs = createTestStates(100);

  // First generate probes
  std::vector<uint32_t> indices;
  std::vector<uint32_t> expectations;
  EXPECT_NO_THROW(engine->generate_probes(inputs, indices, expectations, 0));

  // Then process the batch
  sep::quantum::QBSAResult qbsa_result;
  sep::cuda::QSHResult qsh_result;
  EXPECT_NO_THROW(engine->process_batch(inputs, 0, qbsa_result, qsh_result));

  // Verify results
  EXPECT_EQ(indices.size(), inputs.size());
  EXPECT_EQ(expectations.size(), inputs.size());
  EXPECT_EQ(qsh_result.collapse_indices.size(), inputs.size());
  EXPECT_EQ(qsh_result.collapse_counts.size(), inputs.size());
}

// Error Handling Tests
TEST_F(EngineTest, ProcessBatchWithInvalidInputs) {
  std::vector<PinState> invalid_inputs(2000);  // Exceeds default batch size
  sep::quantum::QBSAResult qbsa_result;
  sep::cuda::QSHResult qsh_result;

  core::ErrorHandler::instance().clearErrors();
  engine->process_batch(invalid_inputs, 0, qbsa_result, qsh_result);
  EXPECT_TRUE(core::ErrorHandler::instance().hasErrors());
  core::ErrorHandler::instance().clearErrors();
}

TEST_F(EngineTest, GenerateProbesWithInvalidInputs) {
  std::vector<PinState> invalid_inputs(2000);  // Exceeds default batch size
  std::vector<uint32_t> indices;
  std::vector<uint32_t> expectations;

  core::ErrorHandler::instance().clearErrors();
  engine->generate_probes(invalid_inputs, indices, expectations, 0);
  EXPECT_TRUE(core::ErrorHandler::instance().hasErrors());
  core::ErrorHandler::instance().clearErrors();
}

// Resource Management Tests
TEST_F(EngineTest, MultipleProcessingCycles) {
  for (int i = 0; i < 10; i++) {
    auto inputs = createTestStates(100);
    sep::quantum::QBSAResult qbsa_result;
    sep::cuda::QSHResult qsh_result;

    EXPECT_NO_THROW(engine->process_batch(inputs, i, qbsa_result, qsh_result))
        << "Failed on iteration " << i;

    EXPECT_EQ(qsh_result.collapse_indices.size(), inputs.size())
        << "Invalid collapse indices size on iteration " << i;
    EXPECT_EQ(qsh_result.collapse_counts.size(), inputs.size())
        << "Invalid collapse counts size on iteration " << i;
  }
}

// Stress Tests
TEST_F(EngineTest, RapidSequentialProcessing) {
  const int NUM_ITERATIONS = 100;
  const int BATCH_SIZE = 64;

  for (int i = 0; i < NUM_ITERATIONS; i++) {
    auto inputs = createTestStates(BATCH_SIZE);
    sep::quantum::QBSAResult qbsa_result;
    sep::cuda::QSHResult qsh_result;

    EXPECT_NO_THROW(engine->process_batch(inputs, i, qbsa_result, qsh_result))
        << "Failed on iteration " << i;
  }
}

TEST_F(EngineTest, AlternatingBatchSizes) {
  std::vector<size_t> batch_sizes = {1, 10, 100, 500, 1000};

  for (size_t batch_size : batch_sizes) {
    auto inputs = createTestStates(batch_size);
    sep::quantum::QBSAResult qbsa_result;
    sep::cuda::QSHResult qsh_result;

    EXPECT_NO_THROW(engine->process_batch(inputs, 0, qbsa_result, qsh_result))
        << "Failed with batch size " << batch_size;

    EXPECT_EQ(qsh_result.collapse_indices.size(), batch_size)
        << "Invalid collapse indices size for batch size " << batch_size;
    EXPECT_EQ(qsh_result.collapse_counts.size(), batch_size)
        << "Invalid collapse counts size for batch size " << batch_size;
  }
}
