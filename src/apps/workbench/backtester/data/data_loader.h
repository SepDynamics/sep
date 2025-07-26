#pragma once

#include <vector>

#include "common/financial_data_types.h"

namespace sep
{
    namespace workbench
    {
        namespace backtester
        {

            class DataLoader
            {
            public:
                /**
                 * @brief Load the default 48h EUR/USD sample if available.
                 *
                 * This checks for Testing/OANDA/sample_48h.json and falls back
                 * to eur_usd_m1_48h.json when the file is missing.
                 *
                 * @return The path of the dataset that was loaded.
                 */
                std::string load_48h_sample();

                std::vector<common::CandleData> load_data(const std::string& file_path);
                const std::vector<common::CandleData>& get_data() const { return data_; }

            private:
                std::vector<common::CandleData> data_;
            };

        }  // namespace backtester
    }  // namespace workbench
}  // namespace sep
