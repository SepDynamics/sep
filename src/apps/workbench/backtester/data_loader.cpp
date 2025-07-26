#include "data_loader.h"
#include "data/json_data_parser.h"

namespace sep {
namespace workbench {
namespace backtester {

void DataLoader::load_data(const std::string& file_path)
{
    data_ = JsonDataParser::parse(file_path);
}

} // namespace backtester
} // namespace workbench
} // namespace sep
