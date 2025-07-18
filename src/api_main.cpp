#include <csignal>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <memory>
#include <string>
#include <thread>

#include "api/auth_middleware.h"
#include "api/server.h"
#include "core/manager.h"
#include "core/data_parser.h"
#include "core/dag_graph.h"
#include <nlohmann/json.hpp>

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
bool parseCmdArgs(int argc, char** argv, bool& daemon_mode, std::string& quant_file, std::string& output_file)
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
        else if (arg == "--quant" && i + 1 < argc)
        {
            quant_file = argv[++i];
        }
        else if (arg == "--output" && i + 1 < argc)
        {
            output_file = argv[++i];
        }
        else if (arg == "--help" || arg == "-h")
        {
            std::cout << "Usage: sep_engine [OPTIONS]" << std::endl;
            std::cout << "Options:" << std::endl;
            std::cout << "  --config FILE       Path to config file" << std::endl;
            std::cout << "  --foreground        Run in foreground (default)" << std::endl;
            std::cout << "  --daemon            Run as daemon" << std::endl;
            std::cout << "  --quant FILE        Process quantitative data file" << std::endl;
            std::cout << "  --output FILE       Output file for quant results (default: stdout)" << std::endl;
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
    std::string quant_file;
    std::string output_file;

    // Parse command line arguments
    if (!parseCmdArgs(argc, argv, daemon_mode, quant_file, output_file))
    {
        return 0;  // Help message was shown
    }

    // If --quant option is provided, process the file and exit
    if (!quant_file.empty())
    {
        try
        {
            // Create data parser
            sep::DataParser parser;
            
            // Parse the data file (auto-detects format)
            auto patterns = parser.parseFile(quant_file);
            
            if (patterns.empty())
            {
                nlohmann::json error_json;
                error_json["error"] = "No patterns parsed from file";
                error_json["file"] = quant_file;
                std::cerr << error_json.dump() << std::endl;
                return 1;
            }
            
            // Process patterns to calculate basic metrics
            for (auto& pattern : patterns)
            {
                // Calculate simple volatility metric from OHLC
                float range = pattern.position.y - pattern.position.z; // high - low
                float avg_price = (pattern.position.x + pattern.position.w) / 2.0f; // (open + close) / 2
                float volatility = (range / avg_price) * 100.0f; // percentage
                
                // Simple coherence calculation (inverse of volatility)
                pattern.coherence = 1.0f / (1.0f + volatility * 0.01f);
                pattern.quantum_state.coherence = pattern.coherence;
                pattern.quantum_state.stability = pattern.coherence;
                pattern.quantum_state.energy = volatility;
            }
            
            // Build DAG for correlations
            sep::dag::DagGraph dag;
            
            // Add patterns to DAG
            for (const auto& pattern : patterns)
            {
                std::vector<uint64_t> parents; // No parents for initial patterns
                
                // Extract position as vec3 for DAG
                glm::vec3 pos(pattern.position.x, pattern.position.y, pattern.position.z);
                
                // Add to DAG with market data if available
                if (!pattern.data.empty())
                {
                    float volume = pattern.data[0];
                    dag.addMarketDataNode(pos, pattern.coherence, pattern.position.w,
                                         0.0f, volume, parents);
                }
                else
                {
                    dag.addNode(pos, pattern.coherence, parents);
                }
            }
            
            // Calculate correlations and metrics
            dag.calculateNodeCorrelations();
            dag.calculateTailRisk();
            dag.calculateAlpha();
            
            // Export results as JSON
            std::string result = dag.exportAsJson();
            
            // Add processing metadata
            nlohmann::json metadata;
            metadata["patterns_processed"] = patterns.size();
            metadata["file"] = quant_file;
            
            // Parse existing result and add metadata
            nlohmann::json final_json = nlohmann::json::parse(result);
            final_json["metadata"] = metadata;
            
            result = final_json.dump(2); // Pretty print with 2-space indent
            
            // Output results
            if (!output_file.empty())
            {
                std::ofstream out(output_file);
                if (out.is_open())
                {
                    out << result;
                    out.close();
                    std::cout << "Results written to: " << output_file << std::endl;
                }
                else
                {
                    std::cerr << "Error: Could not open output file: " << output_file << std::endl;
                    return 1;
                }
            }
            else
            {
                // Output to stdout
                std::cout << result << std::endl;
            }
            
            return 0;
        }
        catch (const std::exception& e)
        {
            std::cerr << "Error processing quant data: " << e.what() << std::endl;
            return 1;
        }
    }

    // Initialize config manager with command line args
    auto& configMgr = sep::config::ConfigManager::getInstance();
    configMgr.initialize(argc, argv);

    // Create auth config
    sep::api::AuthConfig auth_config = createDefaultConfig();

    // Create and start API server
    std::unique_ptr<sep::api::SEPApiServer> server =
        std::make_unique<sep::api::SEPApiServer>(auth_config, nullptr);  // Removed demo renderer

    std::cout << "Starting SEP API Server on port " << auth_config.port << std::endl;

    if (!server->run())
    {
        std::cerr << "Failed to start SEP API Server" << std::endl;
        return 1;
    }

    if (daemon_mode)
    {
        // In daemon mode, block indefinitely until a signal is received
        (void)std::signal(SIGINT, [](int) {
            std::cout << "Received SIGINT, shutting down..." << std::endl;
            exit(0);
        });
        (void)std::signal(SIGTERM, [](int) {
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
