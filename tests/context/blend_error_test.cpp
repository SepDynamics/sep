#include "context/processor.h"
#include <gtest/gtest.h>
#include <memory>
#include <vector>

namespace sep::test {

class BlendErrorTest : public ::testing::Test {
protected:
    void SetUp() override {
        context::ProcessOptions options;
        processor = sep::context::createProcessor(options);
    }

    std::unique_ptr<context::Processor> processor;

    context::Context createEmbedding(const std::vector<float>& emb) {
        context::Context ctx;
        ctx.type = "embedding";
        ctx.content = emb;
        return ctx;
    }
};

TEST_F(BlendErrorTest, InconsistentEmbeddingDimensions) {
    std::vector<context::Context> contexts;
    contexts.push_back(createEmbedding({0.1f, 0.2f}));
    contexts.push_back(createEmbedding({0.3f, 0.4f, 0.5f}));

    std::vector<float> weights{0.5f, 0.5f};
    auto result = processor->blendContexts(contexts, weights);
    EXPECT_FALSE(result.success);
    EXPECT_EQ(result.error, "Inconsistent embedding dimensions");
}

} // namespace sep::test
