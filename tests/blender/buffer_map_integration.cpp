#include <gtest/gtest.h>
#include "blender/gpu_context.h"
#include <vector>
#include <cstring>

using namespace sep;

TEST(GPUContextIntegration, BufferMapping) {
    GPUContext ctx;
    ASSERT_EQ(ctx.init(), SEPResult::SUCCESS);

    const size_t size = 256;
    std::vector<uint8_t> src(size, 0x42);

    auto buf = ctx.createBuffer(size, src.data());
    ASSERT_TRUE(buf);

    void* mapped = ctx.mapBuffer(buf.get());
    ASSERT_NE(nullptr, mapped);

    std::vector<uint8_t> dst(size);
    std::memcpy(dst.data(), mapped, size);
    ctx.unmapBuffer(buf.get());

    for (size_t i = 0; i < size; ++i) {
        EXPECT_EQ(src[i], dst[i]);
    }
}
