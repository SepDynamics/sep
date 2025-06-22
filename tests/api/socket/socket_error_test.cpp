#include "crow/socket_adaptors.h"
#include <gtest/gtest.h>

TEST(SocketAdaptorTest, CloseUnusedSocketReturnsError)
{
    asio::io_context    io;
    crow::SocketAdaptor ad(io, nullptr);
    auto                ec = ad.close();
    EXPECT_TRUE(bool(ec));
}

TEST(SocketAdaptorTest, ShutdownUnusedSocketReturnsError)
{
    asio::io_context    io;
    crow::SocketAdaptor ad(io, nullptr);
    auto                ec1 = ad.shutdown_readwrite();
    EXPECT_TRUE(bool(ec1));
    auto ec2 = ad.shutdown_write();
    EXPECT_TRUE(bool(ec2));
    auto ec3 = ad.shutdown_read();
    EXPECT_TRUE(bool(ec3));
}
