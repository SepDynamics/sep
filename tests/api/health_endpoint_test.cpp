#include "api/client.h"
#include "api/server.h"
#include "api/types.h"
#include <gtest/gtest.h>
#include <chrono>
#include <thread>
#include "api/server.h"
#include <gtest/gtest.h>

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
  
TEST(APIServer, HealthEndpoint) {
    auto port = 12345; // ephemeral for unit test
    auto cfg = sep::config::APIConfig{};
    cfg.port = port;

    SEPApiServer server(cfg);
    server.setup_routes();
    auto resp = server.makeJsonResponse(200, "ok");
    EXPECT_EQ(resp->getCode(), 200);

static sep::config::APIConfig createConfig(uint16_t port) {
    auto cfg = sep::config::APIConfig{};
    cfg.port = port;
    return cfg;

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
