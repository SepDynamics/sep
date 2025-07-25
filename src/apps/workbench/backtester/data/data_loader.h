#pragma once

#include "apps/workbench/core/common_structs.h"

#include <string>
#include <vector>

class DataLoader {
public:
    DataLoader();
    ~DataLoader();

    void loadData(const std::string& filepath);

    const std::vector<sep::workbench::CandleData>& getCandleData() const;

private:
    std::vector<sep::workbench::CandleData> m_candleData;
};
