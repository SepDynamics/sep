#pragma once

#include "engine/internal/standard_includes.h"
#include "memory/types.h"
#include "types.h"

namespace sep::core {

// Compression statistics
struct CompressionStats {
    size_t originalSize{0};
    size_t compressedSize{0};
    float compressionRatio{1.0f};
    float compressionTime{0.0f};
    float decompressionTime{0.0f};
};

// Base compression strategy interface
class CompressionStrategy {
public:
    virtual ~CompressionStrategy() = default;

    virtual std::vector<uint8_t> compress(const void* data, size_t size) = 0;
    virtual bool decompress(const std::vector<uint8_t>& compressed, void* output,
                            size_t outputSize) = 0;
    virtual sep::memory::CompressionMethod getMethod() const = 0;
    virtual CompressionStats getStats() const = 0;
};

// Factory for creating compression strategies
class CompressionFactory {
public:
    static std::unique_ptr<CompressionStrategy> create(sep::memory::CompressionMethod method);
    static sep::memory::CompressionMethod analyzeData(const void* data, size_t size);
    static float estimateCompressionRatio(const void* data, size_t size,
                                          sep::memory::CompressionMethod method);
};

// Compression utility functions
namespace compression_utils {
    float calculateEntropy(const void* data, size_t size);
    float calculateNormalizedEntropy(const void* data, size_t size);
    bool hasRepeatingPatterns(const void* data, size_t size);
    std::vector<uint8_t> downsample(const void* data, size_t size, size_t factor = 4);
    std::vector<uint8_t> upsample(const std::vector<uint8_t>& data, size_t original_size,
                                  size_t factor = 4);
} // namespace compression_utils

} // namespace sep::core