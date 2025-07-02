#include "core/tracing.h"
#include <gmock/gmock.h>
#include <string>

namespace sep::metrics {

class MockTraceSpan {
public:
    MOCK_METHOD(void, setAttribute, (const std::string& key, std::int64_t value));
    
    MockTraceSpan(const std::string& name) : name_(name) {}
    ~MockTraceSpan() = default;

private:
    std::string name_;
};

} // namespace sep::metrics