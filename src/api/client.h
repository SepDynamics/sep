#ifndef SEP_API_CLIENT_H
#define SEP_API_CLIENT_H

#include <chrono>
#include <condition_variable>
#include <functional>
#include <map>
#include <memory>
#include <mutex>
#include <nlohmann/json.hpp>
#include <string>

#include "api/types.h"
#include "curl/curl.h"
#include "engine/internal/common.h"

namespace sep {
namespace api {

// Forward declare
class IHttpClient;

struct ClientConfig {
  std::string baseUrl;
  std::chrono::milliseconds defaultTimeout{5000};
  size_t maxRetries{3};
  bool enableMetrics{true};
  std::map<std::string, std::string> defaultHeaders;
};

class IHttpClient {
 public:
  virtual ~IHttpClient() = default;
  virtual APIResponse send_request(const APIRequest &request) = 0;
};

class CurlHttpClient : public IHttpClient {
 public:
  CurlHttpClient();
  ~CurlHttpClient() override;

  // Prevent copying
  CurlHttpClient(const CurlHttpClient&) = delete;
  CurlHttpClient& operator=(const CurlHttpClient&) = delete;

  // Allow moving
  CurlHttpClient(CurlHttpClient&&) = default;
  CurlHttpClient& operator=(CurlHttpClient&&) = default;

  APIResponse send_request(const APIRequest& request) override;

 private:
  static size_t write_callback(void* contents, size_t size, size_t nmemb, void* userp);
};

class Client {
 public:
  Client(const ClientConfig &config, std::unique_ptr<IHttpClient> httpClient);
  explicit Client(const ClientConfig &config);
  ~Client() = default;

  Client(const Client &) = delete;
  Client &operator=(const Client &) = delete;
  Client(Client &&) noexcept = default;
  Client &operator=(Client &&) noexcept = default;

  APIResponse send(const APIRequest &request);

  APIResponse get(const std::string &endpoint,
                  const std::map<std::string, std::string> &queryParams = {},
                  Priority priority = Priority::NORMAL);

  APIResponse post(const std::string &endpoint, const std::string &body,
                   Priority priority = Priority::NORMAL);

  APIResponse put(const std::string &endpoint, const std::string &body,
                  Priority priority = Priority::NORMAL);

  APIResponse delete_(const std::string &endpoint, Priority priority = Priority::NORMAL);

  void setRequestInterceptor(std::function<void(APIRequest &)> interceptor);
  void setResponseInterceptor(std::function<void(APIResponse &)> interceptor);
  const HealthMetrics &getMetrics() const;
  void resetMetrics();
  const ClientConfig &getConfig() const;
  std::string getLastRequestId() const;

 private:
  class Impl {
   public:
    Impl(const ClientConfig &config, std::unique_ptr<IHttpClient> httpClient);
    ~Impl();

    ClientConfig config;
    std::unique_ptr<IHttpClient> httpClient;
    std::function<void(APIRequest &)> requestInterceptor;
    std::function<void(APIResponse &)> responseInterceptor;
    HealthMetrics metrics;
    mutable std::mutex mutex;
    std::string lastRequestId;
  };

  std::unique_ptr<Impl> impl_;
  APIResponse sendWithRetry(const APIRequest &request);
  void updateMetrics(const APIRequest &request, const APIResponse &response);
  std::string buildUrl(const std::string &endpoint,
                       const std::map<std::string, std::string> &queryParams = {});
};

std::shared_ptr<Client> createClient(const ClientConfig &config);

} // namespace api
namespace ollama { 

class OllamaClient {
public:
  explicit OllamaClient(const sep::ollama::OllamaConfig &config);
  ~OllamaClient();

  // Prevent copying
  OllamaClient(const OllamaClient &) = delete;
  OllamaClient &operator=(const OllamaClient &) = delete;

  // Allow moving
  OllamaClient(OllamaClient &&) noexcept;
  OllamaClient &operator=(OllamaClient &&) noexcept;

  // API methods
  SEPResult post(const std::string &endpoint, const nlohmann::json &payload,
                 std::string &response_out);
  SEPResult get(const std::string &endpoint, std::string &response_out);
  sep::ollama::GenerateResponse
  generate(const sep::ollama::GenerateRequest &request);
  
  sep::ollama::EmbeddingResponse
  getEmbedding(const sep::ollama::EmbeddingRequest &request);

private:
  struct Impl;
  std::unique_ptr<Impl> impl_;
};

}  // namespace ollama
}  // namespace sep

#endif  // SEP_API_CLIENT_H
