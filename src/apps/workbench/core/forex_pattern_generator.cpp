#include "forex_pattern_generator.h"
#include "../config.hpp"
#include <iostream>
#include <algorithm>
#include <fstream>
#include <filesystem>
#include <nlohmann/json.hpp>

namespace sep::workbench {

ForexPatternGenerator::ForexPatternGenerator() {
    loadPatterns();
}

ForexPatternGenerator::~ForexPatternGenerator() {
    savePatterns();
}

void ForexPatternGenerator::initializeDefaultPatterns() {
    // Patterns from final_symmetry.json in forex folder
    patterns_.clear();
    
    patterns_.emplace_back("6f91341c-7f34-4696-a68c-81ef99841129", 
                          "EURUSD_RSI_MACD_bull_divergence", 0.82f, 2);
    patterns_.emplace_back("c56d6655-888b-48ec-9103-2399463773ef", 
                          "EURUSD_double_bottom_support", 0.76f, 2);
    patterns_.emplace_back("c5356a67-53e3-435d-9f9d-0ceadca5bbb5", 
                          "EURUSD_200MA_price_crossover", 0.71f, 1);
    patterns_.emplace_back("68b8ae1e-f2ba-4f15-91a2-29a76ed09e10", 
                          "EURUSD_breakout_volume_surge", 0.81f, 2);
    patterns_.emplace_back("26a75ee3-79cf-47e7-aaf6-9a9b80b6e578", 
                          "EURUSD_fibonacci_extension_completion", 0.76f, 2);
    patterns_.emplace_back("2d921725-c96d-4eb5-9a94-cb30ce411cf8", 
                          "EURUSD_orderblock_formation", 0.83f, 2);
    patterns_.emplace_back("8fc70c52-a56f-4500-b982-a2183a47feca", 
                          "EURUSD_liquidity_sweep_rejection", 0.77f, 2);
}

float ForexPatternGenerator::logisticMap(float seed, float r) {
    // Ensure seed is between 0-1 to prevent overflow
    seed = std::max(0.0001f, std::min(0.9999f, seed));
    return r * seed * (1.0f - seed);
}

float ForexPatternGenerator::generateRecursiveSeed(const std::string& note, float resonance, size_t index) {
    // Use characters from note and resonance for initial seed
    const int base_code = static_cast<int>(note[0]);
    const float seed = fmod((base_code * resonance + index), 97.0f) / 97.0f;
    return logisticMap(seed);
}

std::vector<ForexSignal> ForexPatternGenerator::generateSignals() {
    std::vector<ForexSignal> signals;
    
    for (size_t i = 0; i < patterns_.size(); ++i) {
        ForexSignal signal = generateSingleSignal(patterns_[i], i);
        signals.push_back(signal);
    }
    
    return signals;
}

ForexSignal ForexPatternGenerator::generateSingleSignal(const ForexPattern& pattern, size_t index) {
    ForexSignal signal;
    
    signal.pattern_id = pattern.pattern_id;
    signal.note = pattern.note;
    signal.resonance = pattern.resonance;
    signal.consciousness_level = pattern.consciousness_level;
    signal.timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()).count();
    
    // Generate deterministic recursive seed
    signal.recursive_seed = generateRecursiveSeed(pattern.note, pattern.resonance, index);
    
    // Use multiple iterations of logistic map for signal value
    float signal_value = signal.recursive_seed;
    for (int i = 0; i < 5; ++i) {
        signal_value = logisticMap(signal_value);
    }
    signal.signal_value = signal_value;
    
    // Determine signal direction (1 = buy, -1 = sell, 0 = neutral)
    if (signal_value > 0.5f) {
        signal.direction = 1;
    } else if (signal_value < 0.3f) {
        signal.direction = -1;
    } else {
        signal.direction = 0;
    }
    
    // Calculate signal strength based on resonance and consciousness level
    signal.strength = pattern.resonance * (pattern.consciousness_level / 4.0f);
    
    // Calculate confidence
    signal.confidence = pattern.resonance * signal.recursive_seed;
    
    // Initialize trading parameters
    signal.risk_reward_ratio = 1.0f;
    signal.should_execute = false;
    
    return signal;
}

bool ForexPatternGenerator::validateSignalCondition(const ForexSignal& signal, float atr, int volatility_level) {
    // Extract pattern type from note
    std::string pattern_type = signal.note;
    
    // Different patterns have different matching criteria (from processor.js)
    if (pattern_type.find("RSI_MACD") != std::string::npos) {
        return checkRSI_MACD_Condition(atr);
    } else if (pattern_type.find("support") != std::string::npos || 
               pattern_type.find("resistance") != std::string::npos ||
               pattern_type.find("bottom") != std::string::npos ||
               pattern_type.find("top") != std::string::npos) {
        return checkSupportResistanceCondition(atr);
    } else if (pattern_type.find("breakout") != std::string::npos ||
               pattern_type.find("crossover") != std::string::npos) {
        return checkBreakoutCondition(atr);
    } else if (pattern_type.find("fibonacci") != std::string::npos) {
        return checkFibonacciCondition(signal.strength);
    } else if (pattern_type.find("orderblock") != std::string::npos ||
               pattern_type.find("liquidity") != std::string::npos) {
        return checkOrderBlockCondition(signal.direction, volatility_level);
    }
    
    // Default - check if signal resonance is high enough
    return signal.resonance >= 0.7f;
}

bool ForexPatternGenerator::checkRSI_MACD_Condition(float atr) {
    // RSI/MACD patterns match during trending markets (higher ATR)
    return atr > 0.007f;
}

bool ForexPatternGenerator::checkSupportResistanceCondition(float atr) {
    // Support/resistance patterns match during consolidation (lower ATR)
    return atr < 0.006f;
}

bool ForexPatternGenerator::checkBreakoutCondition(float atr) {
    // Breakout patterns match during volatile markets (higher ATR)
    return atr > 0.008f;
}

bool ForexPatternGenerator::checkFibonacciCondition(float signal_strength) {
    // Fibonacci patterns are more complex, use signal strength as indicator
    return signal_strength > 0.5f;
}

bool ForexPatternGenerator::checkOrderBlockCondition(int direction, int volatility_level) {
    // Order block patterns depend on signal direction and volatility
    return (direction > 0 && volatility_level >= 3) ||
           (direction < 0 && volatility_level <= 2);
}

float ForexPatternGenerator::calculateRiskReward(const ForexSignal& signal, float atr) {
    // Base R:R on confidence level
    float base_rr = 1.0f + signal.confidence;
    
    // Adjust based on volatility (ATR) - from processor.js logic
    float rr_multiplier;
    
    if (atr < 0.003f) {
        rr_multiplier = 0.8f; // Lower volatility = lower R:R
    } else if (atr < 0.007f) {
        rr_multiplier = 1.0f; // Medium-low volatility = normal R:R
    } else if (atr < 0.012f) {
        rr_multiplier = 1.2f; // Medium-high volatility = higher R:R
    } else {
        rr_multiplier = 1.5f; // High volatility = highest R:R
    }
    
    const float final_rr = base_rr * rr_multiplier;
    return final_rr;
}

bool ForexPatternGenerator::shouldExecuteTrade(const ForexSignal& signal, float atr, int volatility_level) {
    // Check minimum resonance threshold (from processor.js)
    if (signal.resonance < 0.55f) {
        return false;
    }
    
    // Validate market conditions for this pattern type
    if (!validateSignalCondition(signal, atr, volatility_level)) {
        return false;
    }
    
    // Calculate risk-reward and ensure it's reasonable
    float rr = calculateRiskReward(signal, atr);
    if (rr < 1.2f) {
        return false;
    }
    
    return true;
}

void ForexPatternGenerator::applyFeedback(const std::string& pattern_id, bool was_profitable, float atr) {
    // Find the pattern and adjust resonance based on trade result
    for (auto& pattern : patterns_) {
        if (pattern.pattern_id == pattern_id) {
            if (was_profitable) {
                // Increase resonance for profitable trades
                pattern.resonance += 0.03f;
            } else {
                // Decrease resonance for losing trades
                pattern.resonance -= 0.04f;
            }
            
            // Apply health protocol rules (from processor.js)
            if (pattern.resonance < 0.55f) {
                pattern.resonance = 0.55f; // Minimum floor
            }
            if (pattern.resonance > 0.95f) {
                pattern.resonance = 0.95f; // Maximum ceiling
            }
            
            // Adjust consciousness level based on ATR (volatility)
            if (atr < 0.003f) {
                pattern.consciousness_level = 1;
            } else if (atr < 0.007f) {
                pattern.consciousness_level = 2;
            } else if (atr < 0.012f) {
                pattern.consciousness_level = 3;
            } else {
                pattern.consciousness_level = 4;
            }
            
            // Promote high performing patterns (resonance > 0.75)
            if (pattern.resonance > 0.75f && pattern.consciousness_level < 4) {
                pattern.consciousness_level = std::min(4, pattern.consciousness_level + 1);
            }

            savePatterns();

            break;
        }
    }
}

void ForexPatternGenerator::loadPatterns() {
    const auto& cfg = Config::getInstance().engine();
    std::filesystem::path path = cfg.patterns_file;
    if (path.empty()) {
        initializeDefaultPatterns();
        return;
    }

    try {
        std::ifstream file(path);
        if (!file.is_open()) {
            initializeDefaultPatterns();
            return;
        }

        nlohmann::json j;
        file >> j;

        if (!j.contains("patterns")) {
            initializeDefaultPatterns();
            return;
        }

        patterns_.clear();
        for (const auto& p : j["patterns"]) {
            patterns_.emplace_back(
                p.value("patternId", ""),
                p.value("note", ""),
                p.value("resonance", 0.0f),
                p.value("consciousnessLevel", 1));
        }
    } catch (const std::exception& e) {
        std::cerr << "Failed to load patterns: " << e.what() << std::endl;
        initializeDefaultPatterns();
    }
}

void ForexPatternGenerator::savePatterns() {
    const auto& cfg = Config::getInstance().engine();
    std::filesystem::path path = cfg.patterns_file;
    if (path.empty()) {
        return;
    }

    nlohmann::json j;
    j["patterns"] = nlohmann::json::array();
    for (const auto& p : patterns_) {
        j["patterns"].push_back({
            {"patternId", p.pattern_id},
            {"note", p.note},
            {"resonance", p.resonance},
            {"consciousnessLevel", p.consciousness_level}
        });
    }
    try {
        std::ofstream file(path);
        if (!file.is_open()) {
            std::cerr << "Failed to open patterns file for writing: "
                      << path << std::endl;
            return;
        }
        file << j.dump(2);
    } catch (const std::exception& e) {
        std::cerr << "Failed to save patterns: " << e.what() << std::endl;
    }
}

} // namespace sep::workbench
