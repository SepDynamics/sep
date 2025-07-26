#include "backend_tab_controller.h"

#include <filesystem>
#include <iostream>
#include <cstring>

#include "imgui.h"

namespace sep::workbench {

BackendTabController::BackendTabController()
    : backtester_(std::make_unique<backtester::Backtester>()),
      data_loader_(std::make_unique<backtester::DataLoader>()),
      pattern_engine_(std::make_unique<sep::quantum::PatternMetricEngine>()) {}

BackendTabController::~BackendTabController() { shutdown(); }
bool BackendTabController::initialize() {
    std::cout << "[BackendTabController] Initializing..." << std::endl;
    globalEventBus().subscribe<OrderUpdateEvent>([this](const OrderUpdateEvent& e) {
        auto it = std::find_if(order_cache_.begin(), order_cache_.end(), [&](const sep::connectors::OrderInfo& o) { return o.id == e.info.id; });
        if (it == order_cache_.end()) {
            order_cache_.push_back(e.info);
        } else {
            *it = e.info;
        }
    });
    return true;
}

void BackendTabController::render() {
    ImGui::Columns(2, "BackendColumns", true);
    renderDataSourceSelector();
    ImGui::Separator();
    renderBacktesterPanel();
    renderOrderManagementPanel();

    ImGui::NextColumn();

    std::string selected;
    if (file_dialog_.render(selected)) {
        strncpy(file_path_buffer_, selected.c_str(), sizeof(file_path_buffer_) - 1);
        file_path_buffer_[sizeof(file_path_buffer_) - 1] = '\0';
    }

    ImGui::Columns(1);
}

void BackendTabController::shutdown() {}

void BackendTabController::setMetricsMonitor(std::shared_ptr<MetricsMonitor> monitor) {
    monitor_ = monitor;
}

void BackendTabController::setServiceConnector(ServiceConnector* connector) {
    service_connector_ = connector;
    if (service_connector_) {
        trade_manager_ = service_connector_->getTradeManager();
    }
}

void BackendTabController::renderDataSourceSelector() {
    const char* source_types[] = {"File", "Live Stream", "Generated Data"};
    ImGui::Combo("Source Type", &data_source_type_, source_types, 3);

    switch (data_source_type_) {
        case 0: // File
            ImGui::InputText("File Path", file_path_buffer_, sizeof(file_path_buffer_));
            ImGui::SameLine();
            if (ImGui::Button("Browse")) {
                file_dialog_.open(std::filesystem::path(file_path_buffer_).empty() ? "." : std::filesystem::path(file_path_buffer_).parent_path().string());
            }
            if (ImGui::Button("Load File", ImVec2(-1, 0))) {
                handleDataLoad();
            }
            break;
        case 1: // Live Stream
            ImGui::Text("Live stream not yet implemented");
            break;
        case 2: // Generated
            if (ImGui::Button("Generate Test Data", ImVec2(-1, 0))) {
                std::vector<uint8_t> test_data(1024);
                for (size_t i = 0; i < test_data.size(); ++i) {
                    test_data[i] = static_cast<uint8_t>(i % 256);
                }
                if(monitor_) monitor_->ingestData(test_data.data(), test_data.size());
            }
            break;
    }
}

void BackendTabController::renderOrderManagementPanel() {
    ImGui::Begin("Paper Trading");
    if (ImGui::Checkbox("Enable Paper Trading", &paper_trading_)) {
        if (trade_manager_) {
            trade_manager_->setPaperTrading(paper_trading_);
        }
    }
    if (trade_manager_) {
        ImGui::Text("Balance: %.2f", trade_manager_->getAccountBalance());
        ImGui::Text("Realized PnL: %.2f", trade_manager_->getRealizedPnL());
        const auto& hist = trade_manager_->getBalanceHistory();
        if (!hist.empty()) {
            ImGui::PlotLines("Balance History", hist.data(), hist.size());
        }
    }
    ImGui::End();

    ImGui::Begin("Order Management");

    ImGui::InputText("Instrument", instrument_buffer_, sizeof(instrument_buffer_));
    ImGui::InputInt("Units", &units_);

    if (ImGui::Button("Place Buy Order")) {
        if (trade_manager_ && service_connector_) {
            auto md = service_connector_->getOandaConnector()->getMarketData(instrument_buffer_);
            trade_manager_->placeOrder(instrument_buffer_, units_, md.ask, stop_loss_pips_, take_profit_pips_);
        }
    }

    ImGui::SameLine();

    if (ImGui::Button("Place Sell Order")) {
        if (trade_manager_ && service_connector_) {
            auto md = service_connector_->getOandaConnector()->getMarketData(instrument_buffer_);
            trade_manager_->placeOrder(instrument_buffer_, -units_, md.bid, stop_loss_pips_, take_profit_pips_);
        }
    }

    if (ImGui::Button("Refresh Orders and Positions")) {
        if (service_connector_ && service_connector_->getOandaConnector()) {
            service_connector_->getOandaConnector()->refreshOrders();
            open_positions_ = service_connector_->getOandaConnector()->getOpenPositions();
            orders_ = service_connector_->getOandaConnector()->getOrders();
        }
    }

    ImGui::Text("Open Positions:");
    if (!open_positions_.is_null()) {
        ImGui::TextUnformatted(open_positions_.dump(4).c_str());
    }

    if (trade_manager_) {
        ImGui::Text("Local Positions: %zu", trade_manager_->getPositions().size());
        ImGui::Text("Local Orders: %zu", trade_manager_->getOrders().size());
    }

    ImGui::Text("Orders:");
    for (const auto& o : order_cache_) {
        const char* status_str = "PENDING";
        if (o.status == sep::connectors::OrderStatus::FILLED) status_str = "FILLED";
        else if (o.status == sep::connectors::OrderStatus::CANCELED) status_str = "CANCELED";
        ImGui::Text("%s %s %.0f @ %.5f [%s]", o.id.c_str(), o.instrument.c_str(), o.units, o.price, status_str);
    }

    ImGui::End();
}

void BackendTabController::renderBacktesterPanel() {
    ImGui::Begin("Risk Management");
    if (ImGui::SliderFloat("Risk Percentage", &risk_percentage_, 0.1f, 5.0f, "%.2f%%")) {
        if (trade_manager_) trade_manager_->setRiskPercentage(risk_percentage_ / 100.0);
    }
    if (trade_manager_) {
        ImGui::Text("Account Balance: %.2f", trade_manager_->getAccountBalance());
    }
    ImGui::InputFloat("Stop Loss (pips)", &stop_loss_pips_);
    ImGui::InputFloat("Take Profit (pips)", &take_profit_pips_);
    ImGui::End();

    ImGui::Begin("Signal Validation");
    if (ImGui::Button("Validate Signals")) {
        data_loader_->load_data(file_path_buffer_);
        const auto& candles = data_loader_->get_data();
        if (!candles.empty()) {
            std::vector<uint8_t> byte_stream;
            for (const auto& candle : candles) {
                const uint8_t* candle_bytes = reinterpret_cast<const uint8_t*>(&candle);
                byte_stream.insert(byte_stream.end(), candle_bytes, candle_bytes + sizeof(CandleData));
            }

            pattern_engine_->ingestData(byte_stream.data(), byte_stream.size());
            pattern_engine_->evolvePatterns();
            pattern_engine_->computeMetrics();
            const auto& signals = pattern_engine_->getSignals();

            std::vector<float> prices;
            for (const auto& candle : candles) {
                prices.push_back(candle.close);
            }

            sep::core::ServiceProxyEngine proxy("localhost", 8080);
            validation_result_ = proxy.validateSignalsAgainstHistory(signals, prices);
        }
    }

    ImGui::Text("Accuracy: %.2f", validation_result_.predictive_accuracy);
    ImGui::Text("False Positive Rate: %.2f", validation_result_.false_positive_rate);
    ImGui::PopID();
    ImGui::End();

    ImGui::Begin("Backtester");
    ImGui::PushID("Backtester");
    ImGui::InputText("Dataset", backtest_file_buffer_, sizeof(backtest_file_buffer_));
    if (ImGui::Button("Run Backtest")) {
        data_loader_->load_data(backtest_file_buffer_);
        backtester_->run(pattern_engine_.get(), data_loader_.get());
    }
    ImGui::SameLine();
    if (ImGui::Button("Run 48h Sample")) {
        strncpy(backtest_file_buffer_, "eur_usd_m1_48h.json", sizeof(backtest_file_buffer_) - 1);
        backtest_file_buffer_[sizeof(backtest_file_buffer_) - 1] = '\0';
        data_loader_->load_data(backtest_file_buffer_);
        backtester_->run(pattern_engine_.get(), data_loader_.get());
    }

    const auto& result = backtester_->getResult();
    ImGui::Text("Total Trades: %d", result.total_trades);
    ImGui::Text("Win Rate: %.2f", result.win_rate);
    ImGui::Text("Total PnL: %.2f", result.total_pnl);
    ImGui::Text("Sharpe Ratio: %.2f", result.sharpe_ratio);
    ImGui::Text("Max Drawdown: %.2f", result.max_drawdown);

    ImGui::End();
}

void BackendTabController::handleDataLoad() {
    if (monitor_ && strlen(file_path_buffer_) > 0) {
        monitor_->ingestFile(file_path_buffer_);
    }
}

void BackendTabController::handleClearData() {
    if(monitor_) monitor_->clear();
}

void BackendTabController::handleExportMetrics() {
    if(monitor_) monitor_->saveMetricsToFile(export_path_buffer_);
}

void BackendTabController::handleStartProcessing() {
    if(monitor_) monitor_->startProcessing();
}

void BackendTabController::handleStopProcessing() {
    if(monitor_) monitor_->stopProcessing();
}

} // namespace sep::workbench
