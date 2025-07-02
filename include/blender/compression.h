#pragma once

#include <cstddef>
#include <memory>
#include <vector>
#include "compat/shim.h"
#include <vector>

#include "blender/base_types.h"
// Use unified PatternData definitions


namespace blender {

// Compression method enumeration
enum class CompressionMethod { None, DeltaEncoding, LZ4, ZSTD };

// Compression statistics for method selection
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

  // Core compression operations
  virtual std::vector<uint8_t> compress(const void *data, size_t size) = 0;
  virtual bool decompress(const std::vector<uint8_t> &compressed, void *output,
                          size_t outputSize) = 0;

  // Compression method selection
  virtual CompressionMethod selectMethod(const void *data, size_t size) = 0;
  virtual CompressionStats getStats() const = 0;
};

// Delta encoding implementation
class DeltaCompression : public CompressionStrategy {
public:
  std::vector<uint8_t> compress(const void *data, size_t size) override;
  bool decompress(const std::vector<uint8_t> &compressed, void *output,
                  size_t outputSize) override;
  CompressionMethod selectMethod(const void *data, size_t size) override;
  CompressionStats getStats() const override;

private:
  CompressionStats stats;
  std::vector<uint8_t> previousBlock;
};

// LZ4 compression implementation
class LZ4Compression : public CompressionStrategy {
public:
  std::vector<uint8_t> compress(const void *data, size_t size) override;
  bool decompress(const std::vector<uint8_t> &compressed, void *output,
                  size_t outputSize) override;
  CompressionMethod selectMethod(const void *data, size_t size) override;
  CompressionStats getStats() const override;

private:
  CompressionStats stats;
};

// ZSTD compression implementation
class ZSTDCompression : public CompressionStrategy {
public:
  std::vector<uint8_t> compress(const void *data, size_t size) override;
  bool decompress(const std::vector<uint8_t> &compressed, void *output,
                  size_t outputSize) override;
  CompressionMethod selectMethod(const void *data, size_t size) override;
  CompressionStats getStats() const override;

private:
  CompressionStats stats;
};

// Factory for creating compression strategies
class CompressionFactory {
public:
  static std::unique_ptr<CompressionStrategy> create(CompressionMethod method);
  static CompressionMethod analyzeData(const void *data, size_t size);

private:
  static float calculateEntropy(const void *data, size_t size);
  static bool hasRepeatingPatterns(const void *data, size_t size);
};

// Helper functions for compression
namespace compression_utils {
// Calculate entropy of data block
float calculateEntropy(const void *data, size_t size);
// Calculate normalized entropy (0..1)
float calculateNormalizedEntropy(const void *data, size_t size);

// Analyze data patterns
bool hasRepeatingPatterns(const void *data, size_t size);

// Downsample/upsample helpers
std::vector<uint8_t> downsample(const void *data, size_t size,
                                size_t factor = 4);
std::vector<uint8_t> upsample(const std::vector<uint8_t> &data,
                              size_t original_size, size_t factor = 4);

// Estimate compression ratio
float estimateCompressionRatio(const void *data, size_t size,
                               CompressionMethod method);
} // namespace compression_utils

// Pattern-specific compression class
class PatternCompression {
public:
  enum class Mode { FAST, BALANCED, HIGH };
  enum class Flags { DELTA_ENCODED = 1, QUANTIZED = 2, BIT_PACKED = 4 };

  struct Metadata {
    uint32_t pattern_count{0};
    uint32_t flags{0};
    float compression_ratio{1.0f};
  };
  // Forward declare PatternData to avoid circular dependencies
  static bool
  compressPatterns(const std::vector<void*> &patterns,
                   std::vector<uint8_t> &compressed, size_t &compressed_size,
                   Mode mode = Mode::BALANCED);

  static bool
  decompressPatterns(const uint8_t *compressed_data, size_t compressed_size,
                     std::vector<void*> &patterns);
  static bool getMetadata(const uint8_t *compressed_data,
                          size_t compressed_size, Metadata &metadata);

  static bool validate(const uint8_t *compressed_data, size_t compressed_size);
};

} // namespace blender
