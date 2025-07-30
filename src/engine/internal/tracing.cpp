#include "tracing.h"

#include <iostream>
#include <memory>

namespace sep {
namespace metrics {

    TraceSpan::TraceSpan(const std::string &name)
        : name_(name), start_(std::chrono::high_resolution_clock::now())
    {
#ifdef SEP_VERBOSE_TRACE
  std::cout << "[TRACE] Start span: " << name_ << std::endl;
#endif
    }

TraceSpan::~TraceSpan() {
#ifdef SEP_VERBOSE_TRACE
  auto end = std::chrono::high_resolution_clock::now();
  auto duration =
      std::chrono::duration_cast<std::chrono::microseconds>(end - start_)
          .count();
  std::cout << "[TRACE] End span: " << name_ << " (duration: " << duration
            << "\xC2\xB5s)" << std::endl;
#endif
}

void TraceSpan::setAttribute(const std::string &key, std::int64_t value)
{
#ifdef SEP_VERBOSE_TRACE
  std::cout << "[TRACE] Attribute: " << key << "=" << value
            << " for span: " << name_ << std::endl;
#else
  (void)key;
  (void)value;
#endif
}

}  // namespace metrics
}  // namespace sep
