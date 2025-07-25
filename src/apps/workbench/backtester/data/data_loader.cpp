#include "apps/workbench/backtester/data/data_loader.h"
#include "apps/workbench/backtester/data/json_data_parser.h"

DataLoader::DataLoader() {}

DataLoader::~DataLoader() {}

void DataLoader::loadData(const std::string& filepath) {
    m_candleData = JsonDataParser::parse(filepath);
}

const std::vector<sep::workbench::CandleData>& DataLoader::getCandleData() const {
    return m_candleData;
}
