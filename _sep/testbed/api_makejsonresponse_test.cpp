#include "api/server.h"
#include "config/api_config.h"
#include "blender/cycles_renderer.h"
#include <gtest/gtest.h>
#include <nlohmann/json.hpp>

TEST(APIServerTest, MakeJsonResponseSuccess) {
    sep::config::APIConfig cfg{};
    sep::blender::ccl::CyclesRenderer renderer;
    sep::api::SEPApiServer server(cfg, &renderer);
    auto resp = server.makeJsonResponse(200, "ok");
    EXPECT_EQ(resp->getCode(), 200);
    auto body = nlohmann::json::parse(resp->getBody());
    EXPECT_EQ(body["status"], "success");
    EXPECT_EQ(body["message"], "ok");
}
