#pragma once

#include "common/financial_data_types.h"

#include <string>
#include <vector>

class DataLoader {
public:
    DataLoader();
    ~DataLoader();

    void loadData(const std::string& filepath);
    void load_48h_sample();

    const std::vector<sep::workbench::CandleData>& getCandleData() const;

private:
    std::vector<sep::workbench::CandleData> m_candleData;
};
