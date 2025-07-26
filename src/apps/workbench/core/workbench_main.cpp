#include "std_includes.h"
#include "workbench_core.hpp"
#include "engine/logging.h"
#include "apps/workbench/core/service_proxy_engine.h"
#include "apps/workbench/core/service_connector.hpp"
#include "apps/workbench/backtester/backtester.h"
#include "apps/workbench/backtester/data/data_loader.h"

// Global workbench instance for signal handling
static sep::workbench::WorkbenchEngine* g_workbench = nullptr;

// Signal handler for graceful shutdown
void signalHandler(int signal) {
    std::cout << "\n[Main] Received signal " << signal << ", shutting down gracefully..." << std::endl;
    if (g_workbench) {
        g_workbench->shutdown();
    }
    exit(0);
}

int main(int argc, char* argv[]) {
    sep::logging::Manager::initialize();
    sep::logging::LoggerConfig config;
    config.file.path = "pattern_engine.log";
    sep::logging::Manager::getInstance().createLogger("pattern_engine", config);
    // Install signal handlers
    (void)std::signal(SIGINT, signalHandler);
    (void)std::signal(SIGTERM, signalHandler);
    
    // Parse command line arguments for OANDA credentials
    std::string api_key, account_id;
    bool skip_fetch = false;
    for (int i = 1; i < argc; i++) {
        if (std::string(argv[i]) == "--api-key" && i + 1 < argc) {
            api_key = argv[i + 1];
            i++; // Skip the next argument
        } else if (std::string(argv[i]) == "--account-id" && i + 1 < argc) {
            account_id = argv[i + 1];
            i++; // Skip the next argument
        } else if (std::string(argv[i]) == "--skip-fetch") {
            skip_fetch = true;
        }
    }
    
    // Set environment variables if provided via command line
    if (!api_key.empty()) {
        setenv("OANDA_API_KEY", api_key.c_str(), 1);
        std::cout << "[Main] OANDA API Key set from command line" << std::endl;
    }
    if (!account_id.empty()) {
        setenv("OANDA_ACCOUNT_ID", account_id.c_str(), 1);
        std::cout << "[Main] OANDA Account ID set: " << account_id << std::endl;
    }
    if (skip_fetch) {
        setenv("SEP_SKIP_FETCH", "1", 1);
    }

    // Verify SEP engine availability before launching
    sep::workbench::ServiceConnector connection_test;
    if (!connection_test.connect()) {
        std::cerr << "[Main] Failed to connect to SEP engine. Exiting." << std::endl;
        return 1;
    }
    connection_test.disconnect();
    
    std::cout << "=====================================\n";
    std::cout << "   SEP OANDA Trading Engine v1.0     \n";
    std::cout << "=====================================\n\n";
    
    try {
        // Create workbench core
        auto workbench = std::make_unique<sep::workbench::WorkbenchEngine>();
        g_workbench = workbench.get();
        
        // Initialize
        std::cout << "[Main] Initializing workbench..." << std::endl;
        if (!workbench->initialize()) {
            std::cerr << "[Main] Failed to initialize workbench: " 
                      << workbench->getLastError() << std::endl;
            return 1;
        }
        
        // Run main loop
        std::cout << "[Main] Starting main loop..." << std::endl;
        workbench->run();
        
        // Cleanup
        std::cout << "[Main] Shutting down..." << std::endl;
        workbench->shutdown();
        
        std::cout << "[Main] Goodbye!" << std::endl;
        return 0;
        
    } catch (const std::exception& e) {
        std::cerr << "[Main] Fatal error: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "[Main] Unknown fatal error" << std::endl;
        return 1;
    }
}