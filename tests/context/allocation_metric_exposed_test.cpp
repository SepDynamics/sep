#include "context/processor.h"
#include "context/types.h"
#include "core/allocation_metrics.h"
#include <gtest/gtest.h>
#include <vector>

using namespace sep::context;

namespace {
class FailingProcessor : public Processor
{
public:
    ProcessResult processBatch(const Batch& batch) override
    {
        return processContext(batch.contexts.front());
    }
    ProcessResult processContext(const Context& ctx) override
    {
        if (ctx.content.is_array() && ctx.content.size() > 1'000'000)
        {
            metrics_.allocation_failures++;
            sep::metrics::allocationFailures().value++;
            return ProcessResult::fail("Memory allocation failed");
        }
        return ProcessResult::ok({});
    }
    ValidationResult validateContext(const Context&) override
    {
        return {true, "", {}};
    }
    EmbeddingResult extractEmbeddings(const Context&) override
    {
        return {true, {}, ""};
    }
    SimilarityResult calculateSimilarity(const Context&, const Context&) override
    {
        return {true, 0.0f, ""};
    }
    BlendResult blendContexts(const std::vector<Context>&, const std::vector<float>&) override
    {
        return {true, {}, ""};
    }
    ProcessorMetrics getMetrics() const override
    {
        return metrics_;
    }

private:
    ProcessorMetrics metrics_{};
};
}  // namespace

TEST(ContextMetrics, AllocationFailureCounterExposed)
{
    auto before = sep::metrics::allocationFailures().value.load();

    FailingProcessor processor;
    Context          ctx;
    ctx.type    = "embedding";
    ctx.content = std::vector<float>(2 * 1024 * 1024, 1.0f);

    auto result = processor.processContext(ctx);
    EXPECT_FALSE(result.success);

    auto after = sep::metrics::allocationFailures().value.load();
    EXPECT_GT(after, before);
}
