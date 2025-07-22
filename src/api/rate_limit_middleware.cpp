// Always use the isolation header to avoid dependency on external crow.h
#include "api/rate_limit_middleware.h"
#include "api/crow_request.h"
#include "api/rate_limiter.h"

#include <nlohmann/json.hpp>

namespace sep::api {
RateLimitMiddleware::RateLimitMiddleware()
{
    // Use default configuration
    set_config(sep::config::RateLimitConfig{});
}

void RateLimitMiddleware::set_config(const sep::config::RateLimitConfig& config)
{
    config_ = config;
    if (config_.enabled)
    {
        rate_limiter_ = createRateLimiter(config_.rpm);
    }
    else
    {
        rate_limiter_.reset();
    }
}

void RateLimitMiddleware::before_handle(::crow::request& req, ::crow::response& res, context&)
{
    if (!config_.enabled || !rate_limiter_)
    {
        return;
    }

    CrowRequest wrapped(req);
    if (!rate_limiter_->checkRateLimit(wrapped))
    {
        res.code      = 429;  // Too Many Requests
        auto err_json = nlohmann::json::parse(rate_limiter_->getErrorResponse("Rate limit exceeded", 429));
        res.set_header("Content-Type", "application/json");
        res.body = err_json.dump();
        res.end();
    }
}

void RateLimitMiddleware::after_handle(::crow::request&, ::crow::response&, context&)
{
    // No-op
}

}  // namespace sep::api
