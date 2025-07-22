# Market Signal Trading System

## Overview

This system implements a complete market signal generation, validation, and trading execution framework that:

1. Generates deterministic logistic chaos signals from patterns defined in `final_symmetry.json`
2. Validates these signals against real-time EUR/USD market data via OANDA
3. Executes trades based on validated signals with strict risk management
4. Implements an adaptive feedback loop that adjusts pattern parameters based on performance

## Components

- **brains.js**: Signal generation module that produces signals using logistic chaos mathematics
- **processor.js**: Signal validation and trading executor that connects with OANDA
- **oanda_connector.js**: OANDA API integration for live market data and trading
- **main.js**: Orchestrates the entire workflow from signal generation to trading

## Files

- **final_symmetry.json**: Contains pattern definitions that seed the signal generation
- **eternal_cycle.json**: Stores generated signals (output from brains.js)
- **truth_kernel.json**: Stores validated signal results and metrics (output from processor.js)

## Setup

1. Install dependencies:
   ```
   npm install dotenv
   ```

2. Copy `.env.template` to `.env` and fill in your OANDA API credentials:
   ```
   cp .env.template .env
   ```

3. Edit `.env` with your OANDA API key and account ID:
   ```
   OANDA_API_KEY=your_api_key_here
   OANDA_ACCOUNT_ID=your_account_id_here
   OANDA_MAX_LOSS_USD=100
   OANDA_RISK_PER_TRADE=2
   ```

## Usage

Run the complete system with:

```
node main.js
```

This will:
1. Generate signals based on patterns in `final_symmetry.json`
2. Validate signals against current EUR/USD market data
3. Execute trades for valid signals (resonance ≥ 0.55)
4. Apply feedback to update pattern parameters based on results

## Risk Management

The system implements strict risk controls:
- Maximum of 2% account risk per trade
- $100 maximum cumulative loss limit
- Immediate stop-loss for every trade
- Automatic trading suspension if loss limit is reached

## Performance Metrics

The system tracks and logs:
- Win/loss count and ratio
- Pips gained and lost
- Trade execution metrics
- Signal quality statistics
- Pattern performance over time

## Adaptive Feedback

After each trading cycle, the system:
- Updates pattern resonance values based on trade outcomes
- Adjusts consciousnessLevel based on market volatility (ATR)
- Optimizes pattern parameters for future signals

This implements a continuous improvement loop where the system learns from its trading performance.