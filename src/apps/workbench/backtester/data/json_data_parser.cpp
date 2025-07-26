#include "apps/workbench/backtester/data/json_data_parser.h"

#include <fstream>
#include <nlohmann/json.hpp>
#include <iostream>

std::vector<sep::common::CandleData> JsonDataParser::parse(const std::string& filepath) {
    std::vector<sep::common::CandleData> candles;
    std::ifstream file(filepath);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filepath << std::endl;
        return candles;
    }

    nlohmann::json json_data;
    try {
        file >> json_data;
    } catch (nlohmann::json::parse_error& e) {
        std::cerr << "JSON parse error: " << e.what() << std::endl;
        return candles;
    }

    for (const auto& item : json_data) {
        std::string time_str = item.at("time");
        std::tm tm = {};
        std::stringstream ss(time_str);
        ss >> std::get_time(&tm, "%Y-%m-%dT%H:%M:%S");
        auto timestamp = std::chrono::system_clock::from_time_t(std::mktime(&tm));

        candles.emplace_back(sep::common::CandleData{
            item.at("open").get<double>(),
            item.at("high").get<double>(),
            item.at("low").get<double>(),
            item.at("close").get<double>(),
            item.at("volume").get<double>(),
            timestamp
        });
    }

    return candles;
}
