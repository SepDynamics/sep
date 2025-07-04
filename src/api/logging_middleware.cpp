#include "api/logging_middleware.h"
#include "api/crow_adapter.h"
#include <atomic>

#include <chrono> // For std::chrono

namespace sep::api {

void LoggingMiddleware::before_handle(crow::request& req, crow::response& res, context& ctx) {
  (void)req;
  if (!isReady()) {
    res.code = 503;
    res.end();
    return;
  }

  ctx.start = std::chrono::high_resolution_clock::now();
  std::atomic_thread_fence(std::memory_order_release);
}

void LoggingMiddleware::after_handle(crow::request& req, crow::response& res, context& ctx) {
  if (!isReady()) {
    return;
  }

  auto req_ptr = std::make_unique<sep::api::CrowRequestAdapter>(req);

  if (ctx.start != std::chrono::high_resolution_clock::time_point{}) {
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - ctx.start);
    server_->logRequest(*req_ptr, res.code, res.body, duration.count());
  } else {
    server_->logRequest(*req_ptr, res.code, res.body, 0);
  }
}

} // namespace sep::api