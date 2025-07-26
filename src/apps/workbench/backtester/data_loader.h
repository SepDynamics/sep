#pragma once

#include <vector>

#include "common/financial_data_types.h"

namespace sep {
namespace workbench {
namespace backtester {

class DataLoader {
public:
    void load_data(const std::string& file_path);
    const std::vector<CandleData>& get_data() const { return data_; }

private:
    std::vector<CandleData> data_;
};

}
}
}
