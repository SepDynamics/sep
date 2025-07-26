#include "apps/workbench/backtester/data/data_loader.h"
#include "apps/workbench/backtester/data/json_data_parser.h"
#include "connectors/oanda_connector.h"
#include "engine/data_parser.h"
#include <chrono>
#include <filesystem>
#include <iomanip>
#include <sstream>

DataLoader::DataLoader() {}

DataLoader::~DataLoader() {}

void DataLoader::loadData(const std::string& filepath) {
    load_data(filepath);
}

void DataLoader::load_data(const std::string& filepath) {
    sep::DataParser parser;
    auto candles = parser.parseQuantJSON(filepath);
    m_candleData.clear();
    m_candleData.reserve(candles.size());

    for (const auto& c : candles) {
        std::tm tm{};
        std::stringstream ss(c.time);
        ss >> std::get_time(&tm, "%Y-%m-%dT%H:%M:%S");
        auto tp = std::chrono::system_clock::from_time_t(std::mktime(&tm));
        m_candleData.emplace_back(c.open, c.high, c.low, c.close,
                                  static_cast<int>(c.volume), tp);
    }
}

const std::vector<sep::workbench::CandleData>& DataLoader::getCandleData() const {
    return m_candleData;
}

void DataLoader::load_48h_sample() {
    namespace fs = std::filesystem;
    const std::string output = "eur_usd_m1_48h.json";

    if (!fs::exists(output)) {
        sep::connectors::OandaConnector connector("", "", true);
        if (connector.initialize()) {
            connector.saveEURUSDM1_48h(output);
            connector.shutdown();
        }
    }

    load_data(output);
}
