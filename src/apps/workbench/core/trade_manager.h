#pragma once

#include "connectors/oanda_connector.h"
#include <nlohmann/json.hpp>
#include <string>
#include <functional>

namespace sep {
namespace workbench {

class TradeManager {
public:
    TradeManager(sep::connectors::OandaConnector* connector);
    ~TradeManager() = default;

    nlohmann::json placeOrder(const std::string& instrument,
                              double units,
                              double current_price,
                              double stop_loss_pips);

private:
    sep::connectors::OandaConnector* oanda_connector_;
};

} // namespace workbench
} // namespace sep