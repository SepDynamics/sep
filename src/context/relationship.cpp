#include "context/relationship.h"
#include "cuda/math_common.h"

#include <algorithm>
#include <vector>
#include <string>

namespace sep::context {

float cosineSimilarity(const std::vector<float>& a, const std::vector<float>& b)
{
    if (a.size() != b.size() || a.empty())
        return 0.0f;
    float dot   = 0.0f;
    float norm1 = 0.0f;
    float norm2 = 0.0f;
    for (size_t i = 0; i < a.size(); ++i)
    {
        dot += a[i] * b[i];
        norm1 += a[i] * a[i];
        norm2 += b[i] * b[i];
    }
    if (norm1 == 0.0f || norm2 == 0.0f)
        return 0.0f;
    // Use math utilities to avoid standard library conflicts
    float norm1_sqrt = sep::math::to_float(sep::math::sqrt_safe(static_cast<double>(norm1)));
    float norm2_sqrt = sep::math::to_float(sep::math::sqrt_safe(static_cast<double>(norm2)));
    return dot / (norm1_sqrt * norm2_sqrt);
}

bool simplePatternMatch(const std::string& pattern, const std::string& text)
{
    return text.find(pattern) != std::string::npos;
}

}  // namespace sep::context
