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
    m_candleData = JsonDataParser::parse(filepath);
}

const std::vector<sep::workbench::CandleData>& DataLoader::getCandleData() const {
    return m_candleData;
}

void DataLoader::load_48h_sample() {
    namespace fs = std::filesystem;
    const std::string output = "Testing/OANDA/eurusd_48h.json";

    sep::connectors::OandaConnector connector("", "", true);
    if (!connector.initialize()) {
        return;
    }

    auto candles = connector.getHistoricalData("EUR_USD", "M1", "", "", 48 * 60);
    connector.shutdown();

    std::vector<sep::CandleData> export_candles;
    export_candles.reserve(candles.size());

    m_candleData.clear();
    m_candleData.reserve(candles.size());

    for (const auto& c : candles) {
        std::tm tm{};
        std::stringstream ss(c.time);
        ss >> std::get_time(&tm, "%Y-%m-%dT%H:%M:%S");
        auto tp = std::chrono::system_clock::from_time_t(std::mktime(&tm));
        m_candleData.emplace_back(c.open, c.high, c.low, c.close,
                                  static_cast<int>(c.volume), tp);

        sep::CandleData cd;
        cd.time = c.time;
        cd.volume = static_cast<uint64_t>(c.volume);
        cd.open = static_cast<float>(c.open);
        cd.high = static_cast<float>(c.high);
        cd.low = static_cast<float>(c.low);
        cd.close = static_cast<float>(c.close);
        export_candles.push_back(cd);
    }

    fs::create_directories("Testing/OANDA");
    sep::DataParser parser;
    parser.saveValidatedCandlesJSON(export_candles, output);
}
