#include "api/api_exception.h"
#include "api/client.h"
#include "mock_http_client.h"
#include <chrono>
#include <future>
#include <gtest/gtest.h>
#include <thread>

using namespace sep::api;
using namespace sep::api::test;
using namespace std::chrono_literals;

class HttpClientTest : public ::testing::Test {
protected:
  void SetUp() override {
    // Create the mock and keep a raw pointer before transferring ownership
    auto httpClient = std::make_unique<MockHttpClient>();
    mockHttpClient = httpClient.get();
    mockHttpClient->SetupDefaultResponse();

    config.baseUrl = "http://127.0.0.1:8080";
    config.defaultTimeout = 5000ms;
    config.maxRetries = 3; // Set max retries
    config.defaultHeaders = {{"Content-Type", "application/json"},
                             {"Authorization", "Bearer test-token"}};

    // Create client with the mock HTTP client
    client = std::make_unique<Client>(config, std::move(httpClient));
  }

  ClientConfig config;
  std::unique_ptr<Client> client;
  MockHttpClient *mockHttpClient =
      nullptr; // Non-owning pointer for test access
};

TEST_F(HttpClientTest, BasicGetRequest) {
  mockHttpClient->SetResponseHandler([this](const APIRequest &request) {
    EXPECT_EQ(request.method, "GET");
    EXPECT_EQ(request.url, config.baseUrl + "/test");

    APIResponse response;
    response.success = true;
    response.statusCode = 200;
    response.responseTime = 100ms;
    return response;
  });

  auto response = client->get("/test");
  EXPECT_TRUE(response.success);
  EXPECT_EQ(response.statusCode, 200);
}

TEST_F(HttpClientTest, AsyncPostRequest) {
  std::string body = "{\"data\":\"test\"}";

  mockHttpClient->SetResponseHandler([this, &body](const APIRequest &request) {
    EXPECT_EQ(request.method, "POST");
    EXPECT_EQ(request.body, body);
    EXPECT_EQ(request.url, config.baseUrl + "/test");

    APIResponse response;
    response.success = true;
    response.statusCode = 201;
    response.responseTime = 100ms;
    return response;
  });

  auto response = client->post("/test", body);
  EXPECT_TRUE(response.success);
  EXPECT_EQ(response.statusCode, 201);
}

TEST_F(HttpClientTest, RequestTimeout) {
  mockHttpClient->SetupTimeoutResponse();

  // Use get() instead of raw send() to ensure proper URL handling
  auto response = client->get("/slow");
  EXPECT_FALSE(response.success);
  EXPECT_EQ(response.statusCode, 408);
}

TEST_F(HttpClientTest, HeaderHandling) {
  mockHttpClient->SetResponseHandler([this](const APIRequest &request) {
    // Check if headers from config were properly applied
    EXPECT_TRUE(request.headers.count("Content-Type") > 0);
    EXPECT_TRUE(request.headers.count("Authorization") > 0);
    if (request.headers.count("Content-Type") > 0) {
      EXPECT_EQ(request.headers.at("Content-Type"), "application/json");
    }
    if (request.headers.count("Authorization") > 0) {
      EXPECT_EQ(request.headers.at("Authorization"), "Bearer test-token");
    }
    EXPECT_EQ(request.url, config.baseUrl + "/headers");

    APIResponse response;
    response.success = true;
    response.statusCode = 200;
    response.responseTime = 100ms;
    return response;
  });

  auto response = client->get("/headers");
  EXPECT_TRUE(response.success);
  EXPECT_EQ(response.statusCode, 200);
}

TEST_F(HttpClientTest, ErrorHandling) {
  mockHttpClient->SetupErrorResponse();

  auto response = client->get("/error");
  EXPECT_FALSE(response.success);
  EXPECT_EQ(response.statusCode, 500);
}

TEST_F(HttpClientTest, ConcurrentRequests) {
  int requestCount = 0;
  std::mutex countMutex;

  mockHttpClient->SetResponseHandler(
      [this, &countMutex, &requestCount](const APIRequest &request) {
        EXPECT_EQ(request.method, "GET");
        EXPECT_TRUE(request.url.find(config.baseUrl + "/concurrent/") == 0);

        {
          std::lock_guard<std::mutex> lock(countMutex);
          requestCount++;
        }

        APIResponse response;
        response.success = true;
        response.statusCode = 200;
        response.responseTime = 100ms;
        return response;
      });

  std::vector<std::future<APIResponse>> futures;
  for (int i = 0; i < 10; ++i) {
    futures.push_back(std::async(std::launch::async, [this, i]() {
      return client->get("/concurrent/" + std::to_string(i));
    }));
  }

  for (auto &future : futures) {
    auto response = future.get();
    EXPECT_TRUE(response.success);
    EXPECT_EQ(response.statusCode, 200);
  }

  EXPECT_EQ(requestCount, 10);
}

TEST_F(HttpClientTest, PutRequest) {
  std::string body = "{\"data\":\"update\"}";

  mockHttpClient->SetResponseHandler([this, &body](const APIRequest &request) {
    EXPECT_EQ(request.method, "PUT");
    EXPECT_EQ(request.body, body);
    EXPECT_EQ(request.url, config.baseUrl + "/test/1");

    APIResponse response;
    response.success = true;
    response.statusCode = 200;
    response.responseTime = 100ms;
    return response;
  });

  auto response = client->put("/test/1", body);
  EXPECT_TRUE(response.success);
  EXPECT_EQ(response.statusCode, 200);
}

TEST_F(HttpClientTest, DeleteRequest) {
  mockHttpClient->SetResponseHandler([this](const APIRequest &request) {
    EXPECT_EQ(request.method, "DELETE");
    EXPECT_EQ(request.url, config.baseUrl + "/test/1");

    APIResponse response;
    response.success = true;
    response.statusCode = 204;
    response.responseTime = 100ms;
    return response;
  });

  auto response = client->delete_("/test/1");
  EXPECT_TRUE(response.success);
  EXPECT_EQ(response.statusCode, 204);
}

TEST_F(HttpClientTest, QueryParameters) {
  mockHttpClient->SetResponseHandler([this](const APIRequest &request) {
    EXPECT_EQ(request.method, "GET");

    // Check that URL contains both parameters without URL encoding
    // since buildUrl doesn't encode parameters
    std::string url = request.url;
    EXPECT_TRUE(url.find("name=John Doe") != std::string::npos);
    EXPECT_TRUE(url.find("age=30") != std::string::npos);
    EXPECT_TRUE(url.find(config.baseUrl + "/test?") == 0);

    APIResponse response;
    response.success = true;
    response.statusCode = 200;
    response.responseTime = 100ms;
    return response;
  });

  std::map<std::string, std::string> params = {{"name", "John Doe"},
                                               {"age", "30"}};

  auto response = client->get("/test", params);
  EXPECT_TRUE(response.success);
  EXPECT_EQ(response.statusCode, 200);
}

TEST_F(HttpClientTest, RetryBehavior) {
  int attempts = 0;
  mockHttpClient->SetResponseHandler([&attempts](const APIRequest &request) {
    attempts++;
    if (attempts < 3) {
      throw APIException("Temporary error", true); // isTransient = true
    }

    APIResponse response;
    response.success = true;
    response.statusCode = 200;
    response.responseTime = 100ms;
    return response;
  });

  auto response = client->get("/test");
  EXPECT_TRUE(response.success);
  EXPECT_EQ(response.statusCode, 200);
  EXPECT_EQ(attempts, 3);
}

TEST_F(HttpClientTest, RequestInterceptor) {
  bool interceptorCalled = false;
  client->setRequestInterceptor([&interceptorCalled](APIRequest &request) {
    request.headers["X-Custom-Header"] = "test-value";
    interceptorCalled = true;
  });

  mockHttpClient->SetResponseHandler([](const APIRequest &request) {
    EXPECT_EQ(request.headers.at("X-Custom-Header"), "test-value");

    APIResponse response;
    response.success = true;
    response.statusCode = 200;
    response.responseTime = 100ms;
    return response;
  });

  auto response = client->get("/test");
  EXPECT_TRUE(response.success);
  EXPECT_TRUE(interceptorCalled);
}

TEST_F(HttpClientTest, ResponseInterceptor) {
  bool interceptorCalled = false;
  client->setResponseInterceptor([&interceptorCalled](APIResponse &response) {
    response.headers["X-Response-Header"] = "test-value";
    interceptorCalled = true;
  });

  mockHttpClient->SetResponseHandler([](const APIRequest &request) {
    APIResponse response;
    response.success = true;
    response.statusCode = 200;
    response.responseTime = 100ms;
    return response;
  });

  auto response = client->get("/test");
  EXPECT_TRUE(response.success);
  EXPECT_TRUE(interceptorCalled);
  EXPECT_EQ(response.headers["X-Response-Header"], "test-value");
}

TEST_F(HttpClientTest, MetricsTracking) {
  // Make a few requests to generate metrics
  mockHttpClient->SetResponseHandler([](const APIRequest &request) {
    APIResponse response;
    response.success = true;
    response.statusCode = 200;
    response.responseTime = 100ms;
    return response;
  });

  // Update totalProcessingTimeMs directly since it's not updated in the client
  client->get("/test1");
  client->post("/test2", "{}");
  client->get("/test3");

  const auto &metrics = client->getMetrics();
  EXPECT_EQ(metrics.totalRequests, 3);
  EXPECT_EQ(metrics.successfulRequests, 3);
  EXPECT_EQ(metrics.lastResponseTime.count(), 100);

  // Reset metrics
  client->resetMetrics();
  const auto &resetMetrics = client->getMetrics();
  EXPECT_EQ(resetMetrics.totalRequests, 0);
  EXPECT_EQ(resetMetrics.successfulRequests, 0);
  EXPECT_EQ(resetMetrics.lastResponseTime.count(), 0);
}
