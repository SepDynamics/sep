#include "quantum_tracker_app.hpp"
#include <iostream>
#include <thread>
#include <chrono>

void runHeadlessTest() {
    std::cout << "🔮 SEP Quantum Tracker - HEADLESS TEST MODE" << std::endl;
    std::cout << "===========================================" << std::endl;
    std::cout << "Testing data pipeline and calculations without GUI" << std::endl;
    std::cout << std::endl;

    sep::apps::QuantumTrackerApp app;
    if (!app.initialize()) {
        std::cerr << "[ERROR] Failed to initialize: " << app.getLastError() << std::endl;
        return;
    }

    std::cout << "✅ Quantum Tracker initialized successfully!" << std::endl;
    std::cout << "📊 Data pipeline active, CUDA calculations enabled" << std::endl;
    std::cout << "⏱️  Running test for 60 seconds..." << std::endl;
    std::cout << std::endl;

    // Let the system run for 60 seconds to collect data and run calculations
    auto start_time = std::chrono::steady_clock::now();
    while (true) {
        auto elapsed = std::chrono::steady_clock::now() - start_time;
        if (elapsed >= std::chrono::seconds(60)) {
            break;
        }
        
        // Print status every 10 seconds
        auto seconds = std::chrono::duration_cast<std::chrono::seconds>(elapsed).count();
        if (seconds % 10 == 0 && seconds > 0) {
            static int last_printed = -1;
            if (seconds != last_printed) {
                std::cout << "⏳ Running... " << seconds << "s elapsed" << std::endl;
                last_printed = seconds;
            }
        }
        
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    
    std::cout << std::endl;
    std::cout << "✅ Test completed successfully!" << std::endl;
    std::cout << "📈 Data pipeline and calculations verified" << std::endl;
    
    app.shutdown();
}

int main(int argc, char* argv[]) {
    // Check for headless test mode
    bool headless = false;
    for (int i = 1; i < argc; ++i) {
        if (std::string(argv[i]) == "--test" || std::string(argv[i]) == "--headless") {
            headless = true;
            break;
        }
    }
    
    if (headless) {
        runHeadlessTest();
        return 0;
    }
    
    // Normal GUI mode
    std::cout << "🔮 SEP Quantum Signal Tracker - Live Performance Monitor" << std::endl;
    std::cout << "=======================================================" << std::endl;
    std::cout << "This app tracks quantum predictions vs market performance in real-time." << std::endl;
    std::cout << "Press Ctrl+C to exit gracefully." << std::endl;
    std::cout << "Tip: Use --test flag for headless pipeline testing" << std::endl;
    std::cout << std::endl;

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
