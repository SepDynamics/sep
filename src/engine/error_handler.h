#ifndef SEP_CORE_ERROR_HANDLER_H
#define SEP_CORE_ERROR_HANDLER_H

#include "common.h"
#include "engine/shim.h"
// Removed duplicate include of core/types.h

namespace sep {

// Error type for the SEP engine
struct Error {
    shim::SEPResult code{shim::SEPResult::SUCCESS};
    ::sep::shim::string message;
    ::sep::shim::string location;
    
    Error() = default;
    Error(shim::SEPResult code, const ::sep::shim::string &msg, const ::sep::shim::string &loc = "")
        : code(code), message(msg), location(loc)
    {
    }
};

namespace core {

class ErrorHandler {
 public:
  static ErrorHandler &instance();

  void reportError(const ::sep::Error &error, std::function<bool()> retry = {});

  shim::vector<::sep::Error> getErrors() const;

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
  shim::vector<Entry> errors_;
};

}  // namespace core
}  // namespace sep

#endif  // SEP_CORE_ERROR_HANDLER_H
