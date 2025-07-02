#include "embeddings/simple_embedding_model.h"
#include <cmath>

namespace sep::embeddings {

SimpleEmbeddingModel::SimpleEmbeddingModel() {
    weights_ = {0.01, 0.02, 0.03, 0.04, 0.05};
}

std::vector<double> SimpleEmbeddingModel::compute(const std::string& text) const {
    std::vector<double> out(kDim, 0.0);
    for (unsigned char c : text) {
        for (std::size_t i = 0; i < kDim; ++i) {
            out[i] += static_cast<double>(c) * weights_[i];
        }
    }
    double norm = 0.0;
    for (double v : out) norm += v * v;
    norm = std::sqrt(norm);
    if (norm > 0.0) {
        for (double& v : out) v /= norm;
    }
    return out;
}

} // namespace sep::embeddings

