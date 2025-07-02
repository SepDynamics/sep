#include <gtest/gtest.h>
#include "blender/gpu_context.h"
#include "compat/shim.h"
#include <fstream>
#include <cstdio>

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

    // Simulate an updated shader by copying to a new temporary file
    sep::shim::string updated = "dummy_updated.spv";
    {
        std::ifstream in(shader.c_str(), std::ios::binary);
        std::ofstream out(updated.c_str(), std::ios::binary);
        out << in.rdbuf();
    }
    ASSERT_EQ(ctx.loadComputeShader(updated), SEPResult::SUCCESS);
    EXPECT_GT(ctx.getShaderRevision(), rev1);
    std::remove(updated.c_str());
}
