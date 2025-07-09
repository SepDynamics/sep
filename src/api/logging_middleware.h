#pragma once

#include "core/logging.h"
#include "api/server.h"
#include "crow/crow_isolation.h"
#include <chrono>

namespace sep::api {

class LoggingMiddleware {
public:
  struct context {
    std::chrono::high_resolution_clock::time_point start;
  };

  LoggingMiddleware(sep::api::Server* server) : server_(server) {}

  void before_handle(crow::request& req, crow::response& res, context& ctx);
  void after_handle(crow::request& req, crow::response& res, context& ctx);

  bool isReady() const { return server_ != nullptr; }

private:
  sep::api::Server* server_;
};

} // namespace sep::api