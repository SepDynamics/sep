#include "api/server.h"

#include <spdlog/spdlog.h>

#include <chrono>
#include <csignal>
#include <cstdio>
#include <memory>
#include <nlohmann/json.hpp>
#include <thread>

#include "api/client.h"
#include "api/crow_request.h"
#include "api/json_helpers.h"
#include "api/rate_limit_middleware.h"
#include "api/request_interface.h"
#include "api/sep_engine.h"
#include "api/types.h"
#include "engine/internal/config.h"
#include "engine/internal/types.h"
#include "memory/memory_tier_manager.hpp"
#include "quantum/types.h"

namespace sep::api {

#ifndef SEP_HAS_EXCEPTIONS
#    if defined(__cpp_exceptions) || defined(__EXCEPTIONS) || defined(_CPPUNWIND)
#        define SEP_HAS_EXCEPTIONS 1
#    else
#        define SEP_HAS_EXCEPTIONS 0
#    endif
#endif

// Static instance for signal handling
SEPApiServer* SEPApiServer::instance_ = nullptr;
SEPApiServer::SEPApiServer(const ::sep::config::SystemConfig& config)
    : config_(config),
      running_(false),
      logger_(nullptr),
      thread_pool_(std::make_unique<ThreadPool>(config.api.threads))
{
    instance_ = this;

    // Initialize the Crow app with middlewares
    app_ = std::make_unique<::crow::Crow<RateLimitMiddleware, AuthMiddleware>>();

    // Initialize logger
    setup_logging();

    // Initialize Ollama client
    ollama_client_ = std::make_unique<sep::ollama::OllamaClient>(config_.ollama);
}

SEPApiServer::~SEPApiServer() {
  if (running_) {
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

bool SEPApiServer::start() {
    if (!app_) return false;

    app_->port(config_.port);
    app_->concurrency(config_.api.threads);

    setup_middleware();
    setup_routes();

    return true;
}

bool SEPApiServer::run() {
  // Ensure routes are registered just before starting
  setup_routes();
  return start();
}

void SEPApiServer::start() {
    if (!app_) return;

    app_->port(config_.port);
    app_->concurrency(config_.api.threads);

    setup_middleware();
    setup_routes();
}


void SEPApiServer::stop() {
  if (!running_) {
    return;
  }

  logger_->info("Stopping SEP API Server");
  running_ = false;

  if (app_) {
    app_->stop();
  }

  if (server_thread_ && server_thread_->joinable()) {
    server_thread_->join();
  }

  logger_->info("SEP API Server stopped");
}

void SEPApiServer::waitForShutdown() {
  if (server_thread_ && server_thread_->joinable()) {
    server_thread_->join();
  }
}

void SEPApiServer::updateConfig(const ::sep::config::SystemConfig& new_config) {
  std::lock_guard<std::mutex> lock(metrics_mutex_);
  config_ = new_config;
  logger_->info("Configuration updated");
}


std::unique_ptr<HttpResponse> SEPApiServer::makeJsonResponse(int code, const std::string& message) {
  // This is a simplified implementation
  class SimpleHttpResponse : public HttpResponse {
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

std::string SEPApiServer::handleError(const std::string& message, int code) {
  nlohmann::json error_response;
  error_response["error"] = true;
  error_response["message"] = message;
  error_response["code"] = code;
  error_response["timestamp"] = std::chrono::duration_cast<std::chrono::seconds>(
                                    std::chrono::system_clock::now().time_since_epoch())
                                    .count();

  logger_->error("API Error [{}]: {}", code, message);

  std::lock_guard<std::mutex> lock(metrics_mutex_);
  metrics_.failedRequests++;
  metrics_.lastErrorCode = code;
  metrics_.lastErrorTime = std::chrono::system_clock::now();

  return error_response.dump();
}

void SEPApiServer::logRequest(const HttpRequest& req, int code, const std::string& body,
                              int64_t duration) {
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

  logger_->info("Request: {} {} - Status: {} - Duration: {}ms",
                req.method(), req.url(), code, duration);
}

std::string SEPApiServer::getErrorResponse(const std::string& message, int status) {
  return handleError(message, status);
}


::crow::response SEPApiServer::makeCrowJsonResponse(int status_code, const nlohmann::json& data) {
  ::crow::response res(status_code);
  res.set_header("Content-Type", "application/json");
  res.body = data.dump();
  return res;
}

nlohmann::json SEPApiServer::handleCrowError(const std::string& message,
                                             int status_code) {
  nlohmann::json error_json{
      {"error", true},
      {"message", message},
      {"code", status_code},
      {"timestamp",
       std::chrono::duration_cast<std::chrono::seconds>(
           std::chrono::system_clock::now().time_since_epoch())
           .count()}};

  logger_->error("API Error [{}]: {}", status_code, message);

  std::lock_guard<std::mutex> lock(metrics_mutex_);
  metrics_.failedRequests++;
  metrics_.lastErrorCode = status_code;
  metrics_.lastErrorTime = std::chrono::system_clock::now();

  return error_json;
}

void SEPApiServer::logRequest(const ::crow::request& req, int status_code,
                              const std::string& response_body, int64_t duration_ms) {
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

  std::string method_name = std::string(::crow::method_name(req.method));
  std::string url = std::string(req.url);

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
   auto& rate_limit_mw =
      app_->get_middleware<RateLimitMiddleware>();

  // Configure auth middleware
  auto& auth_mw = app_->get_middleware<AuthMiddleware>();
  // Set auth tokens if configured
  if (!config_.extra_settings.empty()) {
    auto tokens_it = config_.extra_settings.find("auth_tokens");
    if (tokens_it != config_.extra_settings.end()) {
      // Parse and set tokens - simplified implementation
      std::vector<std::string> tokens = {tokens_it->second};
      auth_mw.set_tokens(tokens);
    }
  }
}

void SEPApiServer::setup_routes() {
  if (!app_) return;

  auto& engine = SepEngine::getInstance();

  // Health check endpoint
  app_->route_dynamic("/api/v1/health")
      .methods(::crow::HTTPMethod::GET)([this, &engine](const ::crow::request& req) {
    auto start_time = std::chrono::steady_clock::now();

    #if SEP_HAS_EXCEPTIONS
    try {
#endif
        auto future = thread_pool_->enqueue([&engine] { return engine.getHealthStatus(); });
        auto health_data = future.get();

        auto end_time = std::chrono::steady_clock::now();
        auto duration =
            std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();

        logRequest(req, 200, health_data.dump(), duration);
        return makeCrowJsonResponse(200, health_data);

#if SEP_HAS_EXCEPTIONS
    } catch (const std::exception& e) {
      auto end_time = std::chrono::steady_clock::now();
      auto duration =
          std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();

      auto error_crow =
          handleCrowError("Health check failed: " + std::string(e.what()), HTTP_INTERNAL_ERROR);
      logRequest(req, HTTP_INTERNAL_ERROR, error_crow.dump(), duration);
      return makeCrowJsonResponse(HTTP_INTERNAL_ERROR, error_crow);
    }
#endif
  });

  // Process patterns endpoint
  app_->route_dynamic("/api/v1/pattern/evolve")
      .methods(::crow::HTTPMethod::POST)([this, &engine](const ::crow::request& req) {
        auto start_time = std::chrono::steady_clock::now();

#if SEP_HAS_EXCEPTIONS
        try {
#endif
          // Parse request body
          nlohmann::json request_data = parse_json(std::string(req.body));

          // Process patterns through SEP engine
          auto future = thread_pool_->enqueue(
              [&engine, request_data] { return engine.processPatterns(request_data); });
          auto result = future.get();
          auto response_data = applyCoherenceModulation(result);

          auto end_time = std::chrono::steady_clock::now();
          auto duration =
              std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();

          logRequest(req, HTTP_OK, response_data.dump(), duration);
          return makeCrowJsonResponse(HTTP_OK, response_data);

        #if SEP_HAS_EXCEPTIONS
        } catch (const nlohmann::json::parse_error& e) {
          auto end_time = std::chrono::steady_clock::now();
          auto duration =
              std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();

          auto error_crow =
              handleCrowError("Invalid JSON: " + std::string(e.what()), HTTP_BAD_REQUEST);
          logRequest(req, HTTP_BAD_REQUEST, error_crow.dump(), duration);
          return makeCrowJsonResponse(HTTP_BAD_REQUEST, error_crow);

        } catch (const std::exception& e) {
          auto end_time = std::chrono::steady_clock::now();
          auto duration =
              std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();

          auto error_crow = handleCrowError("Pattern processing failed: " + std::string(e.what()),
                                            HTTP_INTERNAL_ERROR);
          logRequest(req, HTTP_INTERNAL_ERROR, error_crow.dump(), duration);
          return makeCrowJsonResponse(HTTP_INTERNAL_ERROR, error_crow);
        }
        #endif
      });

  // Process batch endpoint
  app_->route_dynamic("/api/v1/memory/query")
      .methods(::crow::HTTPMethod::POST)([this, &engine](const ::crow::request& req) {
        auto start_time = std::chrono::steady_clock::now();

#if SEP_HAS_EXCEPTIONS
        try {
#endif
          nlohmann::json request_data = parse_json(std::string(req.body));
          auto future = thread_pool_->enqueue(
              [&engine, request_data] { return engine.processBatch(request_data); });
          auto result = future.get();
          auto response_data = applyCoherenceModulation(result);

          auto end_time = std::chrono::steady_clock::now();
          auto duration =
              std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();

          logRequest(req, HTTP_OK, response_data.dump(), duration);
          return makeCrowJsonResponse(HTTP_OK, response_data);

        #if SEP_HAS_EXCEPTIONS
        } catch (const nlohmann::json::parse_error& e) {
          auto end_time = std::chrono::steady_clock::now();
          auto duration =
              std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();

          auto error_crow =
              handleCrowError("Invalid JSON: " + std::string(e.what()), HTTP_BAD_REQUEST);
          logRequest(req, HTTP_BAD_REQUEST, error_crow.dump(), duration);
          return makeCrowJsonResponse(HTTP_BAD_REQUEST, error_crow);

        } catch (const std::exception& e) {
          auto end_time = std::chrono::steady_clock::now();
          auto duration =
              std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();

          auto error_crow = handleCrowError("Batch processing failed: " + std::string(e.what()),
                                            HTTP_INTERNAL_ERROR);
          logRequest(req, HTTP_INTERNAL_ERROR, error_crow.dump(), duration);
          return makeCrowJsonResponse(HTTP_INTERNAL_ERROR, error_crow);
        }
        #endif
      });

  // Pattern history endpoint
  app_->route_dynamic("/api/v1/patterns/history")
      .methods(::crow::HTTPMethod::POST)([this, &engine](const ::crow::request& req) {
        auto start_time = std::chrono::steady_clock::now();

#if SEP_HAS_EXCEPTIONS
        try {
#endif
          nlohmann::json request_data = parse_json(std::string(req.body));
          auto future = thread_pool_->enqueue(
              [&engine, request_data] { return engine.getPatternHistory(request_data); });
          auto result = future.get();
          auto response_data = applyCoherenceModulation(result);

          auto end_time = std::chrono::steady_clock::now();
          auto duration =
              std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();

          logRequest(req, HTTP_OK, response_data.dump(), duration);
          return makeCrowJsonResponse(HTTP_OK, response_data);

        #if SEP_HAS_EXCEPTIONS
        } catch (const nlohmann::json::parse_error& e) {
          auto end_time = std::chrono::steady_clock::now();
          auto duration =
              std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();

          auto error_crow =
              handleCrowError("Invalid JSON: " + std::string(e.what()), HTTP_BAD_REQUEST);
          logRequest(req, HTTP_BAD_REQUEST, error_crow.dump(), duration);
          return makeCrowJsonResponse(HTTP_BAD_REQUEST, error_crow);

        } catch (const std::exception& e) {
          auto end_time = std::chrono::steady_clock::now();
          auto duration =
              std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();

          auto error_crow = handleCrowError("Pattern history failed: " + std::string(e.what()),
                                            HTTP_INTERNAL_ERROR);
          logRequest(req, HTTP_INTERNAL_ERROR, error_crow.dump(), duration);
          return makeCrowJsonResponse(HTTP_INTERNAL_ERROR, error_crow);
        }
        #endif
      });

  // Validate contexts endpoint
  app_->route_dynamic("/api/v1/context/process")
      .methods(::crow::HTTPMethod::POST)([this, &engine](const ::crow::request& req) {
        auto start_time = std::chrono::steady_clock::now();

#if SEP_HAS_EXCEPTIONS
        try {
#endif
          nlohmann::json request_data = parse_json(std::string(req.body));
          auto future = thread_pool_->enqueue(
              [&engine, request_data] { return engine.validateContexts(request_data); });
          auto result = future.get();
          auto response_data = applyCoherenceModulation(result);

          auto end_time = std::chrono::steady_clock::now();
          auto duration =
              std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();

          logRequest(req, HTTP_OK, response_data.dump(), duration);
          return makeCrowJsonResponse(HTTP_OK, response_data);

        #if SEP_HAS_EXCEPTIONS
        } catch (const nlohmann::json::parse_error& e) {
          auto end_time = std::chrono::steady_clock::now();
          auto duration =
              std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();

          auto error_crow =
              handleCrowError("Invalid JSON: " + std::string(e.what()), HTTP_BAD_REQUEST);
          logRequest(req, HTTP_BAD_REQUEST, error_crow.dump(), duration);
          return makeCrowJsonResponse(HTTP_BAD_REQUEST, error_crow);

        } catch (const std::exception& e) {
          auto end_time = std::chrono::steady_clock::now();
          auto duration =
              std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();

          auto error_crow = handleCrowError("Context validation failed: " + std::string(e.what()),
                                            HTTP_INTERNAL_ERROR);
          logRequest(req, HTTP_INTERNAL_ERROR, error_crow.dump(), duration);
          return makeCrowJsonResponse(HTTP_INTERNAL_ERROR, error_crow);
        }
        #endif
      });

  // Extract embeddings endpoint
  app_->route_dynamic("/api/v1/embeddings/extract")
      .methods(::crow::HTTPMethod::POST)([this, &engine](const ::crow::request& req) {
        auto start_time = std::chrono::steady_clock::now();

#if SEP_HAS_EXCEPTIONS
        try {
#endif
          nlohmann::json request_data = parse_json(std::string(req.body));
          auto future = thread_pool_->enqueue(
              [&engine, request_data] { return engine.extractEmbeddings(request_data); });
          auto result = future.get();
          auto response_data = applyCoherenceModulation(result);

          auto end_time = std::chrono::steady_clock::now();
          auto duration =
              std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();

          logRequest(req, HTTP_OK, response_data.dump(), duration);
          return makeCrowJsonResponse(HTTP_OK, response_data);

        #if SEP_HAS_EXCEPTIONS
        } catch (const nlohmann::json::parse_error& e) {
          auto end_time = std::chrono::steady_clock::now();
          auto duration =
              std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();

          auto error_crow =
              handleCrowError("Invalid JSON: " + std::string(e.what()), HTTP_BAD_REQUEST);
          logRequest(req, HTTP_BAD_REQUEST, error_crow.dump(), duration);
          return makeCrowJsonResponse(HTTP_BAD_REQUEST, error_crow);

        } catch (const std::exception& e) {
          auto end_time = std::chrono::steady_clock::now();
          auto duration =
              std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();

          auto error_crow = handleCrowError("Embedding extraction failed: " + std::string(e.what()),
                                            HTTP_INTERNAL_ERROR);
          logRequest(req, HTTP_INTERNAL_ERROR, error_crow.dump(), duration);
          return makeCrowJsonResponse(HTTP_INTERNAL_ERROR, error_crow);
        }
        #endif
      });

  // Analyze patterns endpoint
  app_->route_dynamic("/api/v1/pattern/analyze")
      .methods(::crow::HTTPMethod::POST)([this, &engine](const ::crow::request& req) {
        auto start_time = std::chrono::steady_clock::now();

#if SEP_HAS_EXCEPTIONS
        try {
#endif
          nlohmann::json request_data = parse_json(std::string(req.body));
          auto future = thread_pool_->enqueue(
              [&engine, request_data] { return engine.calculateSimilarity(request_data); });
          auto result = future.get();
          auto response_data = applyCoherenceModulation(result);

          auto end_time = std::chrono::steady_clock::now();
          auto duration =
              std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();

          logRequest(req, HTTP_OK, response_data.dump(), duration);
          return makeCrowJsonResponse(HTTP_OK, response_data);

        #if SEP_HAS_EXCEPTIONS
        } catch (const nlohmann::json::parse_error& e) {
          auto end_time = std::chrono::steady_clock::now();
          auto duration =
              std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();

          auto error_crow =
              handleCrowError("Invalid JSON: " + std::string(e.what()), HTTP_BAD_REQUEST);
          logRequest(req, HTTP_BAD_REQUEST, error_crow.dump(), duration);
          return makeCrowJsonResponse(HTTP_BAD_REQUEST, error_crow);

        } catch (const std::exception& e) {
          auto end_time = std::chrono::steady_clock::now();
          auto duration =
              std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();

          auto error_crow = handleCrowError(
              "Similarity calculation failed: " + std::string(e.what()), HTTP_INTERNAL_ERROR);
          logRequest(req, HTTP_INTERNAL_ERROR, error_crow.dump(), duration);
          return makeCrowJsonResponse(HTTP_INTERNAL_ERROR, error_crow);
        }
        #endif
      });

  // Context relationships endpoint
  app_->route_dynamic("/api/v1/context/relationships")
      .methods(::crow::HTTPMethod::POST)([this, &engine](const ::crow::request& req) {
        auto start_time = std::chrono::steady_clock::now();

#if SEP_HAS_EXCEPTIONS
        try {
#endif
          nlohmann::json request_data = parse_json(std::string(req.body));
          auto future = thread_pool_->enqueue(
              [&engine, request_data] { return engine.blendContexts(request_data); });
          auto result = future.get();
          auto response_data = applyCoherenceModulation(result);

          auto end_time = std::chrono::steady_clock::now();
          auto duration =
              std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();

          logRequest(req, HTTP_OK, response_data.dump(), duration);
          return makeCrowJsonResponse(HTTP_OK, response_data);

        #if SEP_HAS_EXCEPTIONS
        } catch (const nlohmann::json::parse_error& e) {
          auto end_time = std::chrono::steady_clock::now();
          auto duration =
              std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();

          auto error_crow =
              handleCrowError("Invalid JSON: " + std::string(e.what()), HTTP_BAD_REQUEST);
          logRequest(req, HTTP_BAD_REQUEST, error_crow.dump(), duration);
          return makeCrowJsonResponse(HTTP_BAD_REQUEST, error_crow);

        } catch (const std::exception& e) {
          auto end_time = std::chrono::steady_clock::now();
          auto duration =
              std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();

          auto error_crow = handleCrowError("Context blending failed: " + std::string(e.what()),
                                            HTTP_INTERNAL_ERROR);
          logRequest(req, HTTP_INTERNAL_ERROR, error_crow.dump(), duration);
          return makeCrowJsonResponse(HTTP_INTERNAL_ERROR, error_crow);
        }
        #endif
      });

  // Memory metrics endpoint
  app_->route_dynamic("/api/v1/metrics/memory")
      .methods(::crow::HTTPMethod::GET)([this, &engine](const ::crow::request& req) {
        auto start_time = std::chrono::steady_clock::now();

#if SEP_HAS_EXCEPTIONS
        try {
#endif
            auto future = thread_pool_->enqueue([&engine] { return engine.getMemoryMetrics(); });
            auto result = future.get();
            auto response_data = applyCoherenceModulation(result);

            auto end_time = std::chrono::steady_clock::now();
            auto duration =
                std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time)
                    .count();

            logRequest(req, HTTP_OK, response_data.dump(), duration);
            return makeCrowJsonResponse(HTTP_OK, response_data);

#if SEP_HAS_EXCEPTIONS
        } catch (const std::exception& e) {
          auto end_time = std::chrono::steady_clock::now();
          auto duration =
              std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();

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

  // Apply coherence-based response modulation
  if (coherence_score < config_.response_modulation.coherence_threshold) {
    if (config_.response_modulation.simplify_low_coherence) {
      // Simplify response for low coherence
      if (modulated_response.contains("details") && modulated_response["details"].is_array()) {
        auto& details = modulated_response["details"];
        if (details.size() > static_cast<size_t>(config_.response_modulation.max_detail_level)) {
          details = nlohmann::json::array();
          for (size_t i = 0; i < static_cast<size_t>(config_.response_modulation.max_detail_level); ++i) {
            if (i < response["details"].size()) {
              details.push_back(response["details"][i]);
            }
          }
        }
      }

      // Add coherence warning for low scores
      modulated_response["coherence_warning"] = "Response simplified due to low coherence score";
      modulated_response["coherence_score"] = coherence_score;
    }
  }

  // Add coherence metadata
  modulated_response["modulation"] = {
    {"applied", true},
    {"coherence_score", coherence_score},
    {"threshold", config_.response_modulation.coherence_threshold},
    {"simplified", coherence_score < config_.response_modulation.coherence_threshold}
  };

  logger_->debug("Applied coherence modulation: score={}, threshold={}",
                 coherence_score, config_.response_modulation.coherence_threshold);

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

void SEPApiServer::initClients() {
#if SEP_HAS_EXCEPTIONS
  try {
#endif
    // Initialize Ollama client if configured
    ollama_client_ = std::make_unique<sep::ollama::OllamaClient>(config_.ollama);
    logger_->info("Ollama client initialized");
#if SEP_HAS_EXCEPTIONS
  } catch (const std::exception& e) {
    logger_->warn("Failed to initialize Ollama client: {}", e.what());
  }
#endif
}

void SEPApiServer::handleSignal(int signal) {
  if (instance_) {
    instance_->logger_->info("Received signal {}, shutting down", signal);
    instance_->stop();
  }
}

}  // namespace sep::api
