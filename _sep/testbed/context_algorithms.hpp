#pragma once

#ifndef SEP_TESTBED_CONTEXT_ALGORITHMS_HPP
#define SEP_TESTBED_CONTEXT_ALGORITHMS_HPP

#include <nlohmann/json.hpp>
#include <vector>
#include <cmath>
#include <utility>
#include <string>

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
            if (!ctx["metadata"].contains("timestamp") ||
                !ctx["metadata"]["timestamp"].is_number()) {
                return {false, static_cast<int>(i)};
            }
        } else {
            return {false, static_cast<int>(i)};
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

struct ValidationReport {
    bool overall_valid{true};
    std::vector<size_t> invalid_indices;
};

inline ValidationReport validate_contexts(const nlohmann::json& contexts) {
    ValidationReport report{};
    if (!contexts.is_array()) {
        report.overall_valid = false;
        return report;
    }
    size_t index = 0;
    for (const auto& ctx : contexts) {
        bool ok = ctx.is_object() && ctx.contains("type") && ctx["type"].is_string() &&
                  ctx.contains("metadata") && ctx["metadata"].is_object() &&
                  ctx["metadata"].contains("timestamp") &&
                  ctx["metadata"]["timestamp"].is_number();
        if (!ok) {
            report.overall_valid = false;
            report.invalid_indices.push_back(index);
        }
        ++index;
    }
    return report;
}

struct BlendReport {
    bool success{false};
    std::vector<double> blended;
    double coherence{0.0};
    std::string error;
};

inline BlendReport blend_embeddings(const std::vector<std::vector<double>>& embeddings,
                                   const std::vector<double>& weights) {
    BlendReport report{};
    if (embeddings.empty()) {
        report.error = "No contexts provided";
        return report;
    }
    size_t dim = embeddings[0].size();
    for (const auto& e : embeddings) {
        if (e.size() != dim) {
            report.error = "Inconsistent embedding dimensions";
            return report;
        }
    }
    if (!weights.empty() && weights.size() != embeddings.size()) {
        report.error = "Weights size mismatch";
        return report;
    }
    std::vector<double> w = weights;
    if (w.empty()) {
        w.assign(embeddings.size(), 1.0 / static_cast<double>(embeddings.size()));
    }
    double sum_w = 0.0;
    for (double v : w) sum_w += v;
    if (sum_w == 0.0) {
        report.error = "Weights sum to zero";
        return report;
    }
    for (double& v : w) v /= sum_w;
    std::vector<double> mean(dim, 0.0);
    for (size_t i = 0; i < embeddings.size(); ++i) {
        for (size_t j = 0; j < dim; ++j) {
            mean[j] += embeddings[i][j] * w[i];
        }
    }
    double avg_distance = 0.0;
    for (size_t i = 0; i < embeddings.size(); ++i) {
        double dist = 0.0;
        for (size_t j = 0; j < dim; ++j) {
            double diff = embeddings[i][j] - mean[j];
            dist += diff * diff;
        }
        avg_distance += std::sqrt(dist);
    }
    avg_distance /= static_cast<double>(embeddings.size());
    double coherence = 1.0 / (1.0 + avg_distance);

    report.success = true;
    report.blended = std::move(mean);
    report.coherence = coherence;
    return report;
}

} // namespace testbed
} // namespace sep

#endif // SEP_TESTBED_CONTEXT_ALGORITHMS_HPP
