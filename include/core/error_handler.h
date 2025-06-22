#ifndef SEP_CORE_ERROR_HANDLER_H
#define SEP_CORE_ERROR_HANDLER_H

#include <functional>
#include <mutex>
#include "compat/shim.h"

#include "core/types.h"

namespace sep::core {

class ErrorHandler {
 public:
  static ErrorHandler &instance();

  void reportError(const ::sep::Error &error, std::function<bool()> retry = {});

  ::sep::shim::vector<::sep::Error> getErrors() const;

  void clearErrors();

  bool hasErrors() const;

 private:
  struct Entry {
    ::sep::Error error;
    std::function<bool()> retry;
    std::uint32_t attempts{0};
  };

  ErrorHandler() = default;
  void processRetriesLocked();

  mutable std::mutex mutex_;
  ::sep::shim::vector<Entry> errors_;
};

}  // namespace sep::core

#endif  // SEP_CORE_ERROR_HANDLER_H
