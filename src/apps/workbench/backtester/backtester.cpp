#include "backtester.h"
#include <iostream>
#include <numeric>
#include <cmath>
#include <algorithm>

namespace sep::workbench::backtester {

Backtester::Backtester() {
    result_ = {};
}

void Backtester::run(const std::vector<float>& prices, const std::vector<MetricsMonitor::ThresholdSignal>& signals) {
    if (prices.empty() || signals.empty()) {
        return;
    }

    std::vector<float> pnl;
    int wins = 0;
    int losses = 0;

    for (size_t i = 0; i < signals.size(); ++i) {
        if (signals[i].signal_type == MetricsMonitor::ThresholdSignal::BUY) {
            if (i + 1 < prices.size()) {
                Trade trade;
                trade.type = SignalType::BUY;
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
        } else if (signals[i].signal_type == MetricsMonitor::ThresholdSignal::SELL) {
            if (i + 1 < prices.size()) {
                Trade trade;
                trade.type = SignalType::SELL;
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
    for (float p : pnl) {
        current_pnl += p;
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