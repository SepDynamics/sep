#include "compression.h"

#include "engine/internal/standard_includes.h"
#include <algorithm>
#include <cmath>
#include <unordered_map>

namespace sep {
namespace core {

// Default compression implementation using simple RLE
class DefaultCompressor : public CompressionStrategy {
public:
    std::vector<uint8_t> compress(const void* data, size_t size) override
    {
        const uint8_t* bytes = static_cast<const uint8_t*>(data);
        std::vector<uint8_t> compressed;
        compressed.reserve(size);

        for (size_t i = 0; i < size; i++) {
            uint8_t count = 1;
            uint8_t current = bytes[i];

            while (i + 1 < size && bytes[i + 1] == current && count < 255) {
                count++;
                i++;
            }

            compressed.push_back(count);
            compressed.push_back(current);
        }

        return compressed;
    }

    bool decompress(const std::vector<uint8_t>& compressed, void* output,
                    size_t outputSize) override
    {
        uint8_t* out = static_cast<uint8_t*>(output);
        size_t outPos = 0;
        
        for (size_t i = 0; i < compressed.size(); i += 2) {
            if (i + 1 >= compressed.size()) return false;
            
            uint8_t count = compressed[i];
            uint8_t value = compressed[i + 1];
            
            if (outPos + count > outputSize) return false;
            
            std::fill_n(out + outPos, count, value);
            outPos += count;
        }
        
        return outPos == outputSize;
    }

    sep::memory::CompressionMethod getMethod() const override
    {
        return sep::memory::CompressionMethod::None;
    }

    CompressionStats getStats() const override {
        return stats_;
    }

private:
    CompressionStats stats_{};
};

// Factory method implementation
std::unique_ptr<CompressionStrategy> CompressionFactory::create(
    sep::memory::CompressionMethod method)
{
    switch (method) {
        case sep::memory::CompressionMethod::ZSTD:
        case sep::memory::CompressionMethod::None:
        default:
            // Only a simple RLE compressor is implemented in this minimal build
            return std::make_unique<DefaultCompressor>();
    }
}

sep::memory::CompressionMethod CompressionFactory::analyzeData(const void* /*data*/,
                                                               size_t /*size*/)
{
    // Minimal heuristic: always return None
    return sep::memory::CompressionMethod::None;
}

float CompressionFactory::estimateCompressionRatio(const void* data, size_t size,
                                                   sep::memory::CompressionMethod method)
{
    if (!data || size == 0) {
        return 0.0f;
    }
    
    // Simple entropy-based compression estimate
    const uint8_t* bytes = static_cast<const uint8_t*>(data);
    std::unordered_map<uint8_t, size_t> byte_counts;
    
    // Count byte frequencies
    for (size_t i = 0; i < size; ++i) {
        byte_counts[bytes[i]]++;
    }
    
    // Calculate Shannon entropy
    double entropy = 0.0;
    
    for (const auto& pair : byte_counts) {
        double frequency = static_cast<double>(pair.second) / size;
        entropy -= frequency * std::log2(frequency);
    }
    
    // Normalize entropy (0 = perfectly compressible, 1 = random)
    double normalized_entropy = entropy / 8.0; // 8 bits per byte
    
    // Estimate compression ratio based on entropy
    // Lower entropy = better compression
    double compression_ratio = 0.1 + (normalized_entropy * 0.9);
    
    return static_cast<float>(std::clamp(compression_ratio, 0.1, 1.0));
}

// Utility functions implementation
std::vector<uint8_t> downsample(const void* data, size_t size, size_t factor)
{
    if (factor < 1) throw std::invalid_argument("Downsample factor must be >= 1");
    if (factor == 1) {
        const uint8_t* bytes = static_cast<const uint8_t*>(data);
        return std::vector<uint8_t>(bytes, bytes + size);
    }

    std::vector<uint8_t> result;
    result.reserve(size / factor + 1);
    
    const uint8_t* bytes = static_cast<const uint8_t*>(data);
    for (size_t i = 0; i < size; i += factor) {
        // Average the values in the window
        uint32_t sum = 0;
        size_t count = 0;
        for (size_t j = 0; j < factor && (i + j) < size; j++) {
            sum += bytes[i + j];
            count++;
        }
        result.push_back(static_cast<uint8_t>(sum / count));
    }
    
    return result;
}

std::vector<uint8_t> upsample(const std::vector<uint8_t>& data, size_t original_size, size_t factor)
{
    if (factor < 1) throw std::invalid_argument("Upsample factor must be >= 1");
    if (factor == 1) return data;

    std::vector<uint8_t> result;
    result.reserve(original_size);

    // Linear interpolation between points
    for (size_t i = 0; i < data.size() - 1; i++) {
        uint8_t start = data[i];
        uint8_t end = data[i + 1];
        
        for (size_t j = 0; j < factor && result.size() < original_size; j++) {
            float t = static_cast<float>(j) / factor;
            uint8_t interpolated = static_cast<uint8_t>(
                start * (1.0f - t) + end * t
            );
            result.push_back(interpolated);
        }
    }

    // Handle last point if needed
    while (result.size() < original_size) {
        result.push_back(data.back());
    }

    return result;
}

} // namespace core
} // namespace sep