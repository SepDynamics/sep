#include <gtest/gtest.h>
#include "api/lock_free_rate_limiter.h"
#include "api/request_interface.h"
#include "sep/shim.h"
#include <unordered_map>

using namespace sep::api;

class SimpleRequest : public IRequest {
public:
    sep::shim::string method() const override { return "GET"; }
    sep::shim::string url() const override { return "/"; }
    const sep::shim::string& body() const override { return body_; }
    const std::unordered_map<sep::shim::string,sep::shim::string,sep::shim::string_hash>& headers() const override { return headers_; }
    sep::shim::string get_header_value(const sep::shim::string& key) const override {
        auto it = headers_.find(key); return it==headers_.end()? "" : it->second;
    }
    const sep::shim::string& get_remote_ip() const override { return ip_; }
    sep::shim::string body_; std::unordered_map<sep::shim::string,sep::shim::string,sep::shim::string_hash> headers_; sep::shim::string ip_{"127.0.0.1"};
};

TEST(RateLimiterTest, BasicLimit) {
    auto limiter = createLockFreeRateLimiter(5);
    SimpleRequest req;
    for (int i=0; i<5; ++i) {
        EXPECT_TRUE(limiter->checkRateLimit(req));
    }
    EXPECT_FALSE(limiter->checkRateLimit(req));
}
