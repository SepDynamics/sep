#include <cstdio>
#include <string>

namespace sep::crow::error {

enum class Code { None = 0, InvalidMethod };

void log(Code code, const std::string& message) {
    (void)fprintf(stderr, "Crow error %d: %s\n", static_cast<int>(code), message.c_str());
}

#if !defined(__cpp_exceptions) && !defined(__EXCEPTIONS) && !defined(_CPPUNWIND)
const char* last_error = nullptr;
#endif

} // namespace sep::crow::error
