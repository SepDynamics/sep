#include "api/server.h"
#include "config/api_config.h"
#include "blender/cycles_renderer.h"
#include <gtest/gtest.h>
#include <nlohmann/json.hpp>

TEST(APIServerTest, MakeJsonResponseSuccess) {
    sep::config::APIConfig cfg{};
    auto renderer = std::make_unique<sep::blender::ccl::CyclesRenderer>();
    renderer->initialize();
    sep::api::SEPApiServer server(cfg, renderer.get());
    auto resp = server.makeJsonResponse(200, "ok");
    EXPECT_EQ(resp->getCode(), 200);
    auto body = nlohmann::json::parse(resp->getBody());
    EXPECT_EQ(body["status"], "success");
    EXPECT_EQ(body["message"], "ok");
}
