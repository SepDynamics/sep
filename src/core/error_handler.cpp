#include "core/error_handler.h"
#include "compat/shim.h"

#include <functional> // Required for std::function

// Standard Library Includes 
namespace sep::core {
using ::sep::Error;
using std::vector;
using std::mutex;
using std::lock_guard;

ErrorHandler &ErrorHandler::instance() {
  static ErrorHandler handler;
  return handler;
}

void ErrorHandler::reportError(const Error &error, std::function<bool()> retry) {
  lock_guard<mutex> lock(mutex_);
  errors_.push_back({error, retry, 0});
  processRetriesLocked();
}

vector<Error> ErrorHandler::getErrors() const {
  lock_guard<mutex> lock(mutex_);
  vector<Error> result;
  result.reserve(errors_.size());
  for (const auto &e : errors_) {
    result.push_back(e.error);
  }
  return result;
}

void ErrorHandler::clearErrors() {
  lock_guard<mutex> lock(mutex_);
  errors_.clear();
}

bool ErrorHandler::hasErrors() const {
  lock_guard<mutex> lock(mutex_);
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
