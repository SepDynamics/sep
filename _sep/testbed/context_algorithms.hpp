#ifndef SEP_TESTBED_CONTEXT_ALGORITHMS_HPP
#define SEP_TESTBED_CONTEXT_ALGORITHMS_HPP

#include <nlohmann/json.hpp>
#include <vector>
#include <string>
#include <cmath>

namespace sep {
namespace testbed {

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
                  ctx["metadata"].contains("timestamp");
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
