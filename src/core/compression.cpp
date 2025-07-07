#include <cstddef>
#include <cstdint>
#include <vector>
#include <memory>
#include <algorithm>
#include <stdexcept>
#include "core/compression.h"
#include <algorithm>
#include <cstring>
#include <stdexcept>

namespace sep {
namespace core {

// Default compression implementation using simple RLE
class DefaultCompressor : public Compressor {
public:
    std::vector<uint8_t> compress(const void* data, size_t size) override {
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

    bool decompress(const std::vector<uint8_t>& compressed, void* output, size_t outputSize) override {
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
};

// Factory method implementation
std::unique_ptr<Compressor> createCompressor(CompressionType type) {
    switch (type) {
        case CompressionType::Default:
            return std::make_unique<DefaultCompressor>();
        default:
            throw std::invalid_argument("Unsupported compression type");
    }
}

// Utility functions implementation
std::vector<uint8_t> downsample(const void* data, size_t size, size_t factor) {
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

std::vector<uint8_t> upsample(const std::vector<uint8_t>& data, size_t original_size, size_t factor) {
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