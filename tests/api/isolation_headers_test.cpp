#include "crow/asio_isolation.h"
#include "crow/crow_isolation.h"
#include <gtest/gtest.h>

TEST(IsolationHeadersApi, BasicUsage)
{
    asio::io_context ctx;
    crow::SimpleApp  app;
    (void)ctx;
    (void)app;
    SUCCEED();
}
