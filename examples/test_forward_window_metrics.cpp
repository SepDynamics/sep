#include <gtest/gtest.h>
#include "apps/oanda_trader/forward_window_kernels.cuh"

using namespace sep::apps::cuda;

// Test helper: Feed bitstream and get a ForwardWindowResult
ForwardWindowResult testForwardWindow(const std::vector<uint8_t>& bits, size_t index_start = 0) {
    // Stub: In real test, call your kernel or CPU fallback
    ForwardWindowResult result;
    return result;
}

TEST(ForwardWindowTest, AllFlipPattern) {
    std::vector<uint8_t> bitstream = {0,1,0,1,0,1,0,1,0,1};
    auto result = testForwardWindow(bitstream);

    EXPECT_GT(result.coherence, 0.85f);         // High coherence
    EXPECT_GT(result.stability, 0.9f);          // Stable oscillation
    EXPECT_GT(result.entropy, 0.6f);            // Moderate entropy
    EXPECT_EQ(result.rupture_count, 0);         // No 1→1 transitions
    EXPECT_EQ(result.flip_count, 9);            // All transitions are flips
}

TEST(ForwardWindowTest, AllRupturePattern) {
    std::vector<uint8_t> bitstream = {1,1,1,1,1,1,1,1,1,1};
    auto result = testForwardWindow(bitstream);

    EXPECT_LT(result.coherence, 0.2f);          // Low coherence
    EXPECT_LT(result.stability, 0.2f);          // Low stability
    EXPECT_LT(result.entropy, 0.2f);            // Very ordered (low entropy)
    EXPECT_EQ(result.rupture_count, 9);         // All 1→1 transitions
    EXPECT_EQ(result.flip_count, 0);            // No flips
}

TEST(ForwardWindowTest, AlternatingBlockPattern) {
    std::vector<uint8_t> bitstream = {0,0,1,1,0,0,1,1};
    auto result = testForwardWindow(bitstream);

    EXPECT_GT(result.coherence, 0.5f);          
    EXPECT_GT(result.stability, 0.4f);          
    EXPECT_GT(result.entropy, 0.4f);            
    EXPECT_EQ(result.rupture_count, 2);         
    EXPECT_EQ(result.flip_count, 4);            
}

TEST(ForwardWindowTest, RandomNoisePattern) {
    std::vector<uint8_t> bitstream = {0,1,1,0,1,0,0,1,1,0};
    auto result = testForwardWindow(bitstream);

    EXPECT_GT(result.entropy, 0.85f);           // High entropy
    EXPECT_GT(result.flip_count, 3);            // Should contain flips
    EXPECT_GT(result.rupture_count, 1);         // Likely some rupture
    EXPECT_LT(result.coherence, 0.6f);          
    EXPECT_LT(result.stability, 0.5f);          
}

TEST(ForwardWindowTest, NullStatePattern) {
    std::vector<uint8_t> bitstream = {0,0,0,0,0,0,0};
    auto result = testForwardWindow(bitstream);

    EXPECT_NEAR(result.entropy, 0.0f, 1e-3);     // No entropy
    EXPECT_NEAR(result.stability, 1.0f, 1e-2);   // Very stable
    EXPECT_GT(result.coherence, 0.9f);           // Extremely coherent
    EXPECT_EQ(result.rupture_count, 0);
    EXPECT_EQ(result.flip_count, 0);
}
