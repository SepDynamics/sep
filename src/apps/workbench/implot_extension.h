#pragma once

#include <implot.h>
#include <vector>

namespace ImPlot {

// Plots candlestick data with x-coordinates and OHLC values
inline void PlotCandles(const char* label_id,
                       const double* xs,
                       const double* opens,
                       const double* closes,
                       const double* lows,
                       const double* highs,
                       int count) {
    // Plot wicks (vertical lines)
    ImPlot::SetNextLineStyle(ImVec4(1, 1, 1, 1), 1.0f);
    for (int i = 0; i < count; ++i) {
        // Create temporary vectors for the vertical lines
        std::vector<double> x_wick{xs[i], xs[i]};
        std::vector<double> y_wick{lows[i], highs[i]};
        ImPlot::PlotLine("##Wick", x_wick.data(), y_wick.data(), 2);
    }

    // Plot candle bodies
    const float half_width = 0.3f; // Adjust width of candles as needed
    for (int i = 0; i < count; ++i) {
        bool is_bullish = closes[i] > opens[i];
        ImVec4 color = is_bullish ? ImVec4(0, 1, 0, 1) : ImVec4(1, 0, 0, 1);
        
        // Create vectors for the rectangle corners
        std::vector<double> x_body{
            xs[i] - half_width, xs[i] + half_width,
            xs[i] + half_width, xs[i] - half_width
        };
        std::vector<double> y_body{
            opens[i], opens[i],
            closes[i], closes[i]
        };
        
        ImPlot::SetNextFillStyle(color);
        ImPlot::PlotShaded(label_id, x_body.data(), y_body.data(), 4);
    }
}

} // namespace ImPlot