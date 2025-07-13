
#include "workbench/core/workbench_core.hpp"
#include <iostream>
#include <exception>
#include <csignal>

// Global workbench instance for signal handling
static sep::workbench::WorkbenchCore* g_workbench = nullptr;

// Signal handler for graceful shutdown
void signalHandler(int signal) {
    std::cout << "\n[Main] Received signal " << signal << ", shutting down gracefully..." << std::endl;
    if (g_workbench) {
        g_workbench->shutdown();
    }
    exit(0);
}

int main(int, char**) {
    // Install signal handlers
    std::signal(SIGINT, signalHandler);
    std::signal(SIGTERM, signalHandler);
    
    std::cout << "=====================================\n";
    std::cout << "   SEP Workbench - Demo Suite v0.1   \n";
    std::cout << "=====================================\n\n";
    
    try {
        // Create workbench core
        auto workbench = std::make_unique<sep::workbench::WorkbenchCore>();
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
