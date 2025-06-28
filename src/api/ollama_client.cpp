#include "api/client.h"
#include "api/types.h"

#include <chrono>
#include <fstream>
#include <stdexcept> // Required for std::runtime_error
#include <sstream>

namespace sep {
namespace ollama {

struct OllamaClient::Impl {
  OllamaConfig config;
  sep::api::Client client;

  Impl(const OllamaConfig &cfg)
      : config(cfg),
        client(sep::api::ClientConfig{cfg.host,
                            std::chrono::milliseconds(5000),
                            3,
                            true,
                            {}},
               std::make_unique<sep::api::CurlHttpClient>()) {}
};

OllamaClient::OllamaClient(const OllamaConfig &cfg)
    : impl_(std::make_unique<Impl>(cfg)) {}

OllamaClient::~OllamaClient() = default;
OllamaClient::OllamaClient(OllamaClient &&) noexcept = default;
OllamaClient &OllamaClient::operator=(OllamaClient &&) noexcept = default;

}  // namespace ollama

namespace ollama {

sep::SEPResult OllamaClient::get(const std::string &endpoint, std::string &response_out) {
  if (impl_->config.host.rfind("file://", 0) == 0) {
    std::string path = impl_->config.host.substr(7) + endpoint;
    std::ifstream file(path);
    if (!file) {
      return sep::SEPResult::INVALID_ARGUMENT;
    }
    std::ostringstream ss;
    ss << file.rdbuf();
    response_out = ss.str();
    return sep::SEPResult::SUCCESS;
  }
  sep::api::APIResponse resp = impl_->client.get(endpoint);
  if (!resp.success) {
    response_out.clear();
    if (resp.error.code == sep::api::ErrorCode::ApiError) {
      return sep::SEPResult::PROCESSING_ERROR;
    }
    return sep::SEPResult::UNKNOWN_ERROR;
  }
  response_out = resp.body;
  return sep::SEPResult::SUCCESS;
}

sep::SEPResult OllamaClient::post(const std::string &endpoint, const nlohmann::json &payload,
                             std::string &response_out) {
  if (impl_->config.host.rfind("file://", 0) == 0) {
    return sep::SEPResult::INVALID_ARGUMENT;
  }
  sep::api::APIResponse resp = impl_->client.post(endpoint, payload.dump());
  if (!resp.success) {
    response_out.clear();
    if (resp.error.code == sep::api::ErrorCode::ApiError) {
      return sep::SEPResult::PROCESSING_ERROR;
    }
    return sep::SEPResult::UNKNOWN_ERROR;
  }
  response_out = resp.body;
  return sep::SEPResult::SUCCESS;
}

GenerateResponse OllamaClient::generate(const GenerateRequest &request) {
  nlohmann::json payload{{"model", request.model}, {"prompt", request.prompt}};
  std::string result;
  sep::SEPResult res = post("/api/generate", payload, result);
  if (res != sep::SEPResult::SUCCESS) {
    return {};
  }
  nlohmann::json json_result = nlohmann::json::parse(result);
  GenerateResponse resp;
  resp.response = json_result.value("response", "");
  resp.done = json_result.value("done", false);
  resp.model = json_result.value("model", "");
  return resp;
}

EmbeddingResponse OllamaClient::getEmbedding(
    const EmbeddingRequest &request) {
  nlohmann::json payload{{"model", request.model}, {"prompt", request.prompt}};
  std::string result;
  sep::SEPResult res = post("/api/embeddings", payload, result);
  if (res != sep::SEPResult::SUCCESS) {
    return {};
  }
  nlohmann::json json_result = nlohmann::json::parse(result);
  EmbeddingResponse resp;
  if (json_result.contains("embedding") && json_result["embedding"].is_array()) {
    resp.embedding = json_result["embedding"].get<std::vector<float>>();
  }
  return resp;
};
}  // namespace ollama
}  // namespace sep
