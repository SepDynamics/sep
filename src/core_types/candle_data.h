#pragma once

#include <cstdint>

// The One True Normal Reference Frame for Financial Data Types
// This is the canonical source of truth for all financial data structures

namespace sep::core {

// =============================================================================
// CANONICAL C++ TYPES (The Normal Reference Frame)
// =============================================================================

/// Standard OHLC candlestick data for financial markets
struct CandleData {
    uint64_t timestamp{0};    // UNIX timestamp in milliseconds
    double open{0.0};         // Opening price
    double high{0.0};         // High price
    double low{0.0};          // Low price  
    double close{0.0};        // Closing price
    uint64_t volume{0};       // Trading volume
    
    /// Time period in seconds (e.g., 60 for 1-minute candles)
    uint32_t period{0};
    
    /// Currency pair or symbol (e.g., "EUR_USD")
    char symbol[16]{0};
};

// =============================================================================
// CUDA POD PROJECTIONS (For GPU Kernels) 
// =============================================================================

/// POD version of CandleData for CUDA kernels - identical structure
using CandleDataPOD = CandleData;

// =============================================================================
// MEASUREMENT FUNCTIONS (Conversions Between Reference Frames)
// =============================================================================

/// Convert canonical CandleData to POD (trivial copy for this type)
inline void convertToPOD(const CandleData& src, CandleDataPOD& dst) {
    dst = src;
}

/// Convert POD CandleData back to canonical (trivial copy for this type)
inline void convertFromPOD(const CandleDataPOD& src, CandleData& dst) {
    dst = src;
}

} // namespace sep::core
