#include "api/client.h"
#include "api/server.h"
#include "api/types.h"
#include <gtest/gtest.h>
#include <chrono>
#include <thread>

using namespace sep::api;

static sep::config::APIConfig createConfig(uint16_t port) {
    auto cfg = sep::config::APIConfig{};
    cfg.port = port;
    return cfg;
}

class HealthServerTest : public ::testing::Test {};

TEST_F(HealthServerTest, HealthEndpointReturnsOk) {
    uint16_t port = 23765; // fixed port for test
    SEPApiServer server(createConfig(port));
    std::thread server_thread([&](){ server.run(); });
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    ClientConfig client_cfg; client_cfg.baseUrl = "http://127.0.0.1:" + std::to_string(port);
    auto client = createClient(client_cfg);
    auto res = client->get("/api/v1/health");
    EXPECT_EQ(res.statusCode, 200);

    server.stop();
    server_thread.join();
}
