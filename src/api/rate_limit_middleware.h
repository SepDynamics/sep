#pragma once

#include "api/rate_limiter.h"
#include "api/types.h"
#include "crow/crow_isolation.h"

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

    // Use forward declarations to avoid direct dependency on crow types
    template<typename Req, typename Res, typename Ctx>
    void before_handle(Req& req, Res& res, Ctx& ctx);
    
    template<typename Req, typename Res, typename Ctx>
    void after_handle(Req& req, Res& res, Ctx& ctx);

private:
    std::unique_ptr<IRateLimiter> rate_limiter_;
    sep::api::RateLimitConfig  config_;  // Store the config here
};

}  // namespace sep::api
