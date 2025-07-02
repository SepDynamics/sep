#pragma once

#include <array>
#include <string>
#include <vector>

namespace sep::embeddings {

class SimpleEmbeddingModel {
public:
    static constexpr std::size_t kDim = 5;

    SimpleEmbeddingModel();

    std::vector<double> compute(const std::string& text) const;

private:
    std::array<double, kDim> weights_;
};

} // namespace sep::embeddings

