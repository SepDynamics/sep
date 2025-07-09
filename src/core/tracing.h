#pragma once

#include <chrono>
#include <cstdint>
#include <memory>
#include <string>

namespace sep::metrics {

class TraceSpan {
 public:
  explicit TraceSpan(const std::string& name);
  ~TraceSpan();

  void setAttribute(const std::string& key, std::int64_t value);

 private:
  std::string name_;
  std::chrono::high_resolution_clock::time_point start_;
};

}  // namespace sep::metrics
