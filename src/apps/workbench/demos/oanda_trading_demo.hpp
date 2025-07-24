#pragma once

#include "../core/demo_interface.hpp"
#include "../../connectors/oanda_connector.h"
#include <memory>
#include <string>
#include <vector>

namespace sep::workbench {

struct MarketData {
    std::string instrument;
    double bid = 0.0;
    double ask = 0.0;
    double spread = 0.0;
    std::string timestamp;
};

struct Position {
    std::string instrument;
    double units = 0.0;
    double unrealized_pl = 0.0;
    double margin_used = 0.0;
};

class OandaTradingDemo : public DemoInterface {
public:
    OandaTradingDemo();
    ~OandaTradingDemo() override = default;

    void render() override;
    void update() override;
    void reset() override;
    
    std::string getName() const override { return "OANDA Trading Hub"; }
    std::string getDescription() const override { 
        return "Real-time forex trading with pattern analysis"; 
    }

private:
    // Connection management
    void renderConnectionPanel();
    void renderAccountPanel();
    void renderMarketDataPanel();
    void renderTradingPanel();
    void renderPositionsPanel();
    void renderAnalysisPanel();
    
    // OANDA integration
    void connectToOanda();
    void updateMarketData();
    void refreshAccountInfo();
    void placeOrder();
    
    // Data processing integration
    void runPatternAnalysis();
    void updateMetrics();
    
    // State
    std::unique_ptr<sep::connectors::OandaConnector> oanda_connector_;
    bool oanda_connected_ = false;
    std::string connection_status_ = "Disconnected";
    std::string account_balance_ = "N/A";
    std::string account_currency_ = "USD";
    std::string last_error_;
    
    // Market data
    std::vector<MarketData> market_data_;
    std::vector<Position> positions_;
    
    // Trading panel state
    char trade_instrument_[32] = "EUR_USD";
    float trade_units_ = 1000.0f;
    bool is_buy_order_ = true;
    
    // Analysis state
    bool pattern_analysis_running_ = false;
    float coherence_score_ = 0.0f;
    float stability_score_ = 0.0f;
    float entropy_score_ = 0.0f;
    
    // UI state
    bool show_advanced_trading_ = false;
    bool auto_refresh_data_ = true;
    float refresh_interval_ = 5.0f;
    
    std::chrono::steady_clock::time_point last_update_;
};

} // namespace sep::workbench
