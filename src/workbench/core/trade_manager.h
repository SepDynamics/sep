#pragma once

#include "connectors/oanda_connector.h"
#include <nlohmann/json.hpp>
#include <string>

namespace sep {
namespace workbench {
namespace core {

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

} // namespace core
} // namespace workbench
} // namespace sep