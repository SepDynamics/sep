#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <cstring>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <filesystem>
#include <thread>
#include "connectors/oanda_connector.h"
#include "engine/internal/data_parser.h"

using sep::DataParser;

void printUsage() {
    std::cout << "Usage: oanda_historical_fetcher [options]" << std::endl;
    std::cout << "Options:" << std::endl;
    std::cout << "  --instrument <PAIR>     Currency pair (default: EUR_USD)" << std::endl;
    std::cout << "  --granularity <GRAN>    Timeframe M1/M5/M15/H1/H4/D (default: M1)" << std::endl;
    std::cout << "  --hours <N>             Hours of historical data (default: 168)" << std::endl;
    std::cout << "  --output <FILE>         Output file path (auto-generated if not specified)" << std::endl;
    std::cout << "  --cache-dir <DIR>       Cache directory (default: /sep/cache/oanda_historical/)" << std::endl;
    std::cout << "  --verbose               Verbose output" << std::endl;
    std::cout << "  --help                  Show this help message" << std::endl;
}

struct FetchConfig {
    std::string instrument = "EUR_USD";
    std::string granularity = "M1";
    int hours = 168;  // 1 week
    std::string output_file = "";
    std::string cache_dir = "/sep/cache/oanda_historical/";
    bool verbose = false;
};

FetchConfig parseArguments(int argc, char* argv[]) {
    FetchConfig config;
    
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--help") == 0) {
            printUsage();
            exit(0);
        }
        else if (strcmp(argv[i], "--instrument") == 0 && i + 1 < argc) {
            config.instrument = argv[++i];
        }
        else if (strcmp(argv[i], "--granularity") == 0 && i + 1 < argc) {
            config.granularity = argv[++i];
        }
        else if (strcmp(argv[i], "--hours") == 0 && i + 1 < argc) {
            config.hours = std::stoi(argv[++i]);
        }
        else if (strcmp(argv[i], "--output") == 0 && i + 1 < argc) {
            config.output_file = argv[++i];
        }
        else if (strcmp(argv[i], "--cache-dir") == 0 && i + 1 < argc) {
            config.cache_dir = argv[++i];
        }
        else if (strcmp(argv[i], "--verbose") == 0) {
            config.verbose = true;
        }
    }
    
    return config;
}

std::string formatTimestamp(const std::chrono::system_clock::time_point& tp) {
    auto time_t = std::chrono::system_clock::to_time_t(tp);
    std::stringstream ss;
    ss << std::put_time(std::gmtime(&time_t), "%Y-%m-%dT%H:%M:%SZ");
    return ss.str();
}

int main(int argc, char* argv[]) {
    FetchConfig config = parseArguments(argc, argv);
    
    std::cout << "🚀 OANDA Historical Data Fetcher" << std::endl;
    std::cout << "================================" << std::endl;
    std::cout << "📊 Instrument: " << config.instrument << std::endl;
    std::cout << "⏰ Granularity: " << config.granularity << std::endl;
    std::cout << "📅 Hours: " << config.hours << std::endl;
    std::cout << "💾 Cache Dir: " << config.cache_dir << std::endl;
    
    try {
        // Initialize OANDA connector with proper environment variables
        const char* api_key = std::getenv("OANDA_API_KEY");
        const char* account_id = std::getenv("OANDA_ACCOUNT_ID");
        
        if (!api_key || !account_id) {
            std::cerr << "❌ OANDA_API_KEY and OANDA_ACCOUNT_ID environment variables must be set." << std::endl;
            std::cerr << "   Run: source OANDA.env" << std::endl;
            return 1;
        }
        
        std::cout << "✅ OANDA credentials loaded from environment" << std::endl;
        
        auto oanda_connector = std::make_unique<sep::connectors::OandaConnector>(api_key, account_id);
        
        // Initialize the connector
        if (!oanda_connector->initialize()) {
            std::cerr << "❌ Failed to initialize OANDA connector" << std::endl;
            return 1;
        }
        
        // Create cache directory
        std::filesystem::create_directories(config.cache_dir);
        
        // Calculate time range
        auto now = std::chrono::system_clock::now();
        auto start_time = now - std::chrono::hours(config.hours);
        
        std::string from_str = formatTimestamp(start_time);
        std::string to_str = formatTimestamp(now);
        
        if (config.verbose) {
            std::cout << "🕐 Time Range: " << from_str << " to " << to_str << std::endl;
        }
        
        std::cout << "📥 Fetching historical data from OANDA..." << std::endl;
        
        // For large time ranges, we need to fetch in chunks due to OANDA API limits
        // Maximum ~5000 candles per request for M1 data (~3.5 days)
        std::vector<sep::connectors::OandaCandle> candles;
        
        if (config.hours > 72) {  // More than 3 days, use chunked requests
            std::cout << "📦 Large time range detected - using chunked requests..." << std::endl;
            
            int chunk_hours = 48;  // 48 hours per chunk (~2880 M1 candles)
            int chunks_needed = (config.hours + chunk_hours - 1) / chunk_hours;
            
            for (int i = 0; i < chunks_needed; i++) {
                auto chunk_start = start_time + std::chrono::hours(i * chunk_hours);
                auto chunk_end = std::min(chunk_start + std::chrono::hours(chunk_hours), now);
                
                std::string chunk_from = formatTimestamp(chunk_start);
                std::string chunk_to = formatTimestamp(chunk_end);
                
                if (config.verbose) {
                    std::cout << "  📦 Chunk " << (i+1) << "/" << chunks_needed 
                             << ": " << chunk_from << " to " << chunk_to << std::endl;
                }
                
                auto chunk_candles = oanda_connector->getHistoricalData(
                    config.instrument,
                    config.granularity, 
                    chunk_from,
                    chunk_to
                );
                
                if (chunk_candles.empty()) {
                    std::cerr << "⚠️ Warning: Chunk " << (i+1) << " returned no data" << std::endl;
                    continue;
                }
                
                // Remove duplicates when concatenating chunks (overlapping timestamps)
                for (const auto& candle : chunk_candles) {
                    // Check if this timestamp already exists
                    bool duplicate = false;
                    for (const auto& existing : candles) {
                        if (existing.time == candle.time) {
                            duplicate = true;
                            break;
                        }
                    }
                    if (!duplicate) {
                        candles.push_back(candle);
                    }
                }
                
                // Small delay between requests to be respectful to API
                if (i < chunks_needed - 1) {
                    std::this_thread::sleep_for(std::chrono::milliseconds(100));
                }
            }
        } else {
            // Single request for smaller time ranges
            candles = oanda_connector->getHistoricalData(
                config.instrument,
                config.granularity, 
                from_str,
                to_str
            );
        }
        
        if (candles.empty()) {
            std::cerr << "❌ No data received from OANDA API" << std::endl;
            std::cerr << "   Error: " << oanda_connector->getLastError() << std::endl;
            return 1;
        }
        
        std::cout << "✅ Received " << candles.size() << " candles from OANDA" << std::endl;
        
        // Convert to proper format
        std::vector<sep::common::CandleData> processed_candles;
        processed_candles.reserve(candles.size());
        
        for (const auto& candle : candles) {
            sep::common::CandleData cd;
            cd.timestamp = std::chrono::duration_cast<std::chrono::seconds>(
                sep::common::parseTimestamp(candle.time).time_since_epoch()
            ).count();
            cd.open = static_cast<float>(candle.open);
            cd.high = static_cast<float>(candle.high);
            cd.low = static_cast<float>(candle.low);
            cd.close = static_cast<float>(candle.close);
            cd.volume = candle.volume;
            processed_candles.push_back(cd);
        }
        
        // Generate output filename if not specified
        if (config.output_file.empty()) {
            auto now_t = std::chrono::system_clock::to_time_t(now);
            std::stringstream ss;
            ss << config.cache_dir << "/" 
               << config.instrument << "_" 
               << config.granularity << "_"
               << config.hours << "H_"
               << std::put_time(std::gmtime(&now_t), "%Y%m%d_%H%M%S")
               << ".json";
            config.output_file = ss.str();
        }
        
        std::cout << "💾 Saving to: " << config.output_file << std::endl;
        
        // Save processed data
        DataParser parser;
        parser.saveValidatedCandlesJSON(processed_candles, config.output_file);
        
        std::cout << "✅ Historical data saved successfully" << std::endl;
        
        // Validation summary
        if (config.verbose) {
            std::cout << "\n📊 Data Summary:" << std::endl;
            std::cout << "  • Total candles: " << candles.size() << std::endl;
            std::cout << "  • First candle: " << candles.front().time << std::endl;
            std::cout << "  • Last candle: " << candles.back().time << std::endl;
            std::cout << "  • Price range: " << candles.front().open << " to " << candles.back().close << std::endl;
            
            // Calculate basic stats
            double min_price = candles[0].low;
            double max_price = candles[0].high;
            long total_volume = 0;
            
            for (const auto& candle : candles) {
                min_price = std::min(min_price, candle.low);
                max_price = std::max(max_price, candle.high);
                total_volume += candle.volume;
            }
            
            std::cout << "  • Min/Max price: " << min_price << " / " << max_price << std::endl;
            std::cout << "  • Total volume: " << total_volume << std::endl;
        }
        
        std::cout << "\n🎯 Ready for analysis with " << config.instrument << " data!" << std::endl;
        return 0;
        
    } catch (const std::exception& e) {
        std::cerr << "🔥 Exception: " << e.what() << std::endl;
        return 1;
    }
}
