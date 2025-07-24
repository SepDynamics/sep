#include "apps/workbench/core/trade_manager.h"

namespace sep {
namespace workbench {

TradeManager::TradeManager(sep::connectors::OandaConnector* connector)
    : oanda_connector_(connector) {}

nlohmann::json TradeManager::placeOrder(const std::string& instrument,
                                        double units,
                                        double current_price,
                                        double stop_loss_pips) {
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

    return oanda_connector_->placeOrder(order_details);
}

} // namespace workbench
} // namespace sep