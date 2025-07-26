#include "backtester_engine.h"

#include <algorithm>
#include <iostream>
#include <numeric>
#include <vector>

BacktesterEngine::BacktesterEngine() {
}

BacktesterEngine::~BacktesterEngine() {
}

sep::workbench::backtester::BacktestResult BacktesterEngine::run(const std::string& dataset_path) {
    sep::workbench::backtester::DataLoader loader;
    loader.loadData(dataset_path);

    const auto& candles = loader.getCandleData();
    sep::workbench::backtester::BacktestResult result{};
    if (candles.empty()) {
        return result;
    }

    sep::quantum::PatternMetricEngine engine;
    std::vector<uint8_t> byte_stream;
    byte_stream.reserve(candles.size() * sizeof(sep::workbench::CandleData));
    for (const auto& candle : candles) {
        const uint8_t* bytes = reinterpret_cast<const uint8_t*>(&candle);
        byte_stream.insert(byte_stream.end(), bytes, bytes + sizeof(sep::workbench::CandleData));
    }

    engine.ingestData(byte_stream.data(), byte_stream.size());
    engine.evolvePatterns();
    engine.computeMetrics();

    const auto& signals = engine.getSignals();
    std::vector<float> prices;
    prices.reserve(candles.size());
    for (const auto& candle : candles) {
        prices.push_back(static_cast<float>(candle.close));
    }

    std::vector<float> pnl;
    int wins = 0;
    int losses = 0;
    size_t count = std::min(signals.size(), prices.size() - 1);
    for (size_t i = 0; i < count; ++i) {
        if (signals[i].type == sep::quantum::SignalType::BUY) {
            float entry = prices[i];
            float exit = prices[i + 1];
            pnl.push_back(exit - entry);
            wins += exit > entry;
            losses += exit <= entry;
        } else if (signals[i].type == sep::quantum::SignalType::SELL) {
            float entry = prices[i];
            float exit = prices[i + 1];
            pnl.push_back(entry - exit);
            wins += entry > exit;
            losses += entry <= exit;
        }
    }

    result.total_trades = static_cast<int>(pnl.size());
    if (result.total_trades > 0) {
        result.win_rate = static_cast<float>(wins) / result.total_trades;
        result.total_pnl = std::accumulate(pnl.begin(), pnl.end(), 0.0f);
        result.sharpe_ratio = PerformanceMetrics::computeSharpeRatio(pnl);
        result.max_drawdown = PerformanceMetrics::computeMaxDrawdown(pnl);
    }

    return result;
}
