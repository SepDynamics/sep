#include "api/client.h"
#include "curl/curl.h"
#include <stdexcept>
#include <utility>


#include "engine/error_handler.h"  // For sep::ErrorCode

namespace sep::api {

Client::Impl::Impl(const ClientConfig &config, std::unique_ptr<IHttpClient> httpClient)
    : config(config), httpClient(std::move(httpClient)) {
  // httpClient can be null, send operations will fail gracefully.
}

Client::Impl::~Impl() = default;

Client::Client(const ClientConfig &config, std::unique_ptr<IHttpClient> httpClient)
    : impl_(std::make_unique<Impl>(config, std::move(httpClient))) {}

Client::Client(const ClientConfig &config)
    : impl_(std::make_unique<Impl>(config, std::make_unique<CurlHttpClient>())) {}


APIResponse Client::send(const APIRequest &request) {
  APIResponse response = sendWithRetry(request);
  updateMetrics(request, response);
  return response;
}

APIResponse Client::get(const std::string &endpoint,
                        const std::map<std::string, std::string> &queryParams, Priority priority) {
  APIRequest request;
  request.method = "GET";
  request.url = buildUrl(endpoint, queryParams);
  request.headers = impl_->config.defaultHeaders;
  request.priority = priority;
  return send(request);
}

APIResponse Client::post(const std::string &endpoint, const std::string &body, Priority priority) {
  APIRequest request;
  request.method = "POST";
  request.url = buildUrl(endpoint);
  request.headers = impl_->config.defaultHeaders;
  request.body = body;
  request.priority = priority;
  return send(request);
}

APIResponse Client::put(const std::string &endpoint, const std::string &body, Priority priority) {
  APIRequest request;
  request.method = "PUT";
  request.url = buildUrl(endpoint);
  request.headers = impl_->config.defaultHeaders;
  request.body = body;
  request.priority = priority;
  return send(request);
}

APIResponse Client::delete_(const std::string &endpoint, Priority priority) {
  APIRequest request;
  request.method = "DELETE";
  request.url = buildUrl(endpoint);
  request.headers = impl_->config.defaultHeaders;
  request.priority = priority;
  return send(request);
}

void Client::setRequestInterceptor(std::function<void(APIRequest &)> interceptor) {
  std::lock_guard<std::mutex> lock(impl_->mutex);
  impl_->requestInterceptor = interceptor;
}

void Client::setResponseInterceptor(std::function<void(APIResponse &)> interceptor) {
  std::lock_guard<std::mutex> lock(impl_->mutex);
  impl_->responseInterceptor = interceptor;
}

const HealthMetrics &Client::getMetrics() const { return impl_->metrics; }

void Client::resetMetrics() {
  std::lock_guard<std::mutex> lock(impl_->mutex);
  impl_->metrics.totalRequests.store(0);
  impl_->metrics.successfulRequests.store(0);
  impl_->metrics.failedRequests.store(0);
  impl_->metrics.rateLimitedCount.store(0);
  impl_->metrics.lastResponseTime = std::chrono::milliseconds(0);
  impl_->metrics.lastSuccessTime = {};
  impl_->metrics.lastErrorTime = {};
  impl_->metrics.lastErrorCode = 0;
  impl_->metrics.startTime = std::chrono::steady_clock::now();
}

const ClientConfig &Client::getConfig() const { return impl_->config; }

std::string Client::getLastRequestId() const {
  std::lock_guard<std::mutex> lock(impl_->mutex);
  return impl_->lastRequestId;
}

APIResponse Client::sendWithRetry(const APIRequest &request) {
  APIResponse response;
  size_t attempts = 0;
  const size_t maxRetries = impl_->config.maxRetries;

  if (!impl_->httpClient) {
    response.success = false;
    response.error.code = sep::api::ErrorCode::ApiError;
    response.error.message = "HTTP client not initialized";
    return response;
  }

  while (attempts <= maxRetries) {
    if (impl_->requestInterceptor) {
      APIRequest modifiedRequest = request;
      impl_->requestInterceptor(modifiedRequest);
      response = impl_->httpClient->send_request(modifiedRequest);
    } else {
      response = impl_->httpClient->send_request(request);
    }

    if (impl_->responseInterceptor) {
      impl_->responseInterceptor(response);
    }

    std::lock_guard<std::mutex> lock(impl_->mutex);
    impl_->lastRequestId = request.requestId;
    if (response.success || attempts == maxRetries) {
      return response;
    }
    attempts++;
  }

  response.success = false;
  response.error.code = sep::api::ErrorCode::ApiError;
  response.error.message = "Max retries exceeded";
  return response;
}

void Client::updateMetrics(const APIRequest &request,
                           const APIResponse &response) {
  std::lock_guard<std::mutex> lock(impl_->mutex);
  impl_->metrics.totalRequests++;
  impl_->metrics.lastResponseTime = response.responseTime;

  if (response.success) {
    impl_->metrics.successfulRequests++;
    impl_->metrics.lastSuccessTime = std::chrono::system_clock::now();
  } else {
    if (response.statusCode == 429) {
      impl_->metrics.rateLimitedCount++;
    }
    impl_->metrics.failedRequests++;
    impl_->metrics.lastErrorCode = response.statusCode;
    impl_->metrics.lastErrorTime = std::chrono::system_clock::now();
  }
}

std::string Client::buildUrl(const std::string &endpoint,
                             const std::map<std::string, std::string> &queryParams) {
  std::string url = impl_->config.baseUrl + endpoint;
  if (!queryParams.empty()) {
    url += "?";
    bool first = true;
    for (const auto &param : queryParams) {
      if (!first) url += "&";
      url += param.first + "=" + param.second;
      first = false;
    }
  }
  return url;
}

std::shared_ptr<Client> createClient(const ClientConfig &config) {
  return std::make_shared<Client>(config);
}

}  // namespace sep::api
