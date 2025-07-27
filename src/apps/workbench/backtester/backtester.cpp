#include "backtester.h"
#include "common/financial_data_types.h"
#include <iostream>
#include <numeric>
#include <cmath>
#include <algorithm>

namespace sep::workbench::backtester {

Backtester::Backtester() {
    result_ = {};
}

void Backtester::run(sep::quantum::PatternMetricEngine* engine, DataLoader* data_loader) {
    if (!engine || !data_loader) {
        return;
    }

    result_ = {};

    const auto& candles = data_loader->get_data();
    if (candles.empty()) {
        return;
    }

    std::vector<uint8_t> byte_stream;
    for (const auto& candle : candles) {
        const uint8_t* candle_bytes = reinterpret_cast<const uint8_t*>(&candle);
        byte_stream.insert(byte_stream.end(), candle_bytes, candle_bytes + sizeof(sep::common::CandleData));
    }

    engine->ingestData(byte_stream.data(), byte_stream.size());
    engine->evolvePatterns();
    engine->computeMetrics();
    const auto& signals = engine->getSignals();

    std::vector<float> prices;
    for (const auto& candle : candles) {
        prices.push_back(candle.close);
    }

    if (prices.empty() || signals.empty()) {
        return;
    }

    std::vector<float> pnl;
    int wins = 0;
    int losses = 0;

    for (size_t i = 0; i < signals.size(); ++i) {
        if (signals[i].type == sep::quantum::SignalType::BUY) {
            if (i + 1 < prices.size()) {
                Trade trade;
                trade.type = sep::quantum::SignalType::BUY;
                trade.entry_price = prices[i];
                trade.exit_price = prices[i + 1];
                trade.holding_period = 1;
                result_.trades.push_back(trade);
                pnl.push_back(trade.exit_price - trade.entry_price);
                if (trade.exit_price > trade.entry_price) {
                    wins++;
                } else {
                    losses++;
                }
            }
        } else if (signals[i].type == sep::quantum::SignalType::SELL) {
            if (i + 1 < prices.size()) {
                Trade trade;
                trade.type = sep::quantum::SignalType::SELL;
                trade.entry_price = prices[i];
                trade.exit_price = prices[i + 1];
                trade.holding_period = 1;
                result_.trades.push_back(trade);
                pnl.push_back(trade.entry_price - trade.exit_price);
                if (trade.entry_price > trade.exit_price) {
                    wins++;
                } else {
                    losses++;
                }
            }
        }
    }

    result_.total_trades = result_.trades.size();
    if (result_.total_trades > 0) {
        result_.win_rate = static_cast<float>(wins) / result_.total_trades;
    }

    result_.total_pnl = std::accumulate(pnl.begin(), pnl.end(), 0.0f);

    if (pnl.size() > 1) {
        float avg_pnl = result_.total_pnl / pnl.size();
        float std_dev = 0.0f;
        for (float p : pnl) {
            std_dev += (p - avg_pnl) * (p - avg_pnl);
        }
        std_dev = std::sqrt(std_dev / pnl.size());
        if (std_dev > 0) {
            result_.sharpe_ratio = avg_pnl / std_dev;
        }
    }

    float peak = 0.0f;
    float max_drawdown = 0.0f;
    float current_pnl = 0.0f;
    result_.equity_curve.clear();
    result_.equity_curve.push_back(current_pnl);
    for (float p : pnl) {
        current_pnl += p;
        result_.equity_curve.push_back(current_pnl);
        if (current_pnl > peak) {
            peak = current_pnl;
        }
        float drawdown = peak - current_pnl;
        if (drawdown > max_drawdown) {
            max_drawdown = drawdown;
        }
    }
    result_.max_drawdown = max_drawdown;
}

const BacktestResult& Backtester::getResult() const {
    return result_;
}

}