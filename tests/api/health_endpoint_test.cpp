#include "api/server.h"
#include <gtest/gtest.h>

using namespace sep::api;

TEST(APIServer, HealthEndpoint) {
    auto port = 12345; // ephemeral for unit test
    auto cfg = sep::config::APIConfig{};
    cfg.port = port;

    SEPApiServer server(cfg);
    server.setup_routes();
    auto resp = server.makeJsonResponse(200, "ok");
    EXPECT_EQ(resp->getCode(), 200);
}
