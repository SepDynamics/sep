#ifndef SEP_CONTEXT_MOCK_PROCESSOR_HPP
#define SEP_CONTEXT_MOCK_PROCESSOR_HPP

#include "context/processor.h"
#include "context/types.h"
#include <vector>

namespace sep::context {

class MockProcessor : public Processor {
public:
    MockProcessor() = default;
    ~MockProcessor() override = default;

    ProcessResult processBatch(const Batch& batch) override {
        ProcessResult result;
        result.success = true;
        result.value = defaultResults;
        return result;
    }

    ProcessResult processContext(const Context& context) override {
        ProcessResult result;
        result.success = true;
        result.value = defaultResults;
        return result;
    }

    ValidationResult validateContext(const Context& context) override {
        ValidationResult result;
        result.valid = true;
        return result;
    }

    EmbeddingResult extractEmbeddings(const Context& context) override {
        EmbeddingResult result;
        result.success = true;
        result.value = {1.0f, 2.0f, 3.0f};
        return result;
    }

    SimilarityResult calculateSimilarity(const Context& a, const Context& b) override {
        SimilarityResult result;
        result.success = true;
        result.value = 0.95f;
        return result;
    }

    BlendResult blendContexts(const sep::shim::vector<Context>& contexts,
                              const sep::shim::vector<float>& weights) override {
        BlendResult result;
        result.success = true;
        result.value = contexts[0];
        return result;
    }

    ProcessorMetrics getMetrics() const override {
        return metrics_;
    }

    // Test control methods
    void setDefaultResults(const std::vector<CheckResult>& results) {
        defaultResults = results;
    }

private:
    std::vector<CheckResult> defaultResults;
    ProcessorMetrics         metrics_{};
};

// Override the factory function for testing
std::unique_ptr<Processor> createProcessor(const ProcessOptions& options) {
    return std::make_unique<MockProcessor>();
}

} // namespace sep::context
} // namespace sep

#endif // SEP_CONTEXT_MOCK_PROCESSOR_HPP
