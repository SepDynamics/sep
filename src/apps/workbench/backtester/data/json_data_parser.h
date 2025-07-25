#pragma once

#include <string>

#include "apps/workbench/core/common_structs.h"

#include <vector>

class JsonDataParser {
public:
    JsonDataParser() = delete;

    static std::vector<sep::workbench::CandleData> parse(const std::string& filepath);
};
