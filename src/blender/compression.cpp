#include <cstring>
#include <ctime>
#include <cmath>
#include <vector>
#include <string>
#include <memory>

#include <string.h>
#include <time.h>
#include <unistd.h>
#include <cstdlib>

#include <algorithm>
#include <chrono>
#include <array>

#include <lz4.h>
#include <zstd.h>

#include "blender/compression.h"

namespace blender {

// ----------------------------- DeltaCompression -----------------------------
std::vector<uint8_t> DeltaCompression::compress(const void* data, size_t size) {
  const uint8_t* bytes = static_cast<const uint8_t*>(data);
  std::vector<uint8_t> out(size);
  auto start = std::chrono::high_resolution_clock::now(); // Explicitly qualify std::chrono

  if (previousBlock.empty()) {
    out.assign(bytes, bytes + size);
  } else {
    out.resize(size); // Resize to current block size
    for (size_t i = 0; i < size; ++i) {
      uint8_t prev = previousBlock[i % previousBlock.size()];
      out[i] = static_cast<uint8_t>(bytes[i] - prev);
    }
  }

  auto end = std::chrono::high_resolution_clock::now(); // Explicitly qualify std::chrono
  stats.originalSize = size;
  stats.compressedSize = out.size();
  stats.compressionRatio = static_cast<float>(out.size()) / static_cast<float>(size);
  stats.compressionTime = std::chrono::duration<float, std::milli>(end - start).count();
  previousBlock.assign(bytes, bytes + size);
  return out;
}

bool DeltaCompression::decompress(const std::vector<uint8_t>& compressed, void* output,
                                  size_t outputSize) {
  if (outputSize != compressed.size()) return false;
  uint8_t* out = static_cast<uint8_t*>(output);
  auto start = std::chrono::high_resolution_clock::now(); // Explicitly qualify std::chrono

  if (previousBlock.empty()) {
    std::memcpy(out, compressed.data(), outputSize);
  } else {
    for (size_t i = 0; i < outputSize; ++i) {
      uint8_t prev = previousBlock[i % previousBlock.size()];
      out[i] = static_cast<uint8_t>(compressed[i] + prev);
    }
  }

  auto end = std::chrono::high_resolution_clock::now(); // Explicitly qualify std::chrono
  stats.decompressionTime = std::chrono::duration<float, std::milli>(end - start).count();
  previousBlock.assign(out, out + outputSize);
  return true;
}

CompressionMethod DeltaCompression::selectMethod(const void* /*data*/, size_t /*size*/) {
  return CompressionMethod::DeltaEncoding;
}

CompressionStats DeltaCompression::getStats() const { return stats; }

// ----------------------------- LZ4Compression ------------------------------
std::vector<uint8_t> LZ4Compression::compress(const void* data, size_t size) { 
  int maxSize = LZ4_compressBound(static_cast<int>(size));
  std::vector<uint8_t> out(maxSize);
  auto start = std::chrono::high_resolution_clock::now(); // Explicitly qualify std::chrono

  int compressedSize =
      LZ4_compress_default(reinterpret_cast<const char*>(data), reinterpret_cast<char*>(out.data()),
                           static_cast<int>(size), maxSize);
  auto end = std::chrono::high_resolution_clock::now(); // Explicitly qualify std::chrono
  if (compressedSize <= 0) {
    return {};
  }
  out.resize(static_cast<size_t>(compressedSize));

  stats.originalSize = size;
  stats.compressedSize = out.size();
  stats.compressionRatio = static_cast<float>(out.size()) / static_cast<float>(size);
  stats.compressionTime = std::chrono::duration<float, std::milli>(end - start).count();
  return out;
}

bool LZ4Compression::decompress(const std::vector<uint8_t>& compressed, void* output,
                                size_t outputSize) {
  auto start = std::chrono::high_resolution_clock::now(); // Explicitly qualify std::chrono
  int result = LZ4_decompress_safe(
      reinterpret_cast<const char*>(compressed.data()), reinterpret_cast<char*>(output),
      static_cast<int>(compressed.size()), static_cast<int>(outputSize));
  auto end = std::chrono::high_resolution_clock::now(); // Explicitly qualify std::chrono
  stats.decompressionTime = std::chrono::duration<float, std::milli>(end - start).count();
  return result >= 0;
}

CompressionMethod LZ4Compression::selectMethod(const void* /*data*/, size_t /*size*/) {
  return CompressionMethod::LZ4;
}

CompressionStats LZ4Compression::getStats() const { return stats; }

// ----------------------------- ZSTDCompression -----------------------------
std::vector<uint8_t> ZSTDCompression::compress(const void* data, size_t size) {
  size_t maxSize = ZSTD_compressBound(size);
  std::vector<uint8_t> out(maxSize);
  auto start = std::chrono::high_resolution_clock::now(); // Explicitly qualify std::chrono
  size_t compressedSize = ZSTD_compress(out.data(), maxSize, data, size, 1);
  auto end = std::chrono::high_resolution_clock::now(); // Explicitly qualify std::chrono
  if (ZSTD_isError(compressedSize)) {
    return {};
  }
  out.resize(compressedSize);
  stats.originalSize = size;
  stats.compressedSize = out.size();
  stats.compressionRatio = static_cast<float>(out.size()) / static_cast<float>(size);
  stats.compressionTime = std::chrono::duration<float, std::milli>(end - start).count();
  return out;
}

bool ZSTDCompression::decompress(const std::vector<uint8_t>& compressed, void* output,
                                 size_t outputSize) {
  auto start = std::chrono::high_resolution_clock::now(); // Explicitly qualify std::chrono
  size_t result = ZSTD_decompress(output, outputSize, compressed.data(), compressed.size());
  auto end = std::chrono::high_resolution_clock::now(); // Explicitly qualify std::chrono
  stats.decompressionTime = std::chrono::duration<float, std::milli>(end - start).count();
  return !ZSTD_isError(result);
}

CompressionMethod ZSTDCompression::selectMethod(const void* /*data*/, size_t /*size*/) {
  return CompressionMethod::ZSTD;
}

CompressionStats ZSTDCompression::getStats() const { return stats; }

// ----------------------------- CompressionFactory --------------------------
std::unique_ptr<CompressionStrategy> CompressionFactory::create(CompressionMethod method) {
  switch (method) {
    case CompressionMethod::DeltaEncoding:
      return std::make_unique<DeltaCompression>();
    case CompressionMethod::LZ4:
      return std::make_unique<LZ4Compression>();
    case CompressionMethod::ZSTD:
      return std::make_unique<ZSTDCompression>();
    default:
      return nullptr;
  }
}

CompressionMethod CompressionFactory::analyzeData(const void* data, size_t size) {
  auto downsampled = compression_utils::downsample(data, size, 4);
  float nEntropy =
      compression_utils::calculateNormalizedEntropy(downsampled.data(), downsampled.size());
  if (nEntropy < 0.3f) {
    return CompressionMethod::DeltaEncoding;
  } else if (nEntropy < 0.7f) {
    return CompressionMethod::LZ4;
  }
  return CompressionMethod::ZSTD;
}

float CompressionFactory::calculateEntropy(const void* data, size_t size) {
  return compression_utils::calculateEntropy(data, size);
}

bool CompressionFactory::hasRepeatingPatterns(const void* data, size_t size) {
  return compression_utils::hasRepeatingPatterns(data, size);
}

// ----------------------------- compression_utils ---------------------------
// Entropy calculation functions are defined in compression_utils.cpp and linked.
// These are declared in the header but implemented in the other file.
// No implementation needed here.

namespace compression_utils {

std::vector<uint8_t> downsample(const void* data, size_t size, size_t factor) {
  const uint8_t* bytes = static_cast<const uint8_t*>(data);
  size_t out_size = (size + factor - 1) / factor;
  std::vector<uint8_t> result(out_size);
  if (out_size > 0) {
    std::memset(result.data(), 0, out_size);
  }
  for (size_t i = 0; i < out_size; ++i) {
    size_t begin = i * factor;
    size_t end = std::min(begin + factor, size);
    int sum = 0;
    for (size_t j = begin; j < end; ++j) sum += bytes[j];
    result[i] = static_cast<uint8_t>(sum / (end - begin));
  }
  return result;
}

std::vector<uint8_t> upsample(const std::vector<uint8_t>& data, size_t original_size,
                              size_t factor) {
  std::vector<uint8_t> result(original_size);
  for (size_t i = 0; i < original_size; ++i) {
    result[i] = data[i / factor];
  }
  return result;
}

bool hasRepeatingPatterns(const void* data, size_t size) {
  if (size < 8) return false;
  const uint8_t* bytes = static_cast<const uint8_t*>(data);
  for (size_t i = 0; i + 4 < size; ++i) {
    if (std::equal(bytes, bytes + 4, bytes + i)) return true;
  }
  return false;
}

float estimateCompressionRatio(const void* data, size_t size, CompressionMethod method) {
  if (!data || size == 0) return 1.0f;
  switch (method) {
    case CompressionMethod::LZ4: {
      int maxSize = LZ4_compressBound(static_cast<int>(size));
      std::vector<uint8_t> buf(maxSize);
      int csize = LZ4_compress_default(reinterpret_cast<const char*>(data),
                                       reinterpret_cast<char*>(buf.data()), static_cast<int>(size),
                                       maxSize);
      if (csize <= 0) return 1.0f;
      return static_cast<float>(csize) / static_cast<float>(size);
    }
    case CompressionMethod::ZSTD: {
      size_t maxSize = ZSTD_compressBound(size);
      std::vector<uint8_t> buf(maxSize);
      size_t csize = ZSTD_compress(buf.data(), maxSize, data, size, 1);
      if (ZSTD_isError(csize)) return 1.0f;
      return static_cast<float>(csize) / static_cast<float>(size);
    }
    default:
      return 1.0f;
  }
}

}  // namespace compression_utils

}  // namespace blender
