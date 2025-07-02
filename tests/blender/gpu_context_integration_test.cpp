#include "blender/gpu_context.h"
#include <gtest/gtest.h>
#include <vector>
#include <cstring>

TEST(GPUContextIntegration, BufferMapAndShaderReload) {
    sep::GPUContext ctx;
    ASSERT_EQ(sep::SEPResult::SUCCESS, ctx.init());

    const size_t size = 64;
    std::vector<uint8_t> data(size, 7);
    auto buf = ctx.createBuffer(size, data.data());
    ASSERT_TRUE(buf);

    void* ptr = ctx.mapBuffer(buf.get());
    ASSERT_NE(nullptr, ptr);
    std::vector<uint8_t> host(size);
    std::memcpy(host.data(), ptr, size);
    ctx.unmapBuffer(buf.get());

    for (size_t i = 0; i < size; ++i) {
        EXPECT_EQ(data[i], host[i]);
    }

    sep::shim::string shader_path = "_sep/testbed/dummy.spv";
    ASSERT_EQ(sep::SEPResult::SUCCESS, ctx.loadComputeShader(shader_path));
    uint32_t rev1 = ctx.getShaderRevision();
    EXPECT_GT(rev1, 0u);
    EXPECT_EQ(sep::SEPResult::SUCCESS, ctx.reloadComputeShaderIfNeeded());
    EXPECT_EQ(rev1, ctx.getShaderRevision());
}
