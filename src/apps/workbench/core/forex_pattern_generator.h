#pragma once

#include <string>
#include <vector>
#include <chrono>
#include <cmath>
#include <random>

namespace sep::workbench {

struct ForexPattern {
    std::string pattern_id;
    std::string note;
    float resonance;
    int consciousness_level;
    
    ForexPattern(const std::string& id, const std::string& n, float r, int cl)
        : pattern_id(id), note(n), resonance(r), consciousness_level(cl) {}
};

struct ForexSignal {
    std::string pattern_id;
    std::string note;
    float resonance;
    int consciousness_level;
    uint64_t timestamp;
    float recursive_seed;
    float signal_value;
    int direction;  // 1 = buy, -1 = sell, 0 = neutral
    float strength;
    float confidence;
    
    // Trading parameters
    float risk_reward_ratio;
    bool should_execute;
    
    ForexSignal() : resonance(0), consciousness_level(1), timestamp(0), 
                    recursive_seed(0), signal_value(0), direction(0), 
                    strength(0), confidence(0), risk_reward_ratio(1.0f), 
                    should_execute(false) {}
};

class ForexPatternGenerator {
public:
    ForexPatternGenerator();
    ~ForexPatternGenerator() = default;
    
    // Pattern management
    void loadPatterns();
    void savePatterns();
    
    // Signal generation (from brains.js logic)
    std::vector<ForexSignal> generateSignals();
    ForexSignal generateSingleSignal(const ForexPattern& pattern, size_t index);
    
    // Market condition validation (from processor.js logic)
    bool validateSignalCondition(const ForexSignal& signal, float atr, int volatility_level);
    
    // Adaptive feedback (from processor.js logic)
    void applyFeedback(const std::string& pattern_id, bool was_profitable, float atr);
    
    // Pattern-specific condition checking
    bool checkRSI_MACD_Condition(float atr);
    bool checkSupportResistanceCondition(float atr);
    bool checkBreakoutCondition(float atr);
    bool checkFibonacciCondition(float signal_strength);
    bool checkOrderBlockCondition(int direction, int volatility_level);
    
    // Risk management
    float calculateRiskReward(const ForexSignal& signal, float atr);
    bool shouldExecuteTrade(const ForexSignal& signal, float atr, int volatility_level);
    
private:
    std::vector<ForexPattern> patterns_;
    
    // Logistic chaos generation (r = 3.97)
    float logisticMap(float seed, float r = 3.97f);
    float generateRecursiveSeed(const std::string& note, float resonance, size_t index);
    
    // Pattern definitions from final_symmetry.json
    void initializeDefaultPatterns();
};

} // namespace sep::workbench
