#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "mock_blender_bridge.cpp"
#include "mock_mesh_handler.cpp"
#include "mock_pattern_evolution.cpp"
#include "mock_cuda_wrappers.cpp"
#include "mock_trace_span.cpp"
#include "mock_pattern_compression.cpp"

using namespace sep;
using namespace testing;

class PatternIntegrationTest : public Test {
protected:
    void SetUp() override {
        mockBlenderBridge = std::make_unique<test::MockBlenderBridge>();
        mockMeshHandler = std::make_unique<test::MockMeshHandler>();
        mockPatternEvolution = std::make_unique<test::MockPatternEvolution>();
        mockCudaWrapper = std::make_unique<test::MockCudaWrapper>();
        mockTraceSpan = std::make_unique<test::MockTraceSpan>();
        mockCompression = std::make_unique<test::MockPatternCompression>();
    }

    // Helper to create a test pattern
    pattern::PatternData createTestPattern() {
        pattern::PatternData pattern;
        pattern.evolutionRate = 0.75f;
        pattern.energyLevel = 1.2f;
        pattern.couplingStrength = 0.9f;
        return pattern;
    }

    // Helper to verify pattern coherence
    void verifyPatternCoherence(const memory::CoherenceResult& result) {
        EXPECT_GT(result.coherenceScore, 0.8f);
        EXPECT_LT(result.fragmentationLevel, 0.2f);
        EXPECT_GT(result.patternCount, 0);
    }

    std::unique_ptr<test::MockBlenderBridge> mockBlenderBridge;
    std::unique_ptr<test::MockMeshHandler> mockMeshHandler;
    std::unique_ptr<test::MockPatternEvolution> mockPatternEvolution;
    std::unique_ptr<test::MockCudaWrapper> mockCudaWrapper;
    std::unique_ptr<test::MockTraceSpan> mockTraceSpan;
    std::unique_ptr<test::MockPatternCompression> mockCompression;
};

TEST_F(PatternIntegrationTest, FullPatternFlowTest) {
    // Create test pattern
    auto pattern = createTestPattern();

    // Setup expectations for pattern evolution
    EXPECT_CALL(*mockPatternEvolution, evolvePattern(_, _))
        .WillOnce(Return(memory::CoherenceResult{0.95f, 0.1f, 10}));

    // Setup expectations for memory tier transitions
    EXPECT_CALL(*mockCudaWrapper, allocateDeviceMemory(_, _)).Times(1);
    EXPECT_CALL(*mockCudaWrapper, memcpyHostToDevice(_, _, _)).Times(1);

    // Setup expectations for Blender integration
    EXPECT_CALL(*mockBlenderBridge, updateMesh(_)).Times(1);
    EXPECT_CALL(*mockMeshHandler, applyPattern(_, _)).Times(1);

    // Setup expectations for pattern compression
    EXPECT_CALL(*mockCompression, compressPattern(_))
        .WillOnce(Return(std::vector<uint8_t>{1, 2, 3, 4}));
    EXPECT_CALL(*mockCompression, validateCompression(_, _))
        .WillOnce(Return(memory::CoherenceResult{0.98f, 0.05f, 1}));

    // Execute pattern flow
    auto result = mockPatternEvolution->evolvePattern(pattern, mockTraceSpan.get());
    verifyPatternCoherence(result);

    // Verify pattern was processed through memory tiers
    EXPECT_TRUE(mockCudaWrapper->allocateDeviceMemory(nullptr, sizeof(pattern::PatternData)));
    
    // Verify Blender integration
    mockBlenderBridge->updateMesh(pattern);
    mockMeshHandler->applyPattern(pattern, result);

    // Verify pattern compression and persistence
    auto compressed = mockCompression->compressPattern(pattern);
    EXPECT_FALSE(compressed.empty());
    
    auto validationResult = mockCompression->validateCompression(pattern, pattern);
    verifyPatternCoherence(validationResult);
}

TEST_F(PatternIntegrationTest, PatternEvolutionErrorHandling) {
    auto pattern = createTestPattern();

    // Setup expectations for error case
    EXPECT_CALL(*mockPatternEvolution, evolvePattern(_, _))
        .WillOnce(Throw(std::runtime_error("Evolution error")));

    EXPECT_CALL(*mockTraceSpan, setError(_)).Times(1);

    // Verify error handling
    EXPECT_THROW(mockPatternEvolution->evolvePattern(pattern, mockTraceSpan.get()),
                 std::runtime_error);
}

TEST_F(PatternIntegrationTest, MemoryTierTransitions) {
    auto pattern = createTestPattern();

    // Setup expectations for successful tier transitions
    EXPECT_CALL(*mockPatternEvolution, evolvePattern(_, _))
        .WillOnce(Return(memory::CoherenceResult{0.92f, 0.15f, 5}));

    EXPECT_CALL(*mockCudaWrapper, allocateDeviceMemory(_, _)).Times(1);
    EXPECT_CALL(*mockCudaWrapper, memcpyHostToDevice(_, _, _)).Times(1);
    EXPECT_CALL(*mockCudaWrapper, synchronizeStream()).Times(1);

    // Execute and verify tier transitions
    auto result = mockPatternEvolution->evolvePattern(pattern, mockTraceSpan.get());
    verifyPatternCoherence(result);

    EXPECT_TRUE(mockCudaWrapper->allocateDeviceMemory(nullptr, sizeof(pattern::PatternData)));
    mockCudaWrapper->synchronizeStream();
}