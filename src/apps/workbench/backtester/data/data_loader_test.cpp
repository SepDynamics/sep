#include "apps/workbench/backtester/data/data_loader.h"
#include <iostream>

int main() {
    DataLoader dataLoader;
    dataLoader.loadData("Testing/OANDA/eurusd_48h.json");

    const auto& candleData = dataLoader.getCandleData();

    if (candleData.empty()) {
        std::cerr << "No data loaded." << std::endl;
        return 1;
    }

    std::cout << "Loaded " << candleData.size() << " candles." << std::endl;

    // Print the first candle to verify
    const auto& firstCandle = candleData.front();
    std::cout << "First candle: "
              << "O: " << firstCandle.open << ", "
              << "H: " << firstCandle.high << ", "
              << "L: " << firstCandle.low << ", "
              << "C: " << firstCandle.close << ", "
              << "V: " << firstCandle.volume << std::endl;

    return 0;
}
