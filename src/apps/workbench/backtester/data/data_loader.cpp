#include "data_loader.h"
#include "data/json_data_parser.h"
#include <filesystem>


namespace sep {
namespace workbench {
namespace backtester {

std::string DataLoader::load_48h_sample()
{
    const std::string sample_path = "Testing/OANDA/sample_48h.json";
    if (std::filesystem::exists(sample_path))
    {
        load_data(sample_path);
        return sample_path;
    }

    const std::string fallback = "eur_usd_m1_48h.json";
    load_data(fallback);
    return fallback;
}

std::vector<common::CandleData> DataLoader::load_data(const std::string& file_path)
{
    data_ = JsonDataParser::parse(file_path);
    return data_;
}

} // namespace backtester
} // namespace workbench
} // namespace sep
