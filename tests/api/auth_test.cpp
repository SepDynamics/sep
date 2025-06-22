#include <gtest/gtest.h>
#include "../../include/api/auth_middleware.h"

using namespace sep::api;

struct TestAuthMiddleware : public AuthMiddleware {
    using AuthMiddleware::validate_token;
};

TEST(AuthMiddlewareTest, TokenValidation) {
    TestAuthMiddleware mw;
    mw.set_tokens({"secret"});
    EXPECT_TRUE(mw.validate_token("Bearer secret"));
    EXPECT_FALSE(mw.validate_token("Bearer wrong"));
}
