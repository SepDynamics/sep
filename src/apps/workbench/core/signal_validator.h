#pragma once

#include <vector>
#include "quantum/pattern_metric_engine.h"

namespace sep {
namespace workbench {

class SignalValidator {
public:
    struct ValidationResult {
        double predictive_accuracy;
        double false_positive_rate;
    };

    ValidationResult validate_signal(const std::vector<sep::quantum::Signal>& signals, const std::vector<float>& prices) {
        // Real signal validation implementation (TODO.md Phase 2.2)
        if (signals.empty() || prices.empty() || signals.size() != prices.size()) {
            return {0.0, 1.0}; // No data or mismatched sizes
        }
        
        int correct_predictions = 0;
        int total_predictions = 0;
        int false_positives = 0;
        int total_signals = 0;
        
        for (size_t i = 0; i < signals.size() - 1; i++) {
            const auto& current_signal = signals[i];
            float current_price = prices[i];
            float next_price = prices[i + 1];
            
            // Calculate actual price movement
            float price_change = (next_price - current_price) / current_price;
            bool actual_bullish = price_change > 0.0001f; // 1 pip threshold
            bool actual_bearish = price_change < -0.0001f;
            
            // Check signal predictions based on confidence
            if (current_signal.confidence > 0.7f) { // Strong signal threshold
                total_signals++;
                
                bool predicted_bullish = current_signal.type == sep::quantum::SignalType::BUY;
                bool predicted_bearish = current_signal.type == sep::quantum::SignalType::SELL;
                
                if (predicted_bullish || predicted_bearish) {
                    total_predictions++;
                    
                    // Check if prediction was correct
                    if ((predicted_bullish && actual_bullish) || 
                        (predicted_bearish && actual_bearish)) {
                        correct_predictions++;
                    } else if ((predicted_bullish && actual_bearish) || 
                               (predicted_bearish && actual_bullish)) {
                        false_positives++;
                    }
                }
            }
        }
        
        double predictive_accuracy = total_predictions > 0 ? 
            static_cast<double>(correct_predictions) / total_predictions : 0.0;
        double false_positive_rate = total_signals > 0 ? 
            static_cast<double>(false_positives) / total_signals : 0.0;
            
        return {predictive_accuracy, false_positive_rate};
    }
};

}
}
