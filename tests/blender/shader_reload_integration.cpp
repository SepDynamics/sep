#include <gtest/gtest.h>
#include "blender/gpu_context.h"
#include "compat/shim.h"

using namespace sep;

TEST(GPUContextIntegration, ShaderReload) {
    GPUContext ctx;
    ASSERT_EQ(ctx.init(), SEPResult::SUCCESS);

    sep::shim::string shader = "dummy.spv";
    ASSERT_EQ(ctx.loadComputeShader(shader), SEPResult::SUCCESS);
    uint32_t rev1 = ctx.getShaderRevision();
    EXPECT_GT(rev1, 0u);

    EXPECT_EQ(ctx.reloadComputeShaderIfNeeded(), SEPResult::SUCCESS);
    EXPECT_EQ(ctx.getShaderRevision(), rev1);

    ASSERT_EQ(ctx.loadComputeShader("updated.spv"), SEPResult::SUCCESS);
    EXPECT_GT(ctx.getShaderRevision(), rev1);
}
