#include "compat/shim.h"
#include "api/json_helpers.h"
#include <iostream>

int main() {
    sep::shim::string s("hello");
    s += " world";
    auto j = sep::api::parse_json("{\"a\":1}");
    std::cout << s.c_str() << ' ' << j["a"] << std::endl;
    return 0;
}
