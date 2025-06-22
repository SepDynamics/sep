#include <gtest/gtest.h>
#include "crow/logging.h"
#include "crow/socket_adaptors.h"

using namespace crow;

TEST(SocketAdaptorShutdownTest, ReportsErrorsOnInvalidOperations) {
    asio::io_context io;
    SocketAdaptor ad(io, nullptr);

    ad.raw_socket().open(tcp::v4());
    auto ec = ad.shutdown_readwrite();
    EXPECT_TRUE(ec); // shutting down unopened socket should error
    ad.close();
}
