#pragma once

#include "connectors/oanda_connector.h"
#include <nlohmann/json.hpp>
#include <string>
#include <functional>
#include <vector>
#include <mutex>
#include <chrono>
#include <thread>
#include <atomic>
#include <memory>
#include "engine/logging.h"

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

struct RiskConfig {
    double stop_loss_pips{20.0};
    double take_profit_pips{0.0};
};

class TradeManager {
public:
    TradeManager(sep::connectors::OandaConnector* connector);
    ~TradeManager();

    nlohmann::json placeOrder(const std::string& instrument,
                              double units,
                              double current_price,
                              double stop_loss_pips,
                              double take_profit_pips);

    nlohmann::json executeBuy(const std::string& instrument, double units);
    nlohmann::json executeSell(const std::string& instrument, double units);
    nlohmann::json executeOrder(const std::string& instrument, double units);

    void setRiskConfig(const RiskConfig& cfg) { risk_config_ = cfg; }
    const RiskConfig& getRiskConfig() const { return risk_config_; }

    void setAccountBalance(double balance) { account_balance_ = balance; }
    double getAccountBalance() const { return account_balance_; }
    void setRiskPercentage(double pct) { risk_percentage_ = pct; }
    double getRiskPercentage() const { return risk_percentage_; }
    void setMaxExposurePct(double pct) { max_exposure_pct_ = pct; }
    double getMaxExposurePct() const { return max_exposure_pct_; }

    void updateOrderStatus(const std::string& order_id, OrderState state);
    const std::vector<Order>& getOrders() const;
    const std::vector<Position>& getPositions() const;

    double getRealizedPnL() const { return realized_pnl_; }

    void setPaperTrading(bool paper_trading);

    // Continuous paper trading controls
    void startPaperTrading(const std::string& instrument);
    void stopPaperTrading();

    // Performance metrics
    double getWinLossRatio() const;
    double getROI() const;

    const std::vector<double>& getROIHistory() const { return roi_history_; }
    const std::vector<double>& getWinLossHistory() const { return win_loss_history_; }
    const std::vector<double>& getBalanceHistory() const { return balance_history_; }

private:
    void updatePositions(const Order& order);

    sep::connectors::OandaConnector* oanda_connector_;
    std::vector<Order> orders_;
    std::vector<Position> positions_;
    std::shared_ptr<spdlog::logger> logger_;
    std::atomic<bool> paper_thread_active_{false};
    std::string paper_instrument_;
    std::mutex mutex_;
    double account_balance_ = 100000.0;
    double starting_balance_ = 100000.0;
    double realized_pnl_ = 0.0;
    double risk_percentage_ = 0.02;
    double max_exposure_pct_ = 0.02;
    int win_count_ = 0;
    int loss_count_ = 0;
    bool paper_trading_ = false;

    std::vector<double> roi_history_;
    std::vector<double> win_loss_history_;
    std::vector<double> balance_history_;

    RiskConfig risk_config_;
};

} // namespace workbench
} // namespace sep