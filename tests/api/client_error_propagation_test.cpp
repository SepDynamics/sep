#include "api/client.h"
#include <gtest/gtest.h>

using namespace sep::api;

TEST(ClientErrorPropagation, NullHttpClient) {
    ClientConfig cfg;
    cfg.baseUrl = "http://localhost";
    cfg.maxRetries = 0;

    Client client(cfg, std::unique_ptr<IHttpClient>{});
    auto response = client.get("/ping");

    EXPECT_FALSE(response.success);
    EXPECT_EQ(response.error.code, sep::ErrorCode::ApiError);
    EXPECT_STREQ(response.error.message.c_str(), "HTTP client not initialized");
}
