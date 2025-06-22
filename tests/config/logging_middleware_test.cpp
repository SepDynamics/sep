#include "api/server.h"
#include <logging/manager.h>
#include <gtest/gtest.h>
#include <crow.h>
#include <chrono>
#include <thread>

using namespace sep;

namespace {

class FakeServer : public api::Server {
 public:
  api::ServerMetrics metrics{};
  std::mutex mtx{};
  int last_code{0};
  int64_t last_duration{0};
  int count{0};

  std::mutex &getMetricsMutex() override { return mtx; }
  api::ServerMetrics &getModifiableMetrics() override { return metrics; }
  std::unique_ptr<api::HttpResponse> makeJsonResponse(int, const std::string &) override { return nullptr; }
  std::string handleError(const std::string &msg, int) override { return msg; }
  void logRequest(const api::HttpRequest &, int code, const std::string &, int64_t dur) override {
    last_code = code;
    last_duration = dur;
    ++count;
  }
};

}  // namespace

class LoggingMiddlewareTest : public ::testing::Test {
 protected:
  FakeServer server;
  logging::LoggingMiddleware middleware{&server};
};

TEST_F(LoggingMiddlewareTest, RecordsRequest) {
  crow::request req;
  crow::response res;
  logging::LoggingMiddleware::context ctx;

  res.code = 200;
  middleware.before_handle(req, res, ctx);
  std::this_thread::sleep_for(std::chrono::milliseconds(1));
  middleware.after_handle(req, res, ctx);

  EXPECT_EQ(server.last_code, 200);
  EXPECT_EQ(server.count, 1);
  EXPECT_GE(server.last_duration, 0);
}

TEST_F(LoggingMiddlewareTest, ServerNotReady) {
  logging::LoggingMiddleware mw{nullptr};
  crow::request req;
  crow::response res;
  logging::LoggingMiddleware::context ctx;

  mw.before_handle(req, res, ctx);
  EXPECT_EQ(res.code, 503);
}
