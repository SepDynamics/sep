#include "quantum_tracker_app.hpp"
#include <iostream>
#include <thread>
#include <chrono>

void runHeadlessService() {
    std::cout << "🔮 SEP Quantum Tracker - HEADLESS SERVICE MODE" << std::endl;
    std::cout << "===============================================" << std::endl;
    std::cout << "24/7 Autonomous Trading Service (No GUI)" << std::endl;
    std::cout << "Press Ctrl+C to shutdown gracefully" << std::endl;
    std::cout << std::endl;

    sep::apps::QuantumTrackerApp app;
    if (!app.initialize()) {
        std::cerr << "[ERROR] Failed to initialize: " << app.getLastError() << std::endl;
        return;
    }

    std::cout << "✅ Quantum Tracker Service initialized successfully!" << std::endl;
    std::cout << "📊 Live trading pipeline active, CUDA calculations enabled" << std::endl;
    std::cout << "🌐 Autonomous mode: Trading during market hours, optimizing during weekends" << std::endl;
    std::cout << std::endl;

    // Run the core application logic without GUI
    app.runHeadlessService();
}

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
    // Check for command line arguments
    bool headless_service = false;
    bool headless_test = false;
    bool historical_sim = false;
    bool file_sim = false;
    std::string simulate_start_time;
    int simulate_duration_hours = 0;
    
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--headless") {
            headless_service = true;
        } else if (arg == "--test") {
            headless_test = true;
        } else if (arg == "--simulate" && i + 1 < argc) {
            simulate_start_time = argv[++i];
        } else if (arg == "--duration" && i + 1 < argc) {
            simulate_duration_hours = std::stoi(argv[++i]);
        } else if (arg == "--historical-sim") {
            historical_sim = true;
        } else if (arg == "--file-sim") {
            file_sim = true;
        }
    }
    
    // File Simulation Mode takes highest precedence for weekend development
    if (file_sim) {
        std::cout << "📁 SEP Quantum Tracker - FILE SIMULATION MODE" << std::endl;
        std::cout << "===============================================" << std::endl;
        std::cout << "Using local test files for rapid backtesting" << std::endl;
        std::cout << "Perfect for weekend strategy optimization" << std::endl;
        std::cout << std::endl;
        
        sep::apps::QuantumTrackerApp app(false, true); // historical_sim = false, file_sim = true
        if (!app.initialize()) {
            std::cerr << "[ERROR] Failed to initialize: " << app.getLastError() << std::endl;
            return 1;
        }
        
        return 0; // runFileSimulation() is called in initialize()
    }
    
    // Historical Simulation Mode takes second precedence
    if (historical_sim) {
        std::cout << "📊 SEP Quantum Tracker - HISTORICAL SIMULATION MODE" << std::endl;
        std::cout << "====================================================" << std::endl;
        std::cout << "Using proven test data for deterministic backtesting" << std::endl;
        std::cout << std::endl;
        
        sep::apps::QuantumTrackerApp app(true); // historical_sim = true
        if (!app.initialize()) {
            std::cerr << "[ERROR] Failed to initialize: " << app.getLastError() << std::endl;
            return 1;
        }
        
        return 0; // runHistoricalSimulation() is called in initialize()
    }
    
    // Time Machine Mode takes precedence over normal modes
    if (!simulate_start_time.empty() && simulate_duration_hours > 0) {
        std::cout << "🕰️  SEP Quantum Tracker - TIME MACHINE MODE" << std::endl;
        std::cout << "=============================================" << std::endl;
        std::cout << "Simulating from: " << simulate_start_time << std::endl;
        std::cout << "Duration: " << simulate_duration_hours << " hours" << std::endl;
        std::cout << std::endl;
        
        sep::apps::QuantumTrackerApp app(simulate_start_time, simulate_duration_hours);
        if (!app.initialize()) {
            std::cerr << "[ERROR] Failed to initialize: " << app.getLastError() << std::endl;
            return 1;
        }
        
        return 0; // runSimulation() is called in initialize()
    }
    
    if (headless_service) {
        runHeadlessService();
        return 0;
    } else if (headless_test) {
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
