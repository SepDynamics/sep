#include <gtest/gtest.h>
#include "apps/oanda_trader/quantum_signal_bridge.hpp"

TEST(QuantumSignalBridgeTest, Initialization) {
    sep::trading::QuantumSignalBridge bridge;
    ASSERT_TRUE(bridge.initialize());
}

TEST(QuantumSignalBridgeTest, SignalGeneration) {
    sep::trading::QuantumSignalBridge bridge;
    bridge.initialize();

    std::vector<sep::connectors::MarketData> history;
    for (int i = 0; i < 100; ++i) {
        sep::connectors::MarketData data;
        data.instrument = "EUR_USD";
        data.bid = 1.0 + i * 0.01;
        data.ask = 1.0 + i * 0.01;
        data.mid = 1.0 + i * 0.01;
        data.timestamp = (uint64_t)i * 1000000000ULL;
        data.volume = 100.0;
        history.push_back(data);
    }

    std::vector<sep::apps::cuda::ForwardWindowResult> forward_window_results;
    // Populate with dummy data
    for (int i = 0; i < 100; ++i) {
        sep::apps::cuda::ForwardWindowResult result;
        result.confidence = 0.8f;
        result.coherence = 0.8f;
        result.stability = 0.8f;
        forward_window_results.push_back(result);
    }

    sep::connectors::MarketData current_data;
    current_data.instrument = "EUR_USD";
    current_data.bid = 1.99;
    current_data.ask = 1.99;
    current_data.mid = 1.99;
    current_data.timestamp = 100 * 1000000000ULL;
    current_data.volume = 100.0;

    sep::trading::QuantumTradingSignal signal = bridge.analyzeMarketData(current_data, history, forward_window_results);

    ASSERT_NE(signal.action, sep::trading::QuantumTradingSignal::Action::HOLD);
}
