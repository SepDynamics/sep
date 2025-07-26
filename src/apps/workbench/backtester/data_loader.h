#pragma once

#include <vector>
#include "src/apps/workbench/core/common_structs.h"

namespace sep {
namespace workbench {
namespace backtester {

class DataLoader {
public:
    void load_data(const std::string& file_path) {}
    const std::vector<CandleData>& get_data() const { return data_; }

private:
    std::vector<CandleData> data_;
};

}
}
}
