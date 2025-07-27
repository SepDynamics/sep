#include "backtester_engine.h"

#include <algorithm>
#include <iostream>
#include <numeric>
#include <vector>

#include "apps/workbench/backtester/strategies/sep_signal_strategy.h"

BacktesterEngine::BacktesterEngine() {
}

BacktesterEngine::~BacktesterEngine() {
}

sep::workbench::backtester::BacktestResult BacktesterEngine::run(const std::string& dataset_path) {
    sep::quantum::PatternMetricEngine engine;
    if (engine.init(nullptr) != sep::SEPResult::SUCCESS) {
        std::cerr << "Failed to initialize PatternMetricEngine" << std::endl;
        return {};
    }
    SEPSignalStrategy strategy;
    return run(dataset_path, &engine, &strategy);
}

sep::workbench::backtester::BacktestResult BacktesterEngine::run(
    const std::string& dataset_path,
    sep::quantum::PatternMetricEngine* engine,
    BaseStrategy* strategy) {
    sep::workbench::backtester::DataLoader loader;
    if (dataset_path == "EURUSD_48H") {
        loader.load_48h_sample();
    } else {
        loader.load_data(dataset_path);
    }

    const auto& candles = loader.get_data();
    sep::workbench::backtester::BacktestResult result{};
    if (candles.empty()) {
        return result;
    }

    if (!engine) {
        return result;
    }
    sep::quantum::PatternMetricEngine& engine_ref = *engine;
    std::vector<uint8_t> byte_stream;
    byte_stream.reserve(candles.size() * sizeof(sep::common::CandleData));
    for (const auto& candle : candles) {
        const uint8_t* bytes = reinterpret_cast<const uint8_t*>(&candle);
        byte_stream.insert(byte_stream.end(), bytes, bytes + sizeof(sep::common::CandleData));
    }

    engine_ref.ingestData(byte_stream.data(), byte_stream.size());
    engine_ref.evolvePatterns();
    engine_ref.computeMetrics();

    std::vector<sep::quantum::Signal> engine_signals = engine_ref.getSignals();
    std::vector<sep::quantum::Signal> signals = engine_signals;
    if (strategy) {
        std::vector<sep::quantum::Signal> strat_signals =
            strategy->execute(candles, engine_signals);
        if (!strat_signals.empty()) {
            size_t merge_size =
                std::min(strat_signals.size(), engine_signals.size());
            for (size_t i = 0; i < merge_size; ++i) {
                if (strat_signals[i].type != sep::quantum::SignalType::HOLD) {
                    signals[i] = strat_signals[i];
                }
            }
            if (strat_signals.size() > engine_signals.size()) {
                signals.insert(signals.end(), strat_signals.begin() + merge_size,
                               strat_signals.end());
            }
        }
    }
    sep::workbench::backtester::Backtester backtester;
    backtester.run(signals, candles);
    return backtester.getResult();
}

sep::workbench::backtester::BacktestResult BacktesterEngine::run(
    const std::string& dataset_path,
    sep::quantum::PatternMetricEngine* engine) {
    SEPSignalStrategy default_strategy;
    return run(dataset_path, engine, &default_strategy);
}

sep::workbench::backtester::BacktestResult BacktesterEngine::run(
    const std::vector<sep::quantum::Signal>& signals,
    const std::string& dataset_path) {
    sep::workbench::backtester::DataLoader loader;
    if (dataset_path == "EURUSD_48H") {
        loader.load_48h_sample();
    } else {
        loader.load_data(dataset_path);
    }

    sep::workbench::backtester::Backtester backtester;
    backtester.run(signals, loader.get_data());
    return backtester.getResult();
}
