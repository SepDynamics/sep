#include <gtest/gtest.h>
#include "blender/gpu_context.h"
#include "sep/shim.h"
#include <fstream>
#include <cstdio>

using namespace sep;

TEST(GPUContextTestbed, ShaderReload) {
    GPUContext ctx;
    ASSERT_EQ(ctx.init(), SEPResult::SUCCESS);

    sep::shim::string shader = "src/blender/shaders/pattern_process.spv";
    ASSERT_EQ(ctx.loadComputeShader(shader), SEPResult::SUCCESS);
    uint32_t rev1 = ctx.getShaderRevision();
    EXPECT_GT(rev1, 0u);

    // Reload without modification should keep revision
    EXPECT_EQ(ctx.reloadComputeShaderIfNeeded(), SEPResult::SUCCESS);
    EXPECT_EQ(ctx.getShaderRevision(), rev1);

    // Copy shader to temp file to simulate update
    sep::shim::string temp = "pattern_process_copy.spv";
    {
        std::ifstream in(shader, std::ios::binary);
        std::ofstream out(temp, std::ios::binary);
        out << in.rdbuf();
    }
    ASSERT_EQ(ctx.loadComputeShader(temp), SEPResult::SUCCESS);
    uint32_t rev2 = ctx.getShaderRevision();
    EXPECT_GT(rev2, rev1);

    std::remove(temp.c_str());
}
