# SEP Engine 48-Hour EUR/USD M1 Sample Dataset

## Overview

This document describes the 48-hour EUR/USD M1 sample dataset created for SEP Engine workbench integration testing. The dataset provides a controlled environment for testing quantum metrics correlation with market movements without requiring live data during development.

## Files Created

### 1. Core Sample Data
- **`Testing/OANDA/sample_48h.json`** (1.3MB)
  - 2,880 M1 candles covering 48 hours
  - Complete OHLC data with bid/ask spreads
  - Volume data and timestamps
  - OANDA API-compatible JSON format

### 2. Generation Scripts
- **`generate_mock_sample.py`** - Creates realistic forex sample data
- **`fetch_sample_data.py`** - Alternative script for live OANDA data (requires API key)

### 3. Validation Scripts
- **`validate_sample_data.py`** - Comprehensive data quality validation
- **`test_sample_integration.py`** - Integration testing with existing systems

### 4. Metrics Export
- **`Testing/OANDA/sample_metrics.json`** - Processed technical analysis metrics

## Dataset Specifications

### Technical Details
- **Instrument**: EUR/USD
- **Granularity**: M1 (1-minute candles)
- **Duration**: 48 hours (2,880 candles)
- **Time Range**: 2025-07-22 20:04:33 UTC to 2025-07-24 20:03:33 UTC
- **Price Range**: 1.08447 - 1.13049 (460.2 pips range)
- **Total Volume**: 327,338
- **Average Spread**: 1.5 pips

### Data Quality
- **Completeness**: 100% (no gaps in time series)
- **Format Validation**: ✅ OANDA API compatible
- **Price Integrity**: ✅ Valid OHLC relationships
- **Volume Data**: ✅ Realistic volume distribution
- **Market Simulation**: Includes weekend/weekday patterns, session volatility

## Usage Examples

### Validation
```bash
# Validate data quality and format
python3 validate_sample_data.py

# Test integration with existing systems
python3 test_sample_integration.py
```

### Data Generation
```bash
# Generate new mock sample data
python3 generate_mock_sample.py

# Fetch live data (requires OANDA API key)
python3 fetch_sample_data.py
```

### Integration with SEP Engine
```bash
# Use with pattern metrics analysis (when available)
./build/examples/pattern_metric_example Testing/OANDA/sample_48h.json --json

# Use with workbench for real-time visualization
./build/src/apps/workbench/sep_workbench
# Then load sample_48h.json in the interface
```

## Data Structure

The sample data follows OANDA's standard JSON format:

```json
{
  "instrument": "EUR_USD",
  "granularity": "M1", 
  "candles": [
    {
      "complete": true,
      "volume": 150,
      "time": "2025-07-22T20:04:33.000000000Z",
      "mid": {
        "o": "1.08556",
        "h": "1.08567", 
        "l": "1.08544",
        "c": "1.08562"
      },
      "bid": {
        "o": "1.08549",
        "h": "1.08560",
        "l": "1.08537", 
        "c": "1.08555"
      },
      "ask": {
        "o": "1.08563",
        "h": "1.08574",
        "l": "1.08551",
        "c": "1.08569"
      }
    }
    // ... 2879 more candles
  ],
  "total_candles": 2880,
  "data_source": "mock_generator"
}
```

## Market Simulation Features

The generated sample data includes realistic market characteristics:

### Session-Based Volatility
- **Asian Session** (20:00-08:00 UTC): Lower volatility, 20-80 volume
- **London Session** (08:00-17:00 UTC): Medium volatility, 50-200 volume
- **NY/London Overlap** (13:00-17:00 UTC): High volatility, 100-300 volume
- **Weekend**: Minimal movement, 1-5 volume

### Price Dynamics
- **Trend Component**: Slight upward bias (+439.3 pips over 48h)
- **Volatility**: Realistic minute-by-minute price movements
- **Spread Modeling**: Dynamic bid/ask spreads (1.5 pips average)
- **Volume Patterns**: Time-based volume distribution

## Quality Validation Report

The validation script provides comprehensive quality assessment:

```
Overall Quality Score: 5/5 (100%)
✅ Data completeness: Excellent (>95%)
✅ Price range: Valid OHLC relationships
✅ Volume data: Good (<10% zero volume)
✅ Time continuity: Excellent (no gaps)
✅ OANDA format: Valid API format
```

## Integration Testing Results

The integration test confirms compatibility with SEP Engine components:

```
🎯 Integration Status:
✅ Data format compatibility: PASSED
✅ Candle processing: PASSED  
✅ Metrics calculation: PASSED
✅ JSON export: PASSED
✅ Time series continuity: PASSED
```

## Technical Indicators Available

The processed dataset includes standard technical analysis metrics:

- **Moving Averages**: SMA-20, SMA-50 (2,861 and 2,831 values respectively)
- **Volatility Metrics**: Daily and annualized volatility
- **Price Statistics**: Range, min/max, pip movements
- **Volume Analysis**: Total, average, distribution patterns
- **Spread Analysis**: Average, min/max spread in pips

## Next Steps

This sample dataset is ready for:

1. **Quantum Metrics Correlation Testing**: Use with pattern_metric_example for alpha prediction experiments
2. **Workbench Integration**: Load in SEP Workbench for visual analysis and testing
3. **Algorithm Development**: Use as controlled dataset for developing and testing trading algorithms
4. **Performance Benchmarking**: Consistent dataset for comparing algorithm performance
5. **Pattern Recognition Training**: Use for training and validating pattern recognition models

## Notes

- This is realistic mock data generated algorithmically, not actual market data
- Designed to simulate realistic EUR/USD market conditions and patterns
- Safe for development and testing without exposing real trading strategies
- Can be regenerated with different parameters as needed
- Compatible with all existing SEP Engine OANDA connector interfaces

## File Locations

All sample data files are located in:
```
/sep/Testing/OANDA/
├── sample_48h.json       # Main sample dataset (1.3MB)
├── sample_metrics.json   # Processed metrics (776B)
└── (generated files)     # Any additional analysis outputs
```

The dataset is now ready for comprehensive workbench integration testing and quantum metrics correlation analysis.
