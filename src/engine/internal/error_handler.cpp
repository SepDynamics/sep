#include "error_handler.h"

#include <functional>  // Required for std::function
#include <mutex>
#include <vector>

#include "engine/internal/standard_includes.h"

namespace sep::core {
using ::sep::Error;

ErrorHandler &ErrorHandler::instance() {
  static ErrorHandler handler;
  return handler;
}

void ErrorHandler::reportError(const Error &error, std::function<bool()> retry) {
  std::lock_guard<std::mutex> lock(mutex_);
  errors_.push_back({error, retry, 0});
  processRetriesLocked();
}

std::vector<Error> ErrorHandler::getErrors() const
{
    std::lock_guard<std::mutex> lock(mutex_);
    std::vector<Error> result;
    result.reserve(errors_.size());
    for (const auto &e : errors_)
    {
        result.push_back(e.error);
    }
    return result;
}

void ErrorHandler::clearErrors() {
  std::lock_guard<std::mutex> lock(mutex_);
  errors_.clear();
}

bool ErrorHandler::hasErrors() const {
  std::lock_guard<std::mutex> lock(mutex_);
  return !errors_.empty();
}

void ErrorHandler::processRetriesLocked() {
  for (auto it = errors_.begin(); it != errors_.end();) {
    if (it->retry && it->attempts < 3) {
      ++(it->attempts);
      bool success = it->retry();
      if (success) {
        it = errors_.erase(it);
        continue;
      }
    }
    ++it;
  }
}

}  // namespace sep::core
