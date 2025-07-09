#ifndef SEP_API_RATE_LIMITER_H
#define SEP_API_RATE_LIMITER_H

#include "api/types.h"
#include "request_interface.h"
#include <memory>
#include <string>

namespace sep::api {

class IRateLimiter {
public:
    virtual ~IRateLimiter() = default;

    // Core rate limiting interface
    virtual bool checkRateLimit(const IRequest& req) = 0;
    virtual std::string getErrorResponse(const std::string& message, int status) = 0;

    // Configuration methods
    virtual void setEnabled(bool enabled) = 0;
    virtual void setPriorityQuota(Priority priority, float multiplier) = 0;

    // Monitoring methods
    virtual unsigned int GetRequestCount(const std::string& client_id) const = 0;
    virtual unsigned int GetWindowSize(
        const std::string& client_id,
        Priority priority
    ) const = 0;
};

// Factory function declaration
std::unique_ptr<IRateLimiter> createRateLimiter(unsigned int requests_per_minute);

} // namespace sep::api

#endif // SEP_API_RATE_LIMITER_H
