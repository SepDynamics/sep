#include <atomic>
#include <chrono>
#include <csignal>
#include <thread>
#include <curl/curl.h>
#include <spdlog/spdlog.h>

#include "core/manager.h"
#include "core/logging.h"
#include "api/server.h"

static std::atomic<bool> g_keep_running{true};

static void signal_handler(int signal) {
    spdlog::info("Received signal {}", signal);
    g_keep_running.store(false);
}

int main(int argc, char* argv[]) {
    curl_global_init(CURL_GLOBAL_ALL);
    sep::logging::Manager::initialize();

    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);

    auto& config = sep::config::ConfigManager::getInstance();
    config.initialize(argc, argv);

    sep::api::SEPApiServer server(config.getAPIConfig());
    server.run();

    while (g_keep_running.load()) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    server.stop();
    server.waitForShutdown();

    curl_global_cleanup();
    sep::logging::Manager::shutdown();
    return 0;
}
