#include "alpha_tracker.h"

namespace sep::workbench {

void AlphaTracker::processSignal(const sep::quantum::Signal& signal, const sep::CandleData& current_candle) {
    if (initial_price_ == 0.0) {
        initial_price_ = current_candle.close;
    }

    benchmark_pips_ = current_candle.close - initial_price_;

    bool trade_confidence = (
        signal.signal_strength >= 0.6 &&
        signal.coherence >= 0.9 &&
        signal.stability >= 0.0
    );

    if (signal.signal_type == sep::quantum::SignalType::BUY && !in_trade_ && trade_confidence) {
        in_trade_ = true;
        buy_price_ = current_candle.close;
    } else if (signal.signal_type == sep::quantum::SignalType::SELL && in_trade_) {
        in_trade_ = false;
        strategy_pips_gained_ += (current_candle.close - buy_price_);
    }
}

void AlphaTracker::reset() {
    strategy_pips_gained_ = 0.0;
    benchmark_pips_ = 0.0;
    buy_price_ = 0.0;
    in_trade_ = false;
    initial_price_ = 0.0;
}

} // namespace sep::workbench
