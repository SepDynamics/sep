#include <gtest/gtest.h>
#include "blender/gpu_context.h"
#include <fstream>
#include <cstdio>

using namespace sep;

TEST(GPUContextRegression, Initialization) {
    GPUContext ctx;
    EXPECT_EQ(ctx.init(), SEPResult::SUCCESS);
    EXPECT_FALSE(ctx.hasError());
}

TEST(GPUContextRegression, ShaderReload) {
    GPUContext ctx;
    ASSERT_EQ(ctx.init(), SEPResult::SUCCESS);

    sep::shim::string shader = "_sep/testbed/dummy.spv";
    ASSERT_EQ(ctx.loadComputeShader(shader), SEPResult::SUCCESS);
    uint32_t rev = ctx.getShaderRevision();

    EXPECT_EQ(ctx.reloadComputeShaderIfNeeded(), SEPResult::SUCCESS);
    EXPECT_EQ(ctx.getShaderRevision(), rev);

    sep::shim::string temp = "_sep/testbed/dummy_temp.spv";
    {
        std::ifstream in(shader.c_str(), std::ios::binary);
        std::ofstream out(temp.c_str(), std::ios::binary);
        out << in.rdbuf();
    }
    ASSERT_EQ(ctx.loadComputeShader(temp), SEPResult::SUCCESS);
    EXPECT_GT(ctx.getShaderRevision(), rev);
    std::remove(temp.c_str());
}
