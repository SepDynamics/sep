#include "api/server.h"
#include "core/manager.h"
#include "core/logging.h"
#include "api/sep_engine.h"
#include <iostream>
#include <memory>

int main(int argc, char** argv) {
    sep::logging::Manager::initialize();
    auto logger = sep::logging::Manager::getInstance().getLogger("main");
    logger->info("SEP Engine starting up...");

    auto& cfg_manager = sep::config::ConfigManager::getInstance();
    cfg_manager.initialize(argc, argv);
    const auto& api_cfg = cfg_manager.getAPIConfig();
    logger->info("Configuration loaded. API will run on port {}.", api_cfg.port);

    // Initialize core engine before starting server
    auto& engine = sep::api::SepEngine::getInstance();
    auto init_result = engine.initialize(api_cfg);
    if (!init_result.value("success", true)) {
        logger->critical("Engine initialization failed: {}", init_result.dump());
        return 1;
    }

    // The headless API server does not require a renderer
    sep::api::SEPApiServer server(api_cfg, nullptr);

    logger->info("Server object created.");

    if (!server.run()) {
        logger->critical("Failed to run the server!");
        return 1;
    }
    logger->info("Server is running. Waiting for shutdown signal...");
    server.waitForShutdown();

    auto shutdown_result = engine.shutdown();
    if (!shutdown_result.value("success", true)) {
        logger->error("Engine shutdown reported issues: {}", shutdown_result.dump());
    }
    logger->info("SEP Engine shutting down cleanly.");
    return 0;
}
