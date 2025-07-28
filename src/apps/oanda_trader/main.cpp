#include "oanda_trader_app.hpp"
#include <iostream>
#include <exception>
#include <signal.h>

// Declare signal handling functions and constants
extern "C" {
    typedef void (*sighandler_t)(int);
    sighandler_t signal(int signum, sighandler_t handler);
}
#define SIGINT 2
#define SIGTERM 15

// Global app instance for signal handling
static sep::apps::OandaTraderApp* g_app = nullptr;

// Signal handler for graceful shutdown
void signalHandler(int signal) {
    std::cout << "\n[Main] Received signal " << signal << ", shutting down gracefully..." << std::endl;
    if (g_app) {
        g_app->shutdown();
    }
    exit(0);
}

int main(int /*argc*/, char* /*argv*/[]) {
    // Install signal handlers
    (void)signal(SIGINT, signalHandler);
    (void)signal(SIGTERM, signalHandler);
    
    std::cout << "=====================================\n";
    std::cout << "   SEP OANDA Trader - v1.0          \n";
    std::cout << "=====================================\n\n";
    
    try {
        // Create OANDA trader app
        auto app = std::make_unique<sep::apps::OandaTraderApp>();
        g_app = app.get();
        
        // Initialize
        std::cout << "[Main] Initializing OANDA Trader..." << std::endl;
        if (!app->initialize()) {
            std::cerr << "[Main] Failed to initialize OANDA Trader: " 
                      << app->getLastError() << std::endl;
            return 1;
        }
        
        // Run main loop
        app->run();
        
        // Cleanup
        app->shutdown();
        
        return 0;
        
    } catch (const std::exception& e) {
        std::cerr << "[Main] Fatal error: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "[Main] Unknown fatal error" << std::endl;
        return 1;
    }
}
