#include "apps/workbench/core/trade_manager.h"
#include <algorithm>
#include <chrono>
#include <thread>

namespace sep {
namespace workbench {

TradeManager::TradeManager(sep::connectors::OandaConnector* connector)
    : oanda_connector_(connector)
{
    starting_balance_ = account_balance_;
    sep::logging::LoggerConfig config;
    config.file.path = "trades.log";
    logger_ = sep::logging::Manager::getInstance().createLogger("trade_manager", config);
}

TradeManager::~TradeManager()
{
    stopPaperTrading();
}

void TradeManager::setPaperTrading(bool paper_trading) {
    paper_trading_ = paper_trading;
    if (paper_trading_) {
        if (!paper_instrument_.empty()) {
            startPaperTrading(paper_instrument_);
        }
    } else {
        stopPaperTrading();
    }
}

nlohmann::json TradeManager::placeOrder(const std::string& instrument,
                                        double units,
                                        double current_price,
                                        double stop_loss_pips,
                                        double take_profit_pips) {
    double risk_amount = account_balance_ * risk_percentage_;
    double pips_to_risk = stop_loss_pips;
    double pip_value = 0.0001;
    double position_size = risk_amount / (pips_to_risk * pip_value);
    if (units == 0) {
        units = position_size;
    }
    if (paper_trading_) {
        std::lock_guard<std::mutex> lock(mutex_);
        Order order;
        order.id = "paper_trade_" + std::to_string(orders_.size() + 1);
        order.instrument = instrument;
        order.units = units;
        order.price = current_price;
        order.state = OrderState::FILLED;
        orders_.push_back(order);
        updatePositions(order);
        sep::logging::logTrade(instrument, units, current_price, 0.0);
        paper_instrument_ = instrument;
        if (paper_trading_ && !paper_thread_active_) {
            startPaperTrading(instrument);
        }
        return {{"paper_trade", "success"}};
    }

    if (!oanda_connector_) {
        return {{"error", "OANDA connector not initialized"}};
    }

    double stop_loss_price = (units > 0)
                                 ? (current_price - (stop_loss_pips * 0.0001))
                                 : (current_price + (stop_loss_pips * 0.0001));

    nlohmann::json order_details;
    order_details["order"]["instrument"] = instrument;
    order_details["order"]["units"] = std::to_string(units);
    order_details["order"]["type"] = "MARKET";
    order_details["order"]["timeInForce"] = "FOK";
    order_details["order"]["positionFill"] = "DEFAULT";
    order_details["order"]["stopLossOnFill"]["price"] = std::to_string(stop_loss_price);
    order_details["order"]["stopLossOnFill"]["timeInForce"] = "GTC";

    if (take_profit_pips > 0) {
        double take_profit_price = (units > 0)
                                     ? (current_price + (take_profit_pips * 0.0001))
                                     : (current_price - (take_profit_pips * 0.0001));
        order_details["order"]["takeProfitOnFill"]["price"] = std::to_string(take_profit_price);
        order_details["order"]["takeProfitOnFill"]["timeInForce"] = "GTC";
    }

    nlohmann::json result = oanda_connector_->placeOrder(order_details);

    if (result.contains("orderFillTransaction")) {
        std::lock_guard<std::mutex> lock(mutex_);
        Order order;
        order.id = result["orderFillTransaction"]["id"];
        order.instrument = instrument;
        order.units = units;
        order.price = std::stod(result["orderFillTransaction"]["price"].get<std::string>());
        order.state = OrderState::FILLED;
        orders_.push_back(order);
        updatePositions(order);
        sep::logging::logTrade(instrument, units, order.price, 0.0);
    }

    return result;
}

void TradeManager::updateOrderStatus(const std::string& order_id, OrderState state) {
    std::lock_guard<std::mutex> lock(mutex_);
    auto it = std::find_if(orders_.begin(), orders_.end(),
                           [&](const Order& o) { return o.id == order_id; });
    if (it != orders_.end()) {
        it->state = state;
    }
}

const std::vector<Order>& TradeManager::getOrders() const {
    return orders_;
}

const std::vector<Position>& TradeManager::getPositions() const {
    return positions_;
}

void TradeManager::updatePositions(const Order& order) {
    auto it = std::find_if(positions_.begin(), positions_.end(),
                           [&](const Position& p) { return p.instrument == order.instrument; });

    if (it != positions_.end()) {
        double prev_units = it->size;
        double prev_avg = it->average_price;
        it->size += order.units;
        if ((prev_units > 0 && it->size < prev_units) || (prev_units < 0 && it->size > prev_units)) {
            // closing part of position
            double close_units = order.units;
            double pnl = (order.price - prev_avg) * close_units;
            if (prev_units < 0) pnl = -pnl;
            realized_pnl_ += pnl;
            account_balance_ += pnl;
            if (pnl > 0)
                win_count_++;
            else if (pnl < 0)
                loss_count_++;
            sep::logging::logTrade(order.instrument, order.units, order.price, pnl);
            double roi = getROI();
            if (roi < -0.05)
                sep::logging::logAnomaly("ROI below -5%");
        }
        double existing_value = prev_units * prev_avg;
        double new_value = order.units * order.price;
        if (it->size == 0) {
            it->average_price = 0;
        } else {
            it->average_price = (existing_value + new_value) / it->size;
        }
    } else {
        Position new_position(order.instrument, order.units, order.price);
        positions_.push_back(new_position);
        account_balance_ -= order.units * order.price;
    }
}

void TradeManager::startPaperTrading(const std::string& instrument)
{
    paper_instrument_ = instrument;
    if (paper_thread_active_) return;
    paper_thread_active_ = true;
    paper_thread_ = std::thread([this]() {
        while (paper_thread_active_) {
            if (oanda_connector_) {
                auto md = oanda_connector_->getMarketData(paper_instrument_);
                std::lock_guard<std::mutex> lock(mutex_);
                for (auto& pos : positions_) {
                    pos.current_price = (pos.size > 0) ? md.bid : md.ask;
                    pos.unrealized_pl = (pos.current_price - pos.average_price) * pos.size;
                }
            }
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    });
}

void TradeManager::stopPaperTrading()
{
    paper_thread_active_ = false;
    if (paper_thread_.joinable())
        paper_thread_.join();
}

double TradeManager::getWinLossRatio() const
{
    int total = win_count_ + loss_count_;
    if (total == 0) return 0.0;
    return static_cast<double>(win_count_) / total;
}

double TradeManager::getROI() const
{
    if (starting_balance_ == 0.0) return 0.0;
    return realized_pnl_ / starting_balance_;
}

} // namespace workbench
} // namespace sep