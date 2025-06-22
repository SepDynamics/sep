/**
 * Unit tests for quantum processing functionality
 */

#include "core/quantum_processor.h"
#include "cuda/kernel_interface.h"

#include <gtest/gtest.h>
#include <memory>
#include <vector>

namespace sep::test {

// Mock quantum state for testing
struct QuantumStateData {
    std::vector<float> amplitudes;
    std::vector<uint64_t> bitStates;
    std::vector<float> errorRates;

    static QuantumStateData createTestState(int numBits, int numStates) {
        QuantumStateData data;
        data.amplitudes.resize(numStates);
        data.bitStates.resize(numStates);
        data.errorRates.resize(numBits);

        // Initialize with test values
        for (int i = 0; i < numStates; i++) {
            data.amplitudes[i] = 1.0f / std::sqrt(static_cast<float>(numStates));  // Uniform distribution
            data.bitStates[i] = i;                             // Simple sequential bit patterns
        }

        for (int i = 0; i < numBits; i++) {
            data.errorRates[i] = 0.01f * i;  // Increasing error rates
        }

        return data;
    }
};

// Test fixture for the quantum processor
class QuantumProcessorTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Initialize processor with test options
        quantum::ProcessorOptions options;
        options.useGPU = true;
        options.symmetryPairs = 32;
        options.errorDetectionEnabled = true;
        options.batchSize = 64;

        processor = std::make_unique<quantum::Processor>(options);

        // Create test quantum state
        testState = QuantumStateData::createTestState(10, 64);
    }

    void TearDown() override {
        processor.reset();
    }

    std::unique_ptr<quantum::Processor> processor;
    QuantumStateData testState;
};

// Test initialization and options
TEST_F(QuantumProcessorTest, InitializationAndOptions) {
    EXPECT_TRUE(processor->getOptions().useGPU);
    EXPECT_EQ(processor->getOptions().symmetryPairs, 32);
    EXPECT_TRUE(processor->getOptions().errorDetectionEnabled);
    EXPECT_EQ(processor->getOptions().batchSize, 64);

    // Update options and verify
    quantum::ProcessorOptions newOptions;
    newOptions.useGPU = false;
    newOptions.symmetryPairs = 16;
    newOptions.errorDetectionEnabled = false;
    newOptions.batchSize = 32;

    processor->setOptions(newOptions);

    EXPECT_FALSE(processor->getOptions().useGPU);
    EXPECT_EQ(processor->getOptions().symmetryPairs, 16);
    EXPECT_FALSE(processor->getOptions().errorDetectionEnabled);
    EXPECT_EQ(processor->getOptions().batchSize, 32);
}

// Test quantum bit state analysis (QBSA)
TEST_F(QuantumProcessorTest, QuantumBitStateAnalysis) {
    // Perform QBSA on the test state
    auto result = processor->analyzeQuantumBitState(testState.bitStates, testState.amplitudes);

    // Verify the results
    EXPECT_TRUE(result.success);
    EXPECT_EQ(result.stateCount, testState.bitStates.size());
    EXPECT_GT(result.stateEntropy, 0.0f);
    EXPECT_LE(result.stateEntropy, std::log2(testState.bitStates.size()));
    EXPECT_EQ(result.symmetryViolations.size(), 0);
}

// Test quantum state history (QSH) tracking
TEST_F(QuantumProcessorTest, QuantumStateHistoryTracking) {
    // Start tracking the quantum state
    auto trackingId = processor->startStateTracking(testState.bitStates, testState.amplitudes);
    EXPECT_NE(trackingId, 0);

    // Modify the state (simulating a quantum operation)
    std::vector<float> newAmplitudes = testState.amplitudes;
    for (auto& amp : newAmplitudes) {
        amp *= 0.9f;  // Reduce amplitude slightly
    }

    // Update the tracked state
    auto updateResult =
        processor->updateTrackedState(trackingId, testState.bitStates, newAmplitudes);
    EXPECT_TRUE(updateResult.success);

    // Get the state history
    auto history = processor->getStateHistory(trackingId);
    EXPECT_EQ(history.stateChanges.size(), 1);
    EXPECT_GT(history.stabilityScore, 0.8f);
    EXPECT_GT(history.collapseEvents.size(), 0);
}

// Test error detection in quantum processing
TEST_F(QuantumProcessorTest, ErrorDetection) {
    // Create a state with a simulated error
    auto errorState = testState;
    errorState.amplitudes[0] = 2.0f;  // Invalid amplitude (not normalized)

    // Run error detection
    auto result = processor->detectErrors(errorState.bitStates, errorState.amplitudes);

    // Verify errors were detected
    EXPECT_TRUE(result.errorsDetected);
    EXPECT_GT(result.errorCount, 0);
    EXPECT_FALSE(result.correctionApplied);

    // Test error correction
    auto correctedResult = processor->correctErrors(errorState.bitStates, errorState.amplitudes);

    // Verify correction worked
    EXPECT_TRUE(correctedResult.success);
    EXPECT_TRUE(correctedResult.correctionApplied);

    // Verify the corrected state is valid
    auto validationResult =
        processor->validateQuantumState(correctedResult.bitStates, correctedResult.amplitudes);
    EXPECT_TRUE(validationResult.valid);
}

// Test bit-space operations
TEST_F(QuantumProcessorTest, BitSpaceOperations) {
    // Test symmetry pair validation
    auto symmetryResult = processor->validateSymmetryPairs(testState.bitStates);
    EXPECT_TRUE(symmetryResult.valid);
    EXPECT_EQ(symmetryResult.validPairs, processor->getOptions().symmetryPairs);

    // Test bit-space transformation
    auto transformedState =
        processor->transformBitSpace(testState.bitStates, quantum::BitTransformation::ROTATE);
    EXPECT_EQ(transformedState.bitStates.size(), testState.bitStates.size());
    EXPECT_NE(transformedState.bitStates[0], testState.bitStates[0]);

    // Verify the transformation is reversible
    auto reversedState = processor->transformBitSpace(transformedState.bitStates,
                                                      quantum::BitTransformation::INVERSE_ROTATE);
    EXPECT_EQ(reversedState.bitStates, testState.bitStates);
}

// Test batch processing of quantum states
TEST_F(QuantumProcessorTest, BatchProcessing) {
    // Create multiple test states
    std::vector<std::vector<uint64_t>> batchBitStates;
    std::vector<std::vector<float>> batchAmplitudes;

    for (int i = 0; i < 5; i++) {
        auto state = QuantumStateData::createTestState(10, 64);
        batchBitStates.push_back(state.bitStates);
        batchAmplitudes.push_back(state.amplitudes);
    }

    // Process the batch
    auto results = processor->processBatch(batchBitStates, batchAmplitudes);

    // Verify results
    EXPECT_EQ(results.size(), 5);
    for (const auto& result : results) {
        EXPECT_TRUE(result.success);
        EXPECT_GT(result.stabilityScore, 0.0f);
        EXPECT_LE(result.stabilityScore, 1.0f);
    }
}

// Test performance metrics for quantum processing
TEST_F(QuantumProcessorTest, PerformanceMetrics) {
    // Reset metrics
    processor->resetMetrics();

    // Perform operations to generate metrics
    for (int i = 0; i < 10; i++) {
        processor->analyzeQuantumBitState(testState.bitStates, testState.amplitudes);
    }

    // Get metrics
    auto metrics = processor->getPerformanceMetrics();

    // Verify metrics were recorded
    EXPECT_EQ(metrics.operationsPerformed, 10);
    EXPECT_GT(metrics.averageProcessingTime, 0.0);
    EXPECT_GT(metrics.averageGpuUtilization, 0.0);
    EXPECT_LT(metrics.averageGpuUtilization, 100.0);
}

}  // namespace sep::test
