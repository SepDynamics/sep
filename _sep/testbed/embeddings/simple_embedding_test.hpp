#pragma once
#include "embeddings/simple_embedding_model.h"
#include <string>
#include <vector>

namespace sep::testbed::embeddings {
inline std::vector<double> compute_test_embedding(const std::string& text) {
    static sep::embeddings::SimpleEmbeddingModel model;
    return model.compute(text);
}
} // namespace sep::testbed::embeddings

