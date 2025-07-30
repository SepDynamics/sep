#ifndef SEP_CORE_ERROR_HANDLER_H
#define SEP_CORE_ERROR_HANDLER_H

#include "common.h"
#include "engine/internal/standard_includes.h"
// Removed duplicate include of core/types.h

namespace sep {

// Error type for the SEP engine
struct Error {
    sep::SEPResult code{sep::SEPResult::SUCCESS};
    std::string message;
    std::string location;

    Error() = default;
    Error(sep::SEPResult code, const std::string &msg, const std::string &loc = "")
        : code(code), message(msg), location(loc)
    {
    }
};

namespace core {

class ErrorHandler {
 public:
  static ErrorHandler &instance();

  void reportError(const sep::Error &error, std::function<bool()> retry = {});

  std::vector<::sep::Error> getErrors() const;

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
  std::vector<Entry> errors_;
};

}  // namespace core
}  // namespace sep

#endif  // SEP_CORE_ERROR_HANDLER_H
