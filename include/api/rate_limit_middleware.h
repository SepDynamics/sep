#pragma once

#ifdef CROW_DISABLE_RTTI
#include "crow/crow_isolation.h"
#else
#include "crow/http_request.h"
#include "crow/http_response.h"
#endif

#include "api/rate_limiter.h"
#include "api/types.h"
#include "core/types.h"  // Include for RateLimitConfig

namespace sep::api {

class RateLimitMiddleware
{
public:
    // Context object for RateLimitMiddleware
    struct context
    {};

    RateLimitMiddleware();
    // Added set_config method
    void set_config(const sep::api::RateLimitConfig& config);

    void before_handle(::crow::request& req, ::crow::response& res, context& ctx);
    void after_handle(::crow::request& req, ::crow::response& res, context& ctx);

private:
    std::unique_ptr<IRateLimiter> rate_limiter_;
    sep::api::RateLimitConfig  config_;  // Store the config here
};

}  // namespace sep::api
