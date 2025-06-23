#pragma once
#include "context/types.h"
#include <memory>

namespace sep::context {

struct ProcessResult {
    bool success{false};
    ::sep::shim::vector<CheckResult> value;
};

struct ValidationResult {
    bool valid{false};
    sep::shim::string error;
};

struct EmbeddingResult {
    bool success{false};
    ::sep::shim::vector<float> value;
};

struct SimilarityResult {
    bool success{false};
    float value{0.0f};
};

struct BlendResult {
    bool success{false};
    Context value;
};

struct ProcessorMetrics {
    size_t processed{0};
};

struct ProcessOptions {};

class Processor {
public:
    virtual ~Processor() = default;
    virtual ProcessResult processBatch(const Batch& batch) = 0;
    virtual ProcessResult processContext(const Context& context) = 0;
    virtual ValidationResult validateContext(const Context& context) = 0;
    virtual EmbeddingResult extractEmbeddings(const Context& context) = 0;
    virtual SimilarityResult calculateSimilarity(const Context& a, const Context& b) = 0;
    virtual BlendResult blendContexts(const ::sep::shim::vector<Context>& contexts,
                                     const ::sep::shim::vector<float>& weights) = 0;
    virtual ProcessorMetrics getMetrics() const = 0;
};

std::unique_ptr<Processor> createProcessor(const ProcessOptions& options = ProcessOptions());

} // namespace sep::context
