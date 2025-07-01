#include "api/server.h"
#include "api/client.h"
#include "logging/manager.h"
#include <gtest/gtest.h>
#include <thread>

using namespace sep::api;

TEST(APIHealthEndpoint, ReturnsOk) {
    sep::logging::initializeLogging();
    uint16_t port = 18080;
    auto config = sep::config::APIConfig{};
    config.port = port;
    ServerGuard guard(config);

    ClientConfig client_config;
    client_config.baseUrl = "http://127.0.0.1:" + std::to_string(port);
    auto client = createClient(client_config);
    auto res = client->get("/api/v1/health");
    EXPECT_EQ(res.statusCode, 200);
    sep::logging::shutdownLogging();
}
