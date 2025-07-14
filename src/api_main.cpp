#include <csignal>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <string>
#include <thread>

#include "api/auth_middleware.h"
#include "api/server.h"
#include "core/manager.h"
#include "core/engine.h"
#include "blender/cycles_renderer.hpp"

// Default auth configuration
sep::api::AuthConfig createDefaultConfig()
{
    sep::api::AuthConfig config;

    // Set default port from environment variable or use default 3000
    const char* port_env = std::getenv("SEP_API_PORT");
    config.port = port_env ? std::atoi(port_env) : 3000;

    // Set default log level
    config.log_level = "info";

    // Set up CORS
    config.cors.enabled = true;

    return config;
}

// Parse command line arguments
bool parseCmdArgs(int argc, char** argv, bool& daemon_mode)
{
    for (int i = 1; i < argc; i++)
    {
        std::string arg = argv[i];
        if (arg == "--foreground")
        {
            daemon_mode = false;
        }
        else if (arg == "--daemon")
        {
            daemon_mode = true;
        }
        else if (arg == "--help" || arg == "-h")
        {
            std::cout << "Usage: sep_api_server [OPTIONS]" << std::endl;
            std::cout << "Options:" << std::endl;
            std::cout << "  --config FILE       Path to config file" << std::endl;
            std::cout << "  --foreground        Run in foreground (default)" << std::endl;
            std::cout << "  --daemon            Run as daemon" << std::endl;
            std::cout << "  --help, -h          Show this help message" << std::endl;
            return false;
        }
    }
    return true;
}

int main(int argc, char** argv)
{
    // Default to foreground mode
    bool daemon_mode = false;

    // Parse command line arguments
    if (!parseCmdArgs(argc, argv, daemon_mode))
    {
        return 0;  // Help message was shown
    }

    // Initialize config manager with command line args
    auto& configMgr = sep::config::ConfigManager::getInstance();
    configMgr.initialize(argc, argv);

    // Create renderer implementation
    auto renderer = std::make_unique<sep::blender::CyclesRenderer>();
    renderer->setRotation(0.0f);

    // Create auth config
    sep::api::AuthConfig auth_config = createDefaultConfig();

    // Create and start API server
    std::unique_ptr<sep::api::SEPApiServer> server =
        std::make_unique<sep::api::SEPApiServer>(auth_config, renderer.get());

    std::cout << "Starting SEP API Server on port " << auth_config.port << std::endl;

    if (!server->run())
    {
        std::cerr << "Failed to start SEP API Server" << std::endl;
        return 1;
    }

    if (daemon_mode)
    {
        // In daemon mode, block indefinitely until a signal is received
        std::signal(SIGINT, [](int) {
            std::cout << "Received SIGINT, shutting down..." << std::endl;
            exit(0);
        });
        std::signal(SIGTERM, [](int) {
            std::cout << "Received SIGTERM, shutting down..." << std::endl;
            exit(0);
        });

        // Sleep indefinitely
        while (true)
        {
            std::this_thread::sleep_for(std::chrono::seconds(3600));  // Sleep for an hour
        }
    }
    else
    {
        // In foreground mode, wait for server to finish
        server->waitForShutdown();
    }

    return 0;
}
