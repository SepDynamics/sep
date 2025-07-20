#include <string.h>
#include <cstring> // For std::memcpy, std::strcmp etc.
#include <ctime>
#include <time.h>   // For CLOCK_MONOTONIC
#include <unistd.h> // For nanosleep
#include <cstdlib>

// Standard library includes
#include <chrono>
#include <csignal>
#include <cstdio>
#include <memory>
#include <string>
#include <thread>

// Compatibility layer includes
#include "engine/standard_includes.h"

// Third-party includes
#include <nlohmann/json.hpp>
#include <spdlog/spdlog.h>

// Project includes
#include "api/crow_request.h"
#include "api/json_helpers.h"
#include "api/ollama_client.h"
#include "api/rate_limit_middleware.h"
#include "api/request_interface.h"
#include "api/sep_engine.h"
#include "api/server.h"
#include "api/types.h"
#include "crow/crow_isolation.h"
#include "engine/common.h"
#include "engine/cuda_unified.h"
#include "engine/dag_graph.h"
#include "engine/data_parser.h"
#include "engine/logging.h"
#include "engine/types.h"

namespace sep {
namespace api {

// Define static instance
SEPApiServer* SEPApiServer::instance_ = nullptr;


SEPApiServer::~SEPApiServer() {
  if (running_.load()) {
    stop();
  }
  instance_ = nullptr;
}

std::mutex &SEPApiServer::getMetricsMutex() {
  return metrics_mutex_;
}

ServerMetrics &SEPApiServer::getModifiableMetrics() {
  return server_metrics_;
}

void SEPApiServer::start() {
    if (!app_) return;

    app_->port(config_.port);
    app_->multithreaded();

    setup_middleware();
    setup_routes();
}

bool SEPApiServer::run() {
  if (!app_) return false;

  // Ensure routes are registered just before starting
  setup_routes();
  start();

  // Start server in a new thread
  server_thread_ = std::make_unique<std::thread>([this]() {
    running_ = true;
    app_->run();
  });

  return true;
}


void SEPApiServer::stop() {
  if (!running_) {
    return;
  }

  if (logger_)
    logger_->info("Stopping SEP API Server");
  running_ = false;

  if (app_) {
    // Stop the Crow app first
    app_->stop();
    
    // Wait for server thread to finish
    if (server_thread_ && server_thread_->joinable()) {
      if (logger_)
        logger_->debug("Waiting for server thread to join...");
      server_thread_->join();
      server_thread_.reset();
    }
  }

  if (logger_)
    logger_->info("SEP API Server stopped");
}

void SEPApiServer::waitForShutdown() {
  if (server_thread_ && server_thread_->joinable()) {
    server_thread_->join();
  }
}

std::unique_ptr<HttpResponse> SEPApiServer::makeJsonResponse(int code, const std::string& message)
{
    // This is a simplified implementation
    class SimpleHttpResponse : public HttpResponse
    {
    public:
        SimpleHttpResponse(int code, const std::string& body) : code_(code), body_(body) {}
        void setCode(int code) override { code_ = code; }
        void setBody(const std::string& body) override { body_ = body; }
        void end() override {}
        int getCode() const override { return code_; }
        std::string getBody() const override { return body_; }

    private:
        int code_;
        std::string body_;
    };

    nlohmann::json response;
    response["status"] = (code >= 200 && code < 300) ? "success" : "error";
    response["message"] = message;
    response["code"] = code;

    return std::make_unique<SimpleHttpResponse>(code, response.dump());
}

std::string SEPApiServer::handleError(const std::string& message, int code)
{
    nlohmann::json error_response{};
    error_response["error"] = true;
    error_response["message"] = message;
    error_response["code"] = code;
    error_response["timestamp"] = std::chrono::duration_cast<std::chrono::seconds>(
                                      std::chrono::system_clock::now().time_since_epoch())
                                      .count();

    if (logger_) logger_->error("API Error [{}]: {}", code, message);

    std::lock_guard<std::mutex> lock(metrics_mutex_);
    metrics_.failedRequests++;
    metrics_.lastErrorCode = code;
    metrics_.lastErrorTime = std::chrono::system_clock::now();

    return error_response.dump();
}

void SEPApiServer::logRequest(const HttpRequest& req, int code, const std::string& response_body,
                              int64_t duration)
{
    (void)response_body; // Silence unused parameter warning
    if (!logger_) return;
    std::lock_guard<std::mutex> lock(metrics_mutex_);

  metrics_.totalRequests++;
  if (code >= 200 && code < 300) {
    metrics_.successfulRequests++;
    metrics_.lastSuccessTime = std::chrono::system_clock::now();
  } else {
    metrics_.failedRequests++;
    metrics_.lastErrorCode = code;
    metrics_.lastErrorTime = std::chrono::system_clock::now();
  }

  // Update average response time
  double current_avg = metrics_.averageResponseTime.load();
  double new_avg = (current_avg * (metrics_.totalRequests - 1) + duration) / metrics_.totalRequests;
  metrics_.averageResponseTime = new_avg;

  metrics_.lastResponseTime = std::chrono::milliseconds(duration);

  if (logger_)
    logger_->info("Request: {} {} - Status: {} - Duration: {}ms",
                  req.method(), req.url(), code, duration);
}

std::string SEPApiServer::getErrorResponse(const std::string& message, int status)
{
    return handleError(message, status);
}

crow::response SEPApiServer::makeCrowJsonResponse(int status_code, const nlohmann::json& data)
{
    crow::response res(status_code);
    res.set_header("Content-Type", "application/json");
    res.body = data.dump();
    return res;
}

nlohmann::json SEPApiServer::handleCrowError(const std::string& message, int status_code)
{
    nlohmann::json error_json{{"error", true},
                              {"message", message},
                              {"code", status_code},
                              {"timestamp", std::chrono::duration_cast<std::chrono::seconds>(
                                                std::chrono::system_clock::now().time_since_epoch())
                                                .count()}};

    if (logger_) logger_->error("API Error [{}]: {}", status_code, message);

    std::lock_guard<std::mutex> lock(metrics_mutex_);
    metrics_.failedRequests++;
    metrics_.lastErrorCode = status_code;
    metrics_.lastErrorTime = std::chrono::system_clock::now();

    return error_json;
}

void SEPApiServer::logRequest(const crow::request& req, int status_code,
                              const std::string& response_body, int64_t duration_ms)
{
    (void)response_body; // Silence unused parameter warning
    if (!logger_) return;
    std::lock_guard<std::mutex> lock(metrics_mutex_);

  metrics_.totalRequests++;
  if (status_code >= 200 && status_code < 300) {
    metrics_.successfulRequests++;
    metrics_.lastSuccessTime = std::chrono::system_clock::now();
  } else {
    metrics_.failedRequests++;
    metrics_.lastErrorCode = status_code;
    metrics_.lastErrorTime = std::chrono::system_clock::now();
  }

  // Update average response time
  double current_avg = metrics_.averageResponseTime.load();
  double new_avg =
      (current_avg * (metrics_.totalRequests - 1) + duration_ms) / metrics_.totalRequests;
  metrics_.averageResponseTime = new_avg;

  metrics_.lastResponseTime = std::chrono::milliseconds(duration_ms);

  std::string method_name = std::string(crow::method_name(req.method));
  std::string url = std::string(req.url);

  if (logger_)
    logger_->info("Request: {} {} - Status: {} - Duration: {}ms",
                  method_name, url, status_code, duration_ms);
}

void SEPApiServer::setup_logging() {
#if SEP_HAS_EXCEPTIONS
  try {
#endif
    ::sep::logging::Manager::initialize();
    logger_ = spdlog::get("sep_api");
    if (!logger_) {
      logger_ = spdlog::default_logger();
    }

    // Set log level based on config
    if (config_.log_level == "debug") {
      logger_->set_level(spdlog::level::debug);
    } else if (config_.log_level == "info") {
      logger_->set_level(spdlog::level::info);
    } else if (config_.log_level == "warn") {
      logger_->set_level(spdlog::level::warn);
    } else if (config_.log_level == "error") {
      logger_->set_level(spdlog::level::err);
    }

    logger_->info("Logging initialized for SEP API Server");
#if SEP_HAS_EXCEPTIONS
  } catch (const std::exception& e) {
    (void)fprintf(stderr,
                  "Failed to initialize logging: %s\n",
                  e.what());
  }
#endif
}

void SEPApiServer::setup_middleware() {
  if (!app_) return;

  // Configure rate limiting middleware
  auto& rate_limit_mw = app_->get_middleware<RateLimitMiddleware>();
  (void)rate_limit_mw; // Middleware is used implicitly through registration

  // Configure auth middleware
  auto& auth_mw = app_->get_middleware<AuthMiddleware>();
  // Set auth tokens if configured
  if (config_.cors.enabled && !config_.cors.tokens.empty()) {
    auth_mw.set_tokens(config_.cors.tokens);
  }
}

void SEPApiServer::setup_routes() {
  if (!app_) return;

  auto& engine = SepEngine::getInstance();

  // Health check endpoint
  app_->route_dynamic("/api/v1/health")
      .methods(crow::HTTPMethod::GET)([this, &engine](const crow::request& req) {
          auto start_time = std::chrono::steady_clock::now();

#if SEP_HAS_EXCEPTIONS
          try
          {
#endif
              auto health_data = engine.getHealthStatus();
              auto end_time = std::chrono::steady_clock::now();
              auto duration =
                  std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time)
                      .count();

              logRequest(req, HTTP_OK, health_data.dump(), duration);
              return makeCrowJsonResponse(HTTP_OK, health_data);

    #if SEP_HAS_EXCEPTIONS
          }
          catch (const std::exception& e)
          {
              auto end_time = std::chrono::steady_clock::now();
              auto duration =
                  std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time)
                      .count();

              auto error_crow = handleCrowError("Health check failed: " + std::string(e.what()),
                                                HTTP_INTERNAL_ERROR);
              logRequest(req, HTTP_INTERNAL_ERROR, error_crow.dump(), duration);
              return makeCrowJsonResponse(HTTP_INTERNAL_ERROR, error_crow);
          }
#endif
      });

  // Process patterns endpoint
  app_->route_dynamic("/api/v1/pattern/evolve")
      .methods(crow::HTTPMethod::POST)([this, &engine](const crow::request& req) {
          auto start_time = std::chrono::steady_clock::now();

#if SEP_HAS_EXCEPTIONS
          try
          {
#endif
              // Parse request body
              nlohmann::json request_data = parse_json(std::string(req.body));

              // Process patterns through SEP engine
              auto result = engine.processPatterns(request_data);
              auto response_data = applyCoherenceModulation(result);

              auto end_time = std::chrono::steady_clock::now();
              auto duration =
                  std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time)
                      .count();

              logRequest(req, HTTP_OK, response_data.dump(), duration);
              return makeCrowJsonResponse(HTTP_OK, response_data);

        #if SEP_HAS_EXCEPTIONS
          } catch (const nlohmann::json::parse_error& e)
          {
              auto end_time = std::chrono::steady_clock::now();
              auto duration =
                  std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time)
                      .count();

              auto error_crow =
                  handleCrowError("Invalid JSON: " + std::string(e.what()), HTTP_BAD_REQUEST);
              logRequest(req, HTTP_BAD_REQUEST, error_crow.dump(), duration);
              return makeCrowJsonResponse(HTTP_BAD_REQUEST, error_crow);
          } catch (const std::exception& e)
          {
              auto end_time = std::chrono::steady_clock::now();
              auto duration =
                  std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time)
                      .count();

              auto error_crow = handleCrowError(
                  "Pattern processing failed: " + std::string(e.what()), HTTP_INTERNAL_ERROR);
              logRequest(req, HTTP_INTERNAL_ERROR, error_crow.dump(), duration);
              return makeCrowJsonResponse(HTTP_INTERNAL_ERROR, error_crow);
          }
        #endif
      });

  // Process batch endpoint
  app_->route_dynamic("/api/v1/memory/query")
      .methods(crow::HTTPMethod::POST)([this, &engine](const crow::request& req) {
          auto start_time = std::chrono::steady_clock::now();

#if SEP_HAS_EXCEPTIONS
          try
          {
          
#endif
              nlohmann::json request_data = parse_json(std::string(req.body));
              auto result = engine.processBatch(request_data);
              auto response_data = applyCoherenceModulation(result);

              auto end_time = std::chrono::steady_clock::now();
              auto duration =
                  std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time)
                      .count();

              logRequest(req, HTTP_OK, response_data.dump(), duration);
              return makeCrowJsonResponse(HTTP_OK, response_data);

        #if SEP_HAS_EXCEPTIONS
          } catch (const nlohmann::json::parse_error& e)
          {
              auto end_time = std::chrono::steady_clock::now();
              auto duration =
                  std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time)
                      .count();

              auto error_crow =
                  handleCrowError("Invalid JSON: " + std::string(e.what()), HTTP_BAD_REQUEST);
              logRequest(req, HTTP_BAD_REQUEST, error_crow.dump(), duration);
              return makeCrowJsonResponse(HTTP_BAD_REQUEST, error_crow);
          } catch (const std::exception& e)
          {
              auto end_time = std::chrono::steady_clock::now();
              auto duration =
                  std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time)
                      .count();

              auto error_crow = handleCrowError("Batch processing failed: " + std::string(e.what()),
                                                HTTP_INTERNAL_ERROR);
              logRequest(req, HTTP_INTERNAL_ERROR, error_crow.dump(), duration);
              return makeCrowJsonResponse(HTTP_INTERNAL_ERROR, error_crow);
          }
        #endif
      });

  // Pattern history endpoint
  app_->route_dynamic("/api/v1/patterns/history")
      .methods(crow::HTTPMethod::POST)([this, &engine](const crow::request& req) {
          auto start_time = std::chrono::steady_clock::now();

#if SEP_HAS_EXCEPTIONS
          try
          {
          
#endif
              nlohmann::json request_data = parse_json(std::string(req.body));
              auto result = engine.getPatternHistory(request_data);
              auto response_data = applyCoherenceModulation(result);

              auto end_time = std::chrono::steady_clock::now();
              auto duration =
                  std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time)
                      .count();

              logRequest(req, HTTP_OK, response_data.dump(), duration);
              return makeCrowJsonResponse(HTTP_OK, response_data);

        #if SEP_HAS_EXCEPTIONS
          } catch (const nlohmann::json::parse_error& e)
          {
              auto end_time = std::chrono::steady_clock::now();
              auto duration =
                  std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time)
                      .count();

              auto error_crow =
                  handleCrowError("Invalid JSON: " + std::string(e.what()), HTTP_BAD_REQUEST);
              logRequest(req, HTTP_BAD_REQUEST, error_crow.dump(), duration);
              return makeCrowJsonResponse(HTTP_BAD_REQUEST, error_crow);
          } catch (const std::exception& e)
          {
              auto end_time = std::chrono::steady_clock::now();
              auto duration =
                  std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time)
                      .count();

              auto error_crow = handleCrowError("Pattern history failed: " + std::string(e.what()),
                                                HTTP_INTERNAL_ERROR);
              logRequest(req, HTTP_INTERNAL_ERROR, error_crow.dump(), duration);
              return makeCrowJsonResponse(HTTP_INTERNAL_ERROR, error_crow);
          }
        #endif
      });

  // Streaming data endpoint for real-time quant data
  app_->route_dynamic("/api/v1/data/stream")
      .methods(crow::HTTPMethod::POST)([this](const crow::request& req) {
          auto start_time = std::chrono::steady_clock::now();

#if SEP_HAS_EXCEPTIONS
          try
          {
#endif
              // Parse request body - expects raw data or base64 encoded binary
              nlohmann::json request_data = parse_json(std::string(req.body));

              std::string data_format = request_data.value("format", "auto");
              std::string data_content;
              std::vector<uint8_t> binary_data;

              // Check if data is provided as string or binary
              if (request_data.contains("data")) {
                  data_content = request_data["data"].get<std::string>();
              } else if (request_data.contains("binary")) {
                  // Base64 decode if binary data is provided
                  std::string base64_data = request_data["binary"].get<std::string>();
                  // Simple base64 decode (you may want to use a proper base64 library)
                  binary_data.resize(base64_data.length() * 3 / 4);
                  // TODO: Implement proper base64 decoding
              }
              
              // Create data parser
              sep::DataParser parser;
              std::vector<sep::Pattern> patterns;

              // Parse based on content type
              if (!binary_data.empty()) {
                  sep::DataFormat format = sep::DataFormat::AUTO;
                  if (data_format == "binary") format = sep::DataFormat::BINARY;
                  else if (data_format == "json") format = sep::DataFormat::JSON;
                  else if (data_format == "csv") format = sep::DataFormat::CSV;
                  
                  patterns = parser.parseBuffer(binary_data.data(), binary_data.size(), format);
              } else if (!data_content.empty()) {
                  // Parse as string data
                  std::istringstream stream(data_content);
                  patterns = parser.parseStream(stream);
              }
              
              // Process patterns
              nlohmann::json response;
              response["patterns_received"] = patterns.size();
              response["status"] = "processed";
              
              if (!patterns.empty()) {
                  // Calculate basic metrics
                  float avg_coherence = 0.0f;
                  for (const auto& pattern : patterns) {
                      avg_coherence += pattern.coherence;
                  }
                  avg_coherence /= patterns.size();
                  
                  response["metrics"]["average_coherence"] = avg_coherence;
                  response["metrics"]["pattern_count"] = patterns.size();
              }
              
              auto end_time = std::chrono::steady_clock::now();
              auto duration =
                  std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time)
                      .count();

              logRequest(req, HTTP_OK, response.dump(), duration);
              return makeCrowJsonResponse(HTTP_OK, response);

        #if SEP_HAS_EXCEPTIONS
          } catch (const nlohmann::json::parse_error& e)
          {
              auto end_time = std::chrono::steady_clock::now();
              auto duration =
                  std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time)
                      .count();

              auto error_crow =
                  handleCrowError("Invalid JSON: " + std::string(e.what()), HTTP_BAD_REQUEST);
              logRequest(req, HTTP_BAD_REQUEST, error_crow.dump(), duration);
              return makeCrowJsonResponse(HTTP_BAD_REQUEST, error_crow);
          } catch (const std::exception& e)
          {
              auto end_time = std::chrono::steady_clock::now();
              auto duration =
                  std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time)
                      .count();

              auto error_crow = handleCrowError(
                  "Stream processing failed: " + std::string(e.what()), HTTP_INTERNAL_ERROR);
              logRequest(req, HTTP_INTERNAL_ERROR, error_crow.dump(), duration);
              return makeCrowJsonResponse(HTTP_INTERNAL_ERROR, error_crow);
          }
        #endif
      });

  // Quant data processing endpoint with full DAG analysis
  app_->route_dynamic("/api/v1/quant/process")
      .methods(crow::HTTPMethod::POST)([this, &engine](const crow::request& req) {
          auto start_time = std::chrono::steady_clock::now();

#if SEP_HAS_EXCEPTIONS
          try
          {
#endif
              nlohmann::json request_data = parse_json(std::string(req.body));

              // Extract candle data or file path
              nlohmann::json result;
              
              if (request_data.contains("file_path")) {
                  // Process from file
                  std::string file_path = request_data["file_path"].get<std::string>();
                  result = engine.processQuantData(file_path);
              } else if (request_data.contains("candles")) {
                  // Process inline candle data
                  // Save to temporary file for processing
                  std::string temp_file =
                      "/tmp/quant_data_" + std::to_string(std::time(nullptr)) + ".json";
                  std::ofstream out(temp_file);
                  out << request_data["candles"].dump();
                  out.close();
                  
                  result = engine.processQuantData(temp_file);
                  
                  // Clean up temp file
                  (void)std::remove(temp_file.c_str());
              } else {
                  throw std::runtime_error("Missing 'file_path' or 'candles' in request");
              }
              
              auto end_time = std::chrono::steady_clock::now();
              auto duration =
                  std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time)
                      .count();

              logRequest(req, HTTP_OK, result.dump(), duration);
              return makeCrowJsonResponse(HTTP_OK, result);

        #if SEP_HAS_EXCEPTIONS
          } catch (const nlohmann::json::parse_error& e)
          {
              auto end_time = std::chrono::steady_clock::now();
              auto duration =
                  std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time)
                      .count();

              auto error_crow =
                  handleCrowError("Invalid JSON: " + std::string(e.what()), HTTP_BAD_REQUEST);
              logRequest(req, HTTP_BAD_REQUEST, error_crow.dump(), duration);
              return makeCrowJsonResponse(HTTP_BAD_REQUEST, error_crow);
          } catch (const std::exception& e)
          {
              auto end_time = std::chrono::steady_clock::now();
              auto duration =
                  std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time)
                      .count();

              auto error_crow = handleCrowError("Quant processing failed: " + std::string(e.what()),
                                                HTTP_INTERNAL_ERROR);
              logRequest(req, HTTP_INTERNAL_ERROR, error_crow.dump(), duration);
              return makeCrowJsonResponse(HTTP_INTERNAL_ERROR, error_crow);
          }
        #endif
      });

  // Validate contexts endpoint
  app_->route_dynamic("/api/v1/context/process")
      .methods(crow::HTTPMethod::POST)([this, &engine](const crow::request& req) {
          auto start_time = std::chrono::steady_clock::now();

#if SEP_HAS_EXCEPTIONS
          try
          {
          
#endif
              nlohmann::json request_data = parse_json(std::string(req.body));
              auto result = engine.validateContexts(request_data);
              auto response_data = applyCoherenceModulation(result);

              auto end_time = std::chrono::steady_clock::now();
              auto duration =
                  std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time)
                      .count();

              logRequest(req, HTTP_OK, response_data.dump(), duration);
              return makeCrowJsonResponse(HTTP_OK, response_data);

        #if SEP_HAS_EXCEPTIONS
          } catch (const nlohmann::json::parse_error& e)
          {
              auto end_time = std::chrono::steady_clock::now();
              auto duration =
                  std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time)
                      .count();

              auto error_crow =
                  handleCrowError("Invalid JSON: " + std::string(e.what()), HTTP_BAD_REQUEST);
              logRequest(req, HTTP_BAD_REQUEST, error_crow.dump(), duration);
              return makeCrowJsonResponse(HTTP_BAD_REQUEST, error_crow);
          } catch (const std::exception& e)
          {
              auto end_time = std::chrono::steady_clock::now();
              auto duration =
                  std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time)
                      .count();

              auto error_crow = handleCrowError(
                  "Context validation failed: " + std::string(e.what()), HTTP_INTERNAL_ERROR);
              logRequest(req, HTTP_INTERNAL_ERROR, error_crow.dump(), duration);
              return makeCrowJsonResponse(HTTP_INTERNAL_ERROR, error_crow);
          }
        #endif
      });

  // Extract embeddings endpoint
  app_->route_dynamic("/api/v1/embeddings/extract")
      .methods(crow::HTTPMethod::POST)([this, &engine](const crow::request& req) {
          auto start_time = std::chrono::steady_clock::now();

#if SEP_HAS_EXCEPTIONS
          try
          {
          
#endif
              nlohmann::json request_data = parse_json(std::string(req.body));
              auto result = engine.extractEmbeddings(request_data);
              auto response_data = applyCoherenceModulation(result);

              auto end_time = std::chrono::steady_clock::now();
              auto duration =
                  std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time)
                      .count();

              logRequest(req, HTTP_OK, response_data.dump(), duration);
              return makeCrowJsonResponse(HTTP_OK, response_data);

        #if SEP_HAS_EXCEPTIONS
          } catch (const nlohmann::json::parse_error& e)
          {
              auto end_time = std::chrono::steady_clock::now();
              auto duration =
                  std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time)
                      .count();

              auto error_crow =
                  handleCrowError("Invalid JSON: " + std::string(e.what()), HTTP_BAD_REQUEST);
              logRequest(req, HTTP_BAD_REQUEST, error_crow.dump(), duration);
              return makeCrowJsonResponse(HTTP_BAD_REQUEST, error_crow);
          } catch (const std::exception& e)
          {
              auto end_time = std::chrono::steady_clock::now();
              auto duration =
                  std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time)
                      .count();

              auto error_crow = handleCrowError(
                  "Embedding extraction failed: " + std::string(e.what()), HTTP_INTERNAL_ERROR);
              logRequest(req, HTTP_INTERNAL_ERROR, error_crow.dump(), duration);
              return makeCrowJsonResponse(HTTP_INTERNAL_ERROR, error_crow);
          }
        #endif
      });

  // Analyze patterns endpoint
  app_->route_dynamic("/api/v1/pattern/analyze")
      .methods(crow::HTTPMethod::POST)([this, &engine](const crow::request& req) {
          auto start_time = std::chrono::steady_clock::now();

#if SEP_HAS_EXCEPTIONS
          try
          {
          
#endif
              nlohmann::json request_data = parse_json(std::string(req.body));
              auto result = engine.calculateSimilarity(request_data);
              auto response_data = applyCoherenceModulation(result);

              auto end_time = std::chrono::steady_clock::now();
              auto duration =
                  std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time)
                      .count();

              logRequest(req, HTTP_OK, response_data.dump(), duration);
              return makeCrowJsonResponse(HTTP_OK, response_data);

        #if SEP_HAS_EXCEPTIONS
          } catch (const nlohmann::json::parse_error& e)
          {
              auto end_time = std::chrono::steady_clock::now();
              auto duration =
                  std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time)
                      .count();

              auto error_crow =
                  handleCrowError("Invalid JSON: " + std::string(e.what()), HTTP_BAD_REQUEST);
              logRequest(req, HTTP_BAD_REQUEST, error_crow.dump(), duration);
              return makeCrowJsonResponse(HTTP_BAD_REQUEST, error_crow);
          } catch (const std::exception& e)
          {
              auto end_time = std::chrono::steady_clock::now();
              auto duration =
                  std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time)
                      .count();

              auto error_crow = handleCrowError(
                  "Similarity calculation failed: " + std::string(e.what()), HTTP_INTERNAL_ERROR);
              logRequest(req, HTTP_INTERNAL_ERROR, error_crow.dump(), duration);
              return makeCrowJsonResponse(HTTP_INTERNAL_ERROR, error_crow);
          }
        #endif
      });

  // Context relationships endpoint
  app_->route_dynamic("/api/v1/context/relationships")
      .methods(crow::HTTPMethod::POST)([this, &engine](const crow::request& req) {
          auto start_time = std::chrono::steady_clock::now();

#if SEP_HAS_EXCEPTIONS
          try
          {
          
#endif
              nlohmann::json request_data = parse_json(std::string(req.body));
              auto result = engine.blendContexts(request_data);
              auto response_data = applyCoherenceModulation(result);

              auto end_time = std::chrono::steady_clock::now();
              auto duration =
                  std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time)
                      .count();

              logRequest(req, HTTP_OK, response_data.dump(), duration);
              return makeCrowJsonResponse(HTTP_OK, response_data);

        #if SEP_HAS_EXCEPTIONS
          } catch (const nlohmann::json::parse_error& e)
          {
              auto end_time = std::chrono::steady_clock::now();
              auto duration =
                  std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time)
                      .count();

              auto error_crow =
                  handleCrowError("Invalid JSON: " + std::string(e.what()), HTTP_BAD_REQUEST);
              logRequest(req, HTTP_BAD_REQUEST, error_crow.dump(), duration);
              return makeCrowJsonResponse(HTTP_BAD_REQUEST, error_crow);
          } catch (const std::exception& e)
          {
              auto end_time = std::chrono::steady_clock::now();
              auto duration =
                  std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time)
                      .count();

              auto error_crow = handleCrowError("Context blending failed: " + std::string(e.what()),
                                                HTTP_INTERNAL_ERROR);
              logRequest(req, HTTP_INTERNAL_ERROR, error_crow.dump(), duration);
              return makeCrowJsonResponse(HTTP_INTERNAL_ERROR, error_crow);
          }
        #endif
      });

  // Memory metrics endpoint
  app_->route_dynamic("/api/v1/metrics/memory")
      .methods(crow::HTTPMethod::GET)([this, &engine](const crow::request& req) {
          auto start_time = std::chrono::steady_clock::now();

#if SEP_HAS_EXCEPTIONS
          try
          {
          
#endif
              auto result = engine.getMemoryMetrics();
              auto response_data = applyCoherenceModulation(result);

              auto end_time = std::chrono::steady_clock::now();
              auto duration =
                  std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time)
                      .count();

              logRequest(req, HTTP_OK, response_data.dump(), duration);
              return makeCrowJsonResponse(HTTP_OK, response_data);

        #if SEP_HAS_EXCEPTIONS
          } catch (const std::exception& e)
          {
              auto end_time = std::chrono::steady_clock::now();
              auto duration =
                  std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time)
                      .count();

              auto error_crow = handleCrowError("Memory metrics failed: " + std::string(e.what()),
                                                HTTP_INTERNAL_ERROR);

              logRequest(req, HTTP_INTERNAL_ERROR, error_crow.dump(), duration);
              return makeCrowJsonResponse(HTTP_INTERNAL_ERROR, error_crow);
          }
        #endif
      });

  logger_->info("API routes configured successfully");
}

nlohmann::json SEPApiServer::applyCoherenceModulation(const nlohmann::json& response, double coherence_score) {
  if (!config_.response_modulation.enabled) {
    return response;
  }

  nlohmann::json modulated_response = response;

  // Add coherence metadata
  modulated_response["modulation"] = {
      {"applied", true},
      {"coherence_score", coherence_score},
  };

  logger_->debug("Applied coherence modulation: score={}, threshold={}");

  return modulated_response;
}

nlohmann::json SEPApiServer::applyCoherenceModulation(const nlohmann::json& response) {
  double coherence_score = 0.0;

  if (response.contains("coherence_score") && response["coherence_score"].is_number()) {
    coherence_score = response["coherence_score"].get<double>();
  } else if (response.contains("metrics") && response["metrics"].is_object() &&
             response["metrics"].contains("coherence_score") &&
             response["metrics"]["coherence_score"].is_number()) {
    coherence_score = response["metrics"]["coherence_score"].get<double>();
  } else {
    logger_->debug("No coherence_score found; returning original response");
    return response;
  }

  coherence_score = std::clamp(coherence_score, 0.0, 1.0);
  logger_->debug("Extracted coherence score: {} from response", coherence_score);
  return applyCoherenceModulation(response, coherence_score);
}

void SEPApiServer::setup_signal_handlers() {
  (void)std::signal(SIGINT, handleSignal);
  (void)std::signal(SIGTERM, handleSignal);
}

void SEPApiServer::handleSignal(int signal) {
  if (instance_) {
    instance_->logger_->info("Received signal {}, shutting down", signal);
    instance_->stop();
  }
}

}  // namespace api
}  // namespace sep

