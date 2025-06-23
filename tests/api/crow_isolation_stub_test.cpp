#include <gtest/gtest.h>
#include "crow/crow_isolation.h"
#include "crow/asio_isolation.h"

TEST(CrowIsolationStub, ResponseFields)
{
    crow::response res;
    EXPECT_EQ(res.code, 200);
    EXPECT_EQ(res.status, 200);
    res.write("hello");
    EXPECT_EQ(res.body, "hello");
}

TEST(AsioIsolationStub, ReadWriteReturnTypes)
{
    asio::io_context io;
    char data[1];
    auto r = asio::read(io, asio::mutable_buffer(data, 1));
    auto w = asio::write(io, asio::const_buffer(data, 1));
    (void)r;
    (void)w;
    SUCCEED();
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
