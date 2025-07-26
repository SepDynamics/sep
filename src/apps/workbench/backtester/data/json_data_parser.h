#pragma once

#include <string>

#include "common/financial_data_types.h"

#include <vector>

class JsonDataParser {
public:
    JsonDataParser() = delete;

    static std::vector<sep::workbench::CandleData> parse(const std::string& filepath);
};
