#include "blender_pch.h"
#include <unistd.h>


#include "blender/compression.h"
#include "compat/math_common.h"



namespace blender {
namespace compression_utils {

float calculateEntropy(const void* data, size_t size) {
    if (!data || size == 0) return 0.0f;
    const uint8_t* bytes = static_cast<const uint8_t*>(data);
    std::array<size_t, 256> counts{};
    for (size_t i = 0; i < size; ++i) {
        counts[bytes[i]]++;
    }
    float entropy = 0.0f;
    for (size_t c : counts) {
        if (c == 0) continue;
        float p = static_cast<float>(c) / static_cast<float>(size);
        entropy -= p * std::log2(p);
    }
    return entropy;
}

float calculateNormalizedEntropy(const void* data, size_t size) {
    float ent = calculateEntropy(data, size);
    return size == 0 ? 0.0f : ent / 8.0f;
}

} // namespace compression_utils
} // namespace blender

