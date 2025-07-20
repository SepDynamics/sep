#pragma once

#include <chrono>
#include <cstdint>
#include <memory>
#include <string>

#include "engine/shim.h"

namespace sep::metrics {

class TraceSpan {
 public:
     explicit TraceSpan(const shim::string& name);
     ~TraceSpan();

     void setAttribute(const shim::string& key, std::int64_t value);

 private:
     shim::string name_;
     std::chrono::high_resolution_clock::time_point start_;
};

}  // namespace sep::metrics
