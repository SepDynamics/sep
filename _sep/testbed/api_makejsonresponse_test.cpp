#include "api/server.h"
#include "config/api_config.h"
#include <gtest/gtest.h>
#include <nlohmann/json.hpp>

TEST(APIServerTest, MakeJsonResponseSuccess) {
    sep::config::APIConfig cfg{};
    sep::api::SEPApiServer server(cfg, nullptr);
    auto resp = server.makeJsonResponse(200, "ok");
    EXPECT_EQ(resp->getCode(), 200);
    auto body = nlohmann::json::parse(resp->getBody());
    EXPECT_EQ(body["status"], "success");
    EXPECT_EQ(body["message"], "ok");
}
