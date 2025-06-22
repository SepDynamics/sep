#include "context/processor.h"
#include "context/types.h"
#include <gtest/gtest.h>

using namespace sep::context;

TEST(ContextPatternIntegration, UpdatesProcessorResult)
{
    ProcessOptions opts;
    auto           processor = createProcessor(opts);

    Context ctx;
    ctx.type                  = "embedding";
    ctx.content               = std::vector<float>{0.1f, 0.2f, 0.3f, 0.4f};
    ctx.metadata["timestamp"] = 1;

    auto result = processor->processContext(ctx);
    ASSERT_TRUE(result.success);
    ASSERT_TRUE(ctx.processorResult.contains("patterns"));
    auto patterns = ctx.processorResult["patterns"];
    EXPECT_FALSE(patterns.empty());
}
