#include "api/client.h"
#include "api/server.h"
#include "api/types.h"
#include "blender/cycles_renderer.h"
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
    sep::logging::Manager::shutdown();
  
TEST(APIServer, HealthEndpoint) {
    auto port = 12345; // ephemeral for unit test
    auto cfg = sep::config::APIConfig{};
    cfg.port = port;

    auto renderer = std::make_unique<sep::blender::ccl::CyclesRenderer>();
    renderer->initialize();
    SEPApiServer server(cfg, renderer.get());
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
    auto renderer2 = std::make_unique<sep::blender::ccl::CyclesRenderer>();
    renderer2->initialize();
    SEPApiServer server(createConfig(port), renderer2.get());
    std::thread server_thread([&](){ server.run(); });
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    ClientConfig client_cfg; client_cfg.baseUrl = "http://127.0.0.1:" + std::to_string(port);
    auto client = createClient(client_cfg);
    auto res = client->get("/api/v1/health");
    EXPECT_EQ(res.statusCode, 200);

    server.stop();
    server_thread.join();
}

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
    sep::logging::Manager::shutdown();
}
