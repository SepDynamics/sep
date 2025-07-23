# SEP Engine - Predictive Financial Modeling System

## Overview
SEP Engine is a CUDA-accelerated predictive financial modeling system using quantum-inspired pattern analysis on forex data. The system combines advanced time-series analysis with GPU optimization to develop alpha prediction capabilities for trading applications.

## Current State ✅
- **Build System**: Stabilized Docker-based builds with CUDA Toolkit v12.9
- **Core Engine**: Functional pattern analysis with JSON output support
- **Data Pipeline**: Integrated Python scripts for complete experiment workflow
- **Performance**: Sub-10ms processing on CUDA-enabled systems

## Key Components
- **Pattern Analysis**: CUDA-accelerated pattern metric calculations (coherence, stability, entropy)
- **Financial Data**: OANDA historical forex data integration
- **Prediction Engine**: Alpha generation through time-series pattern recognition
- **Backtesting**: Strategy validation with performance metrics

## Quick Start
```bash
# Build the complete system
./build.sh

# Run pattern analysis
./build/examples/pattern_metric_example Testing/OANDA/ --json

# Execute complete alpha experiment
python3 run_alpha_experiment.py

# Run financial backtesting
python3 financial_backtest.py
```

## Architecture
```
[Forex Data] → [CUDA Pattern Analysis] → [Alpha Prediction] → [Backtesting] → [Performance Metrics]
                       ↓
               [JSON Output Pipeline] → [Python Analysis] → [Visualization]
```

## Performance & Profiling
- **CUDA Profiling**: `nsys profile --output=profile ./build/examples/pattern_metric_example Testing/OANDA/`
- **Target Latency**: <10ms for real-time analysis
- **Throughput**: 1M+ data points per second on RTX-class GPUs

This is a launching demo ready for extended development. See TODO.md for current priorities.