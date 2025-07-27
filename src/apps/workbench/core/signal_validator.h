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
        if (signals.empty() || prices.size() < signals.size() + 1) {
            return {0.0, 1.0};
        }

        int correct_predictions = 0;
        int total_predictions = 0;
        int false_positives = 0;

        for (size_t i = 0; i < signals.size() && i < prices.size() - 1; ++i) {
            const auto& current_signal = signals[i];
            float current_price = prices[i];
            float next_price = prices[i + 1];

            bool price_up = next_price > current_price;
            bool price_down = next_price < current_price;

            bool predicted_buy = current_signal.type == sep::quantum::SignalType::BUY;
            bool predicted_sell = current_signal.type == sep::quantum::SignalType::SELL;

            if (predicted_buy || predicted_sell) {
                ++total_predictions;
                if ((predicted_buy && price_up) || (predicted_sell && price_down)) {
                    ++correct_predictions;
                } else if ((predicted_buy && price_down) || (predicted_sell && price_up)) {
                    ++false_positives;
                }
            }
        }

        double predictive_accuracy = total_predictions > 0 ?
            static_cast<double>(correct_predictions) / total_predictions : 0.0;
        double false_positive_rate = total_predictions > 0 ?
            static_cast<double>(false_positives) / total_predictions : 0.0;
            
        return {predictive_accuracy, false_positive_rate};
    }
};

}
}
