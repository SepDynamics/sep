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

    auto& engine = sep::api::SepEngine::getInstance();
    engine.initialize(api_cfg);

    // The headless API server does not require a renderer
    sep::api::SEPApiServer server(api_cfg, nullptr);

    logger->info("Server object created.");

    if (!server.run()) {
        logger->critical("Failed to run the server!");
        return 1;
    }
    logger->info("Server is running. Waiting for shutdown signal...");
    server.waitForShutdown();

    engine.shutdown();
    logger->info("SEP Engine shutting down cleanly.");
    return 0;
}
