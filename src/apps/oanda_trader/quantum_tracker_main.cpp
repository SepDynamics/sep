#include "quantum_tracker_app.hpp"
#include <iostream>

int main() {
    
    std::cout << "🔮 SEP Quantum Signal Tracker - Live Performance Monitor" << std::endl;
    std::cout << "=======================================================" << std::endl;
    std::cout << "This app tracks quantum predictions vs market performance in real-time." << std::endl;
    std::cout << "Press Ctrl+C to exit gracefully.\n" << std::endl;
    
    // Create and initialize the app
    sep::apps::QuantumTrackerApp app;
    
    if (!app.initialize()) {
        std::cerr << "[ERROR] Failed to initialize: " << app.getLastError() << std::endl;
        return 1;
    }
    
    std::cout << "[QuantumTracker] Initialization complete. Starting live tracking..." << std::endl;
    
    try {
        // Run the main loop
        app.run();
    } catch (const std::exception& e) {
        std::cerr << "[ERROR] Runtime exception: " << e.what() << std::endl;
        app.shutdown();
        return 1;
    }
    
    // Clean shutdown
    app.shutdown();
    std::cout << "[QuantumTracker] Shutdown complete." << std::endl;
    
    return 0;
}
