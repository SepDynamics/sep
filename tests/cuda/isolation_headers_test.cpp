#include "sep/asio_isolation.h"
#include "sep/crow_isolation.h"
#include <gtest/gtest.h>

TEST(IsolationHeadersCuda, BasicUsage)
{
    asio::io_context ctx;
    crow::SimpleApp  app;
    (void)ctx;
    (void)app;
    SUCCEED();
}
