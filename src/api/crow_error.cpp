#include "crow/crow_error.h"
#include <cstdio>

void sep::crow::error::log(sep::crow::error::Code code, const std::string& message)
{
    (void)fprintf(stderr,
                  "Crow error %d: %s\n",
                  static_cast<int>(code),
                  message.c_str());
}

#if !defined(__cpp_exceptions) && !defined(__EXCEPTIONS) && !defined(_CPPUNWIND)
const char* crow::last_error = nullptr;
#endif
