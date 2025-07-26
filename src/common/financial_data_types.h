#pragma once

#include <chrono>
#include <string>
#include <vector>

namespace sep {
namespace workbench {

struct TickData {
    double bid, ask, spread;
    std::chrono::system_clock::time_point timestamp;
    int volume_tick = 0;

    TickData(double b, double a, std::chrono::system_clock::time_point t)
        : bid(b), ask(a), spread(a - b), timestamp(t) {}
};

struct OrderBookLevel {
    double price;
    double size;
    int order_count;

    OrderBookLevel(double p, double s, int c = 1) : price(p), size(s), order_count(c) {}
};

struct OrderBook {
    std::vector<OrderBookLevel> bids;
    std::vector<OrderBookLevel> asks;
    std::chrono::system_clock::time_point timestamp;

    void clear() { bids.clear(); asks.clear(); }
};

struct PendingOrder {
    enum Type { MARKET, LIMIT, STOP, STOP_LIMIT };
    enum Side { BUY, SELL };

    std::string id;
    Type type;
    Side side;
    double size;
    double price;
    double stop_loss = 0.0;
    double take_profit = 0.0;
    std::chrono::system_clock::time_point created_time;
    std::string status = "PENDING";

    PendingOrder(Type t, Side s, double sz, double p)
        : type(t), side(s), size(sz), price(p),
          created_time(std::chrono::system_clock::now()) {}
};

struct TradeHistory {
    std::string id;
    std::string instrument;
    PendingOrder::Side side;
    double size;
    double entry_price;
    double exit_price;
    double pl;
    std::chrono::system_clock::time_point entry_time;
    std::chrono::system_clock::time_point exit_time;
    std::string strategy = "Manual";

    TradeHistory(const std::string& inst, PendingOrder::Side s, double sz,
                 double entry, double exit, std::chrono::system_clock::time_point et,
                 std::chrono::system_clock::time_point ext)
        : instrument(inst), side(s), size(sz), entry_price(entry), exit_price(exit),
          pl((s == PendingOrder::BUY ? (exit - entry) : (entry - exit)) * sz),
          entry_time(et), exit_time(ext) {}
};

struct CandleData {
    double open, high, low, close;
    int volume;
    std::chrono::system_clock::time_point timestamp;

    CandleData(double o, double h, double l, double c, int v,
               std::chrono::system_clock::time_point t)
        : open(o), high(h), low(l), close(c), volume(v), timestamp(t) {}
};

} // namespace workbench
} // namespace sep
