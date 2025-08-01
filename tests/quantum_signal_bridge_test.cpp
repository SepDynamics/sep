#include <gtest/gtest.h>
#include "quantum/bitspace/forward_window_result.h"
#include "apps/oanda_trader/quantum_signal_bridge.hpp"
#include <torch/torch.h>

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

    std::vector<sep::quantum::bitspace::ForwardWindowResult> forward_window_results;
    // Populate with dummy data
    for (int i = 0; i < 100; ++i) {
        sep::quantum::bitspace::ForwardWindowResult result;
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

TEST(QuantumSignalBridgeTest, NeuralEnsemble) {
    // Create a simple neural network
    struct Net : torch::nn::Module {
        Net() {
            fc1 = register_module("fc1", torch::nn::Linear(3, 16));
            fc2 = register_module("fc2", torch::nn::Linear(16, 3));
        }

        torch::Tensor forward(torch::Tensor x) {
            x = torch::relu(fc1->forward(x));
            x = torch::log_softmax(fc2->forward(x), 1);
            return x;
        }

        torch::nn::Linear fc1{nullptr}, fc2{nullptr};
    };

    auto net = std::make_shared<Net>();
    torch::Tensor input = torch::randn({1, 3});
    torch::Tensor output = net->forward(input);

    ASSERT_EQ(output.size(0), 1);
    ASSERT_EQ(output.size(1), 3);
}
