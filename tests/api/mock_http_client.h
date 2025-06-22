#ifndef SEP_TEST_MOCK_HTTP_CLIENT_H
#define SEP_TEST_MOCK_HTTP_CLIENT_H

#include "api/client.h"
#include <functional>
#include <gtest/gtest.h>

namespace sep::api::test {

class MockHttpClient : public IHttpClient {
public:
  MockHttpClient() = default;
  ~MockHttpClient() override = default;

  // Override the pure virtual method
  APIResponse send_request(const APIRequest &request) override {
    if (responseHandler) {
      return responseHandler(request);
    }
    return defaultResponse;
  }

  // Helper to set up default successful response
  void SetupDefaultResponse() {
    defaultResponse.success = true;
    defaultResponse.statusCode = 200;
    defaultResponse.body = "{}";
    defaultResponse.responseTime = std::chrono::milliseconds(50);

    responseHandler = [this](const APIRequest &request) {
      auto response = defaultResponse;
      if (request.method == "POST") {
        response.statusCode = 201;
      }
      return response;
    };
  }

  // Helper to set up error response
  void SetupErrorResponse() {
    defaultResponse.success = false;
    defaultResponse.statusCode = 500;
    defaultResponse.error = "Internal Server Error";
    defaultResponse.responseTime = std::chrono::milliseconds(100);

    responseHandler = [this](const APIRequest &) { return defaultResponse; };
  }

  // Helper to set up timeout response
  void SetupTimeoutResponse() {
    defaultResponse.success = false;
    defaultResponse.statusCode = 408;
    defaultResponse.error = "Request Timeout";
    defaultResponse.responseTime = std::chrono::milliseconds(5000);

    responseHandler = [this](const APIRequest &) { return defaultResponse; };
  }

  // Set a custom response handler
  void
  SetResponseHandler(std::function<APIResponse(const APIRequest &)> handler) {
    responseHandler = std::move(handler);
  }

private:
  APIResponse defaultResponse;
  std::function<APIResponse(const APIRequest &)> responseHandler;
};

} // namespace sep::api::test

#endif // SEP_TEST_MOCK_HTTP_CLIENT_H
