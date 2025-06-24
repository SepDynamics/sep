#include "compat/shim.h"
#include "api/json_helpers.h"
#include "memory/memory_tier_manager.hpp"
#include <iostream>

int main() {
    sep::shim::string s("hello");
    s += " world";
    auto j = sep::api::parse_json("{\"a\":1}");
    sep::memory::MemoryTierManager mgr;
    mgr.allocate(128, sep::memory::TierType::STM);
    std::cout << s.c_str() << ' ' << j["a"]
              << " total=" << mgr.getTotalAllocated() << std::endl;
    return 0;
}
