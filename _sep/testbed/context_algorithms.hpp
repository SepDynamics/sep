#pragma once
#include <nlohmann/json.hpp>
#include <vector>
#include <cmath>
#include <utility>

namespace sep {
namespace testbed {

inline std::pair<bool,int> validate_contexts_impl(const nlohmann::json& contexts) {
    if (!contexts.is_array()) {
        return {false, -1};
    }
    for (size_t i = 0; i < contexts.size(); ++i) {
        const auto& ctx = contexts[i];
        if (!ctx.is_object()) {
            return {false, static_cast<int>(i)};
        }
        if (!ctx.contains("type") || !ctx["type"].is_string()) {
            return {false, static_cast<int>(i)};
        }
        if (!ctx.contains("content")) {
            return {false, static_cast<int>(i)};
        }
        if (ctx.contains("metadata") && ctx["metadata"].is_object()) {
            if (!ctx["metadata"].contains("timestamp")) {
                return {false, static_cast<int>(i)};
            }
        }
    }
    return {true, -1};
}

inline double cosine_similarity(const std::vector<double>& a, const std::vector<double>& b) {
    double dot = 0.0;
    double norm1 = 0.0;
    double norm2 = 0.0;
    for (size_t i = 0; i < a.size(); ++i) {
        dot += a[i] * b[i];
        norm1 += a[i] * a[i];
        norm2 += b[i] * b[i];
    }
    if (norm1 == 0.0 || norm2 == 0.0) {
        return 0.0;
    }
    return dot / (std::sqrt(norm1) * std::sqrt(norm2));
}

inline nlohmann::json blend_contexts_impl(
    const std::vector<std::vector<double>>& embeddings,
    const std::vector<float>& weights)
{
    nlohmann::json result;
    if (embeddings.empty()) {
        result["embedding"] = std::vector<double>{};
        result["coherence"] = 0.0;
        return result;
    }

    size_t dim = embeddings[0].size();
    std::vector<double> blend(dim, 0.0);

    std::vector<float> w = weights;
    if (w.empty()) {
        w.assign(embeddings.size(), 1.0f / static_cast<float>(embeddings.size()));
    }

    for (size_t i = 0; i < embeddings.size(); ++i) {
        for (size_t j = 0; j < dim; ++j) {
            blend[j] += embeddings[i][j] * w[i];
        }
    }

    double total = 0.0;
    int count = 0;
    for (size_t i = 0; i < embeddings.size(); ++i) {
        for (size_t j = i + 1; j < embeddings.size(); ++j) {
            total += cosine_similarity(embeddings[i], embeddings[j]);
            ++count;
        }
    }
    double coherence = count ? total / count : 1.0;

    result["embedding"] = blend;
    result["coherence"] = coherence;
    return result;
}

} // namespace testbed
} // namespace sep

