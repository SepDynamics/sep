#include <gtest/gtest.h>
#include "apps/workbench/core/signal_validator.h"
#include "quantum/pattern_metric_engine.h"

using namespace sep::workbench;

TEST(SignalValidator, BuySignalCorrect) {
    std::vector<sep::quantum::Signal> signals(1);
    signals[0].type = sep::quantum::SignalType::BUY;
    signals[0].confidence = 1.0f;
    std::vector<float> prices = {1.0f, 1.1f};

    SignalValidator validator;
    auto result = validator.validate_signal(signals, prices);
    EXPECT_DOUBLE_EQ(result.predictive_accuracy, 1.0);
    EXPECT_DOUBLE_EQ(result.false_positive_rate, 0.0);
}

TEST(SignalValidator, SellSignalIncorrect) {
    std::vector<sep::quantum::Signal> signals(1);
    signals[0].type = sep::quantum::SignalType::SELL;
    signals[0].confidence = 1.0f;
    std::vector<float> prices = {1.0f, 1.1f};

    SignalValidator validator;
    auto result = validator.validate_signal(signals, prices);
    EXPECT_DOUBLE_EQ(result.predictive_accuracy, 0.0);
    EXPECT_DOUBLE_EQ(result.false_positive_rate, 1.0);
}
