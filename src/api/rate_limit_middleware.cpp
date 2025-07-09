// Always use the isolation header to avoid dependency on external crow.h
#include "api/rate_limit_middleware.h"

#include <memory>
#include <nlohmann/json.hpp>

#include "api/crow_request.h"
#include "api/rate_limiter.h"
#include "crow/crow_isolation.h"

namespace sep::api {
RateLimitMiddleware::RateLimitMiddleware() {
    set_config(sep::api::RateLimitConfig{});
}

void RateLimitMiddleware::set_config(const sep::api::RateLimitConfig& config) {
    config_ = config;
    if (config_.enabled) {
        rate_limiter_ = createRateLimiter(config_.requests_per_minute);  // Fixed property name
    } else {
        rate_limiter_.reset();
    }
}

// Template implementation of before_handle
template <typename Req, typename Res, typename Ctx>
void RateLimitMiddleware::before_handle(Req& req, Res& res, Ctx&)
{
    if (!config_.enabled || !rate_limiter_) {
        return;
    }

    CrowRequest wrapped(req);
    if (!rate_limiter_->checkRateLimit(wrapped)) {
        res.code = 429;  // Too Many Requests
        // For the isolation shim, we won't attempt to set real values
        // Just set the code and let the rate limiter know we've rejected the request
        res.code = 429;  // Too Many Requests
        
        // When using the real Crow, this would be a proper JSON response
        // But for the isolation shim, we just need to signal rejection
        res.end();
    }
}

// Template implementation of after_handle
template <typename Req, typename Res, typename Ctx>
void RateLimitMiddleware::after_handle(Req&, Res&, Ctx&)
{
    // No-op
}

// Explicit template instantiations for crow types
template void
RateLimitMiddleware::before_handle<::crow::request, ::crow::response, RateLimitMiddleware::context>(
    ::crow::request&, ::crow::response&, RateLimitMiddleware::context&);
template void
RateLimitMiddleware::after_handle<::crow::request, ::crow::response, RateLimitMiddleware::context>(
    ::crow::request&, ::crow::response&, RateLimitMiddleware::context&);

}  // namespace sep::api
