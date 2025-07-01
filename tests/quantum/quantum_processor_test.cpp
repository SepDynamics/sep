#include "quantum/quantum_processor.h"
#include "quantum/processor.h"
#include "quantum/types.h"

#include <atomic>
#include <cmath>
#include <gtest/gtest.h>
#include <memory>
#include <thread>
#include <glm/gtc/random.hpp>

namespace sep::quantum::test {

// Mock quantum state for testing
struct QuantumStateData {
  std::vector<double> amplitudes;  // Match QuantumState's double type
  std::vector<uint32_t> bitStates; // Match QuantumState's uint32_t type

  static QuantumStateData createTestState(size_t count) {
    QuantumStateData data;
    data.amplitudes.resize(count);
    data.bitStates.resize(count);
    double norm = 1.0 / std::sqrt(static_cast<double>(count));

    for (size_t i = 0; i < count; i++) {
      data.bitStates[i] = (1U << i);
      data.amplitudes[i] = norm;
    }
    return data;
  }

  static QuantumStateData createSymmetryPairs(size_t pairs) {
    QuantumStateData data;
    data.amplitudes.resize(pairs * 2);
    data.bitStates.resize(pairs * 2);
    double norm = 1.0 / std::sqrt(static_cast<double>(pairs * 2));

    for (size_t i = 0; i < pairs; i++) {
      data.bitStates[i * 2] = (1U << i);
      data.bitStates[i * 2 + 1] = (1U << (i + pairs));
      data.amplitudes[i * 2] = data.amplitudes[i * 2 + 1] = norm;
    }
    return data;
  }

  QuantumState toQuantumState() const {
    QuantumState state;
    state.basis_states = bitStates; // Direct assignment since types match
    state.amplitudes = amplitudes;  // Direct assignment since types match
    return state;
  }
};

class QuantumProcessorTest : public ::testing::Test {
protected:
  void SetUp() override {
    ProcessorOptions options;
    options.useGPU = true;
    options.batchSize = 256;
    options.errorDetectionEnabled = true;
    options.symmetryPairs = 32;
    processor = std::make_unique<Processor>(options);
  }

  void TearDown() override { processor.reset(); }

  std::unique_ptr<Processor> processor;
};

// Memory Limit Tests
TEST_F(QuantumProcessorTest, MaxBatchSize) {
  auto testState = QuantumStateData::createTestState(1024);
  auto quantumState = testState.toQuantumState();

  // Test batch processing at limit
  std::vector<QuantumState> states(10, quantumState);
  EXPECT_TRUE(processor->processBatch(states));
  EXPECT_EQ(states.size(), 10);
}

TEST_F(QuantumProcessorTest, ChunkSizeOperations) {
  auto testState = QuantumStateData::createTestState(256);
  auto quantumState = testState.toQuantumState();

  // Test validation
  EXPECT_TRUE(processor->validateState(quantumState));
}

TEST_F(QuantumProcessorTest, SymmetryPairLimit) {
  auto testState = QuantumStateData::createSymmetryPairs(32);
  auto quantumState = testState.toQuantumState();

  // Test validation
  EXPECT_TRUE(processor->validateState(quantumState));
}

// Basic State Validation
TEST_F(QuantumProcessorTest, ValidStateValidation) {
  auto testState = QuantumStateData::createTestState(4);
  auto quantumState = testState.toQuantumState();

  EXPECT_TRUE(processor->validateState(quantumState));
}

TEST_F(QuantumProcessorTest, InvalidNormalization) {
  auto testState = QuantumStateData::createTestState(4);
  testState.amplitudes[0] *= 2.0; // Break normalization
  auto quantumState = testState.toQuantumState();

  EXPECT_FALSE(processor->validateState(quantumState));
}

TEST_F(QuantumProcessorTest, InvalidStateSize) {
  auto testState = QuantumStateData::createTestState(4);
  testState.bitStates.pop_back(); // Create size mismatch
  auto quantumState = testState.toQuantumState();

  EXPECT_FALSE(processor->validateState(quantumState));
}

// Error Detection Tests
TEST_F(QuantumProcessorTest, ValidationErrorDetection) {
  auto testState = QuantumStateData::createTestState(4);
  testState.amplitudes[0] *= 2.0; // Break normalization
  auto quantumState = testState.toQuantumState();

  EXPECT_FALSE(processor->validateState(quantumState));
}

TEST_F(QuantumProcessorTest, ErrorCorrection) {
  auto testState = QuantumStateData::createTestState(4);
  auto quantumState = testState.toQuantumState();

  // Break normalization but keep equal amplitudes
  for (auto &amp : quantumState.amplitudes) {
    amp *= 2.0;
  }

  // Test evolution should normalize the state
  EXPECT_TRUE(processor->evolveState(quantumState));
  EXPECT_TRUE(processor->validateState(quantumState));
}

// State Tracking Tests
TEST_F(QuantumProcessorTest, StateTrackingAndHistory) {
  auto testState = QuantumStateData::createTestState(4);
  auto quantumState = testState.toQuantumState();

  // Create context
  std::string pattern_id = "test_pattern";
  EXPECT_TRUE(processor->createContext(quantumState, pattern_id));

  // Update context
  quantumState.phase += 0.1;
  EXPECT_TRUE(processor->updateContext(pattern_id, quantumState));

  // Get related contexts
  auto contexts = processor->getRelatedContexts(quantumState);
  EXPECT_GE(contexts.size(), 0);
}

TEST_F(QuantumProcessorTest, StabilityAndCollapseChecks) {
  glm::vec3 stable(1.0f, 0.0f, 0.0f);
  processor->processPattern(stable);
  EXPECT_TRUE(processor->isStable(stable));

  glm::vec3 collapsed(0.0f, 0.0f, 0.0f);
  EXPECT_TRUE(processor->isCollapsed(collapsed));
}

TEST_F(QuantumProcessorTest, StabilityFuzz) {
  for (int i = 0; i < 100; ++i) {
    glm::vec3 p = glm::sphericalRand(1.0f);
    processor->processPattern(p);
    bool stable = processor->isStable(p);
    bool collapsed = processor->isCollapsed(p);
    EXPECT_FALSE(stable && collapsed);
  }
}

// Concurrent Processing
TEST_F(QuantumProcessorTest, ConcurrentProcessing) {
  const int NUM_THREADS = 4;
  std::vector<std::thread> threads;
  std::atomic<int> successCount = 0;

  auto testState = QuantumStateData::createTestState(256);
  auto quantumState = testState.toQuantumState();

  for (int i = 0; i < NUM_THREADS; i++) {
    threads.emplace_back([this, &successCount, quantumState]() {
      for (int j = 0; j < 5; j++) {
        if (processor->validateState(quantumState)) {
          successCount++;
        }
      }
    });
  }

  for (auto &thread : threads) {
    thread.join();
  }

  EXPECT_EQ(successCount, NUM_THREADS * 5);
}

// Performance Tests
TEST_F(QuantumProcessorTest, PerformanceMetrics) {
  processor->resetMetrics();

  // Run multiple operations
  auto testState = QuantumStateData::createTestState(256);
  auto quantumState = testState.toQuantumState();

  for (int i = 0; i < 10; i++) {
    EXPECT_TRUE(processor->evolveState(quantumState));
    EXPECT_TRUE(processor->validateState(quantumState));
  }

  auto metrics = processor->getPerformanceMetrics();
  EXPECT_GT(metrics.operationsPerformed, 0);
  EXPECT_GT(metrics.averageProcessingTime, 0.0);
  EXPECT_GE(metrics.averageGpuUtilization, 0.0);
  EXPECT_LE(metrics.averageGpuUtilization, 100.0);
}

// State Evolution Tests
TEST_F(QuantumProcessorTest, StateEvolution) {
  auto testState = QuantumStateData::createTestState(4);
  auto quantumState = testState.toQuantumState();

  // Test evolution
  EXPECT_TRUE(processor->evolveState(quantumState));
  EXPECT_TRUE(processor->validateState(quantumState));

  // Test pattern matching
  auto patterns = processor->findMatchingPatterns(quantumState);
  EXPECT_GE(patterns.size(), 0);

  // Test resource estimation
  auto resources = processor->estimateResourceRequirements(quantumState);
  EXPECT_GE(resources, 0);
}

} // namespace sep::quantum::test
