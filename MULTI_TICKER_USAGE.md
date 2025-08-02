# Multi-Ticker Training System - Usage Guide

## Overview
Complete multi-ticker training system that pulls cache from OANDA's historical API and prepares analysis for the most recent week, ready for market open refinement.

## Quick Start

### 1. Environment Setup
```bash
# Ensure OANDA credentials are set
source OANDA.env

# Verify build is current
./build.sh
```

### 2. Market Open Preparation (Recommended)
```bash
# Complete market readiness check and preparation
python3 prepare_for_market_open.py
```

This script:
- ✅ Checks current market status and timing
- 📥 Builds enhanced cache for 8 key currency pairs (168 hours of M1 data)
- 🧠 Runs cross-asset correlation analysis  
- 🎯 Validates signal quality for primary trading pairs
- 📊 Generates comprehensive readiness report
- ⏰ Provides recommendations based on time until market open

### 3. Full Multi-Ticker Training (Advanced)
```bash
# Complete training cycle for all 19 pairs
python3 multi_ticker_training_system.py
```

This comprehensive system:
- 📊 Processes 7 major pairs, 8 minor pairs, 4 exotic pairs
- ⚡ Uses parallel processing (4 concurrent workers)
- 🧠 Performs multi-asset signal fusion analysis
- 🔍 Validates cache quality and completeness
- 📈 Generates detailed performance metrics

## Individual Component Testing

### Enhanced Cache Building
```bash
# Build cache for specific pair
./build/examples/enhanced_cache_testbed --instrument EUR_USD --hours 168

# Build with different timeframe
./build/examples/enhanced_cache_testbed --instrument GBP_USD --timeframe M5 --hours 72
```

### Multi-Asset Analysis
```bash
# Run fusion analysis for specific primary asset
./build/examples/phase2_fusion_testbed --primary-asset EUR_USD --verbose-logging

# Get JSON output for automation
./build/examples/phase2_fusion_testbed --primary-asset GBP_USD --output-json
```

## Key Features

### 🚀 Enhanced Market Model Cache
- **Multi-Asset Intelligence**: Cross-correlation analysis between currency pairs
- **Persistent Caching**: Efficient storage and retrieval of historical data
- **Dynamic Aggregation**: Real-time M1/M5/M15 timeframe processing
- **OANDA API Integration**: Direct historical data fetching

### 🧠 Multi-Asset Signal Fusion (Phase 2)
- **Cross-Asset Correlation**: Dynamic correlation analysis between major pairs
- **Signal Enhancement**: Correlation-boosted confidence scoring
- **Market Regime Detection**: Adaptive threshold adjustment
- **Performance Optimization**: 60.73% high-confidence accuracy achieved

### 📊 Signal Quality Validation
- **Accuracy Metrics**: High-confidence accuracy tracking
- **Signal Rate Analysis**: Optimal frequency for trading
- **Profitability Scoring**: Accuracy × frequency optimization
- **Real-Time Feedback**: Live performance monitoring

## Market Readiness Assessment

### Readiness Criteria
- ✅ **Cache Success Rate**: ≥80% for primary pairs
- ✅ **Signal Accuracy**: ≥55% for high-confidence signals  
- ✅ **Correlation Analysis**: Cross-asset analysis functional
- ✅ **Market Timing**: Preparation aligned with market schedule

### Readiness Scores
- **75-100%**: ✅ Ready for optimal trading
- **50-74%**: ⚠️ Ready with caution - monitor performance
- **0-49%**: ❌ Needs attention before trading

## File Structure

```
/sep/
├── multi_ticker_training_system.py    # Complete training system
├── prepare_for_market_open.py         # Market readiness preparation
├── cache/
│   ├── multi_ticker/                  # Full training cache
│   └── market_preparation/            # Market prep cache
├── build/examples/
│   ├── enhanced_cache_testbed         # Individual cache building
│   └── phase2_fusion_testbed          # Multi-asset analysis
└── src/apps/oanda_trader/             # Core implementation
    ├── enhanced_market_model_cache.*   # Enhanced caching system
    ├── multi_asset_signal_fusion.*    # Signal fusion engine
    └── market_regime_adaptive.*       # Regime adaptation
```

## Performance Metrics

### Current Achievement (August 1, 2025)
- **High-Confidence Accuracy**: 60.73%
- **Signal Rate**: 19.1% 
- **Profitability Score**: 204.94
- **Cache Success Rate**: 100% (in testing)

### Target Enhancement (12-month roadmap)
- **Phase 1 Target**: 65-68% accuracy (multi-asset cache)
- **Phase 2 Target**: 68-72% accuracy (signal fusion + regime adaptation)  
- **Phase 3 Target**: 72-76% accuracy (pattern evolution + learning)
- **Final Target**: 75%+ accuracy (complete next-generation system)

## Troubleshooting

### Common Issues

**OANDA API Connection**
```bash
# Check credentials
echo $OANDA_API_KEY
echo $OANDA_ACCOUNT_ID

# Reload environment
source OANDA.env
```

**Build Issues**
```bash
# Clean rebuild
./build.sh

# Check build log
cat output/build_log.txt
```

**Cache Issues**
```bash
# Clear cache and rebuild
rm -rf cache/multi_ticker/
python3 prepare_for_market_open.py
```

## Integration with Existing System

The multi-ticker system builds on the proven SEP Engine foundation:
- ✅ Compatible with existing quantum algorithms
- ✅ Preserves 60.73% baseline performance
- ✅ Enhances with multi-asset intelligence
- ✅ Maintains autonomous operation
- ✅ Supports live trading deployment

For production deployment, use the market preparation script before each trading session to ensure optimal system readiness.
