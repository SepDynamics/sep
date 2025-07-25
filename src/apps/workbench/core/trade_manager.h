#pragma once

#include "connectors/oanda_connector.h"
#include <nlohmann/json.hpp>
#include <string>
#include <functional>
#include <vector>
#include <mutex>
#include <chrono>

namespace sep {
namespace workbench {

enum class OrderState { PENDING, FILLED, CANCELLED, REJECTED };

struct Order {
    std::string id;
    std::string instrument;
    double units;
    double price;
    OrderState state;
};

struct Position {
    std::string instrument;
    double size; // positive for long, negative for short
    double average_price;
    double current_price;
    double unrealized_pl;
    double realized_pl;
    std::chrono::system_clock::time_point open_time;
    
    Position(const std::string& inst, double sz, double avg_price)
        : instrument(inst), size(sz), average_price(avg_price),
          current_price(avg_price), unrealized_pl(0), realized_pl(0),
          open_time(std::chrono::system_clock::now()) {}
};

class TradeManager {
public:
    TradeManager(sep::connectors::OandaConnector* connector);
    ~TradeManager() = default;

    nlohmann::json placeOrder(const std::string& instrument,
                              double units,
                              double current_price,
                              double stop_loss_pips);

    void updateOrderStatus(const std::string& order_id, OrderState state);
    const std::vector<Order>& getOrders() const;
    const std::vector<Position>& getPositions() const;

private:
    void updatePositions(const Order& order);

    sep::connectors::OandaConnector* oanda_connector_;
    std::vector<Order> orders_;
    std::vector<Position> positions_;
    std::mutex mutex_;
};

} // namespace workbench
} // namespace sep