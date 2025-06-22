#include "config/manager.h"
#include "core/engine.h"
#include "memory/manager.h"
#include "api/server.h"
#include <curl/curl.h>
#include <exception>
#include <iostream>
#include <spdlog/spdlog.h>

#ifndef SEP_HAS_EXCEPTIONS
#    if defined(__cpp_exceptions) || defined(__EXCEPTIONS) || defined(_CPPUNWIND)
#        define SEP_HAS_EXCEPTIONS 1
#    else
#        define SEP_HAS_EXCEPTIONS 0
#    endif
#endif

int main(int argc, char* argv[]) {
  curl_global_init(CURL_GLOBAL_ALL);
  sep::logging::initializeLogging();

#if SEP_HAS_EXCEPTIONS
  try {
#endif
    auto& config = sep::config::ConfigManager::getInstance();
    config.initialize(argc, argv);

    sep::core::Engine engine;
    if (!engine.init(config.getAPIConfig())) {
#if SEP_HAS_EXCEPTIONS
      throw std::runtime_error("Engine initialization failed");
#else
      spdlog::critical("Engine initialization failed");
      curl_global_cleanup();
      sep::logging::shutdownLogging();
      return 1;
#endif
    }
    engine.run();

    sep::api::SEPApiServer server(config.getAPIConfig());
    server.run();
    server.waitForShutdown();
    engine.shutdown();
#if SEP_HAS_EXCEPTIONS
  } catch (const std::exception& e) {
    spdlog::critical("Unhandled exception: {}", e.what());
    curl_global_cleanup();
    sep::logging::shutdownLogging();
    return 1;
  }
#endif

  curl_global_cleanup();
  sep::logging::shutdownLogging();
  return 0;
}
