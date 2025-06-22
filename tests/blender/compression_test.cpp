#include "blender/compression.h"

#include <gtest/gtest.h>

#include "compat/shim.h"

// Unified PatternData definitions
#include "quantum/types.h"

using blender::PatternCompression;
using sep::pattern::PatternData;

class CompressionTest : public ::testing::Test
{
protected:
    sep::shim::vector<PatternData> createRandomPatterns(size_t count)
    {
        sep::shim::vector<PatternData> patterns(count);
        for (size_t i = 0; i < count; ++i)
        {
            float v = static_cast<float>(i) / static_cast<float>(count);
            // Use Vec4 constructor instead of glm::vec4
            patterns[i].position   = glm::vec4(v, v, v, 1.0f);
            patterns[i].velocity   = glm::vec4(v, v / 2.0f, v / 3.0f, 0.0f);
            patterns[i].attributes = glm::vec4(v, v * 0.5f, v * 0.25f, v * 0.75f);

            patterns[i].coherence     = v;
            patterns[i].entropy       = v * 0.5f;
            patterns[i].stability     = 1.0f - v;
            patterns[i].mutation_rate = v * 0.1f;

            patterns[i].generation = static_cast<uint32_t>(i);
            patterns[i].mutations  = static_cast<uint32_t>(i % 100);
            patterns[i].promotions = static_cast<uint32_t>(i % 10);
            patterns[i].flags      = static_cast<uint32_t>(i % 16);
        }

        return patterns;
    }

    void verifyPatternData(const PatternData& expected, const PatternData& actual)
    {
        // Spatial data
        EXPECT_FLOAT_EQ(expected.position.x, actual.position.x);
        EXPECT_FLOAT_EQ(expected.position.y, actual.position.y);
        EXPECT_FLOAT_EQ(expected.position.z, actual.position.z);
        EXPECT_FLOAT_EQ(expected.position.w, actual.position.w);

        EXPECT_FLOAT_EQ(expected.velocity.x, actual.velocity.x);
        EXPECT_FLOAT_EQ(expected.velocity.y, actual.velocity.y);
        EXPECT_FLOAT_EQ(expected.velocity.z, actual.velocity.z);
        EXPECT_FLOAT_EQ(expected.velocity.w, actual.velocity.w);

        EXPECT_FLOAT_EQ(expected.attributes.x, actual.attributes.x);
        EXPECT_FLOAT_EQ(expected.attributes.y, actual.attributes.y);
        EXPECT_FLOAT_EQ(expected.attributes.z, actual.attributes.z);
        EXPECT_FLOAT_EQ(expected.attributes.w, actual.attributes.w);

        // Metrics
        EXPECT_FLOAT_EQ(expected.coherence, actual.coherence);
        EXPECT_FLOAT_EQ(expected.entropy, actual.entropy);
        EXPECT_FLOAT_EQ(expected.stability, actual.stability);
        EXPECT_FLOAT_EQ(expected.mutation_rate, actual.mutation_rate);

        // State
        EXPECT_EQ(expected.generation, actual.generation);
        EXPECT_EQ(expected.mutations, actual.mutations);
        EXPECT_EQ(expected.promotions, actual.promotions);
        EXPECT_EQ(expected.flags, actual.flags);
    }
};

TEST_F(CompressionTest, CompressEmptyData)
{
    sep::shim::vector<PatternData> empty;
    sep::shim::vector<uint8_t>     compressed;
    size_t                         compressed_size = 0;
    auto                           result          = PatternCompression::compressPatterns(empty, compressed, compressed_size);
    EXPECT_FALSE(result);
}

TEST_F(CompressionTest, CompressAndDecompressWithDifferentModes)
{
    auto original_patterns = createRandomPatterns(100);

    for (auto mode :
         {PatternCompression::Mode::FAST, PatternCompression::Mode::BALANCED, PatternCompression::Mode::HIGH})
    {
        // Compress
        sep::shim::vector<uint8_t> compressed;
        size_t                     compressed_size = 0;
        auto result = PatternCompression::compressPatterns(original_patterns, compressed, compressed_size, mode);
        EXPECT_TRUE(result);
        EXPECT_GT(compressed_size, 0);

        // Decompress
        sep::shim::vector<PatternData> decompressed_patterns;
        result = PatternCompression::decompressPatterns(compressed.data(), compressed_size, decompressed_patterns);
        EXPECT_TRUE(result);
        EXPECT_EQ(original_patterns.size(), decompressed_patterns.size());

        // Verify data
        for (size_t i = 0; i < original_patterns.size(); ++i)
        {
            verifyPatternData(original_patterns[i], decompressed_patterns[i]);
        }
    }
}

TEST_F(CompressionTest, MetadataHandling)
{
    auto patterns = createRandomPatterns(100);

    // Compress
    sep::shim::vector<uint8_t> compressed;
    size_t                     compressed_size = 0;
    auto                       result = PatternCompression::compressPatterns(patterns, compressed, compressed_size);
    EXPECT_TRUE(result);

    // Get metadata
    PatternCompression::Metadata metadata;
    result = PatternCompression::getMetadata(compressed.data(), compressed_size, metadata);
    EXPECT_TRUE(result);

    // Verify metadata
    EXPECT_EQ(patterns.size(), metadata.pattern_count);
    EXPECT_NE(0u, metadata.flags & static_cast<uint32_t>(PatternCompression::Flags::DELTA_ENCODED));
    EXPECT_NE(0u, metadata.flags & static_cast<uint32_t>(PatternCompression::Flags::QUANTIZED));
    EXPECT_NE(0u, metadata.flags & static_cast<uint32_t>(PatternCompression::Flags::BIT_PACKED));
}

TEST_F(CompressionTest, ValidationChecks)
{
    auto patterns = createRandomPatterns(100);

    // Compress
    sep::shim::vector<uint8_t> compressed;
    size_t                     compressed_size = 0;
    auto                       result = PatternCompression::compressPatterns(patterns, compressed, compressed_size);
    EXPECT_TRUE(result);

    // Valid data
    result = PatternCompression::validate(compressed.data(), compressed_size);
    EXPECT_TRUE(result);

    // Invalid size
    result = PatternCompression::validate(compressed.data(), 10);
    EXPECT_FALSE(result);

    // Null data
    result = PatternCompression::validate(nullptr, compressed_size);
    EXPECT_FALSE(result);
}

TEST_F(CompressionTest, CompressionRatios)
{
    // Create test data with repeating patterns for better compression
    sep::shim::vector<PatternData> patterns(1000);
    for (auto& pattern : patterns)
    {
        pattern.coherence     = 0.5f;
        pattern.entropy       = 0.3f;
        pattern.stability     = 0.8f;
        pattern.mutation_rate = 0.1f;
        pattern.position      = glm::vec4(1.0f, 2.0f, 3.0f, 1.0f);
        pattern.velocity      = glm::vec4(0.1f, 0.2f, 0.3f, 0.0f);
        pattern.attributes    = glm::vec4(0.5f, 0.5f, 0.5f, 0.5f);
        // Initialize other fields to avoid uninitialized variable warnings
        pattern.generation     = 0;
        pattern.mutations      = 0;
        pattern.promotions     = 0;
        pattern.flags          = 0;
        pattern.offset_in_tier = 0;
        pattern.size_in_bytes  = 0;
        pattern.id             = "";
        pattern.tier_id        = 0;
    }

    sep::shim::vector<uint8_t> compressed;
    size_t                     compressed_size = 0;

    // Test different modes
    float last_ratio = 1.0f;
    for (auto mode :
         {PatternCompression::Mode::FAST, PatternCompression::Mode::BALANCED, PatternCompression::Mode::HIGH})
    {
        auto result = PatternCompression::compressPatterns(patterns, compressed, compressed_size, mode);
        EXPECT_TRUE(result);

        float ratio = static_cast<float>(compressed_size) / (patterns.size() * sizeof(PatternData));
        EXPECT_LT(ratio, 0.5f);        // Should achieve at least 2:1 compression
        EXPECT_LE(ratio, last_ratio);  // Higher compression modes should achieve better ratios
        last_ratio = ratio;
    }
}
