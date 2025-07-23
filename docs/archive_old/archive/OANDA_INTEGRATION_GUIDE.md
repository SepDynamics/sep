# OANDA Integration Guide for SEP Workbench

## Overview

The SEP Workbench now includes direct integration with OANDA's forex trading platform, allowing real-time market data to feed directly into the quantum pattern analysis engine. This guide explains how to set up and use the OANDA integration.

## Current Implementation Status

### ✅ Completed Features
- **Environment-based Authentication**: API credentials are loaded from environment variables
- **Demo/Practice Account Support**: Configured to use OANDA's practice server (api-fxpractice.oanda.com)
- **Real-time Data Streaming**: The application now streams real-time price data from OANDA.
- **Basic UI Display**: The application displays the real-time bid/ask prices and timestamps in the UI.

### 🚧 In Progress Features
- **Stable Connection**: The application currently crashes when connecting to OANDA. This is the highest priority to fix.
- **Static UI Layout**: The UI windows are currently floating and overlapping. This needs to be fixed to provide a stable and usable interface.
- **Metrics Dashboard Integration**: OANDA data is not yet fully integrated with the metrics dashboard.
- **SEP Engine Integration**: The real-time data is not yet being fed into the SEP engine for analysis.

### 🔧 Architecture
```
SEP Workbench
├── OANDA Trader App
│   ├── OANDA Connector (uses env vars)
│   ├── Real-time Data Display
│   └── SEP Engine (integration pending)
├── Memory Monitoring System
```

## Setup Instructions

### 1. Prerequisites
- OANDA demo account (free at https://www.oanda.com/forex-trading/demo-account/)
- API key and account ID from OANDA's developer portal

### 2. Configure Credentials

Create a `keys.txt` file in the project root:
```bash
export OANDA_API_KEY="your-api-key-here"
export OANDA_ACCOUNT_ID="your-account-id-here"
```

For the demo account mentioned in this project:
```bash
export OANDA_API_KEY="f811f3dcf044280e63f2ba075ef0eeff-aee7f27441d32b13da4cb94f6bb10398"
export OANDA_ACCOUNT_ID="001-001-13487160-001"
```

### 3. Build the Project
```bash
./build.sh
```

### 4. Run the Workbench
```bash
./run_workbench.sh
```

This script will:
- Load OANDA credentials from `keys.txt`
- Verify the credentials are set
- Launch the SEP workbench with OANDA integration enabled

## How It Works

### Connection Flow
1. **Startup**: Workbench initializes and loads environment variables
2. **Authentication**: OANDA connector uses API key to authenticate with practice server
3. **Data Streaming**: Real-time forex data streams into the application.
4. **UI Display**: The real-time data is displayed in the UI.
5. **Pattern Analysis (Future)**: SEP's quantum algorithms will analyze market patterns in real-time
6. **Visualization (Future)**: Results will be displayed in the metrics dashboard

### Key Components

#### OANDA Connector (`src/connectors/oanda_connector.cpp`)
- Handles API authentication and requests
- Manages rate limiting and connection stability
- Supports both REST API and streaming endpoints

#### OANDA Trader App (`src/apps/oanda_trader/oanda_trader_app.cpp`)
- Initializes OANDA connection on startup
- Displays connection status
- Displays real-time market data

#### Run Script (`run_workbench.sh`)
- Automates environment setup
- Ensures credentials are loaded before launch
- Provides user-friendly error messages

## Troubleshooting

### Connection Issues
- **Crash on Connect**: This is a known issue and is the highest priority to fix.
- Verify API credentials are correct
- Ensure using practice/demo account (not live)
- Check internet connectivity
- Look for error messages in console output

### Common Errors
- "OANDA credentials not found": Create/check `keys.txt`
- "Failed to connect": Verify API key is valid
- "Account not found": Check account ID format

## Future Enhancements

### Planned Features
- [ ] **Fix Connection Crash**: Stabilize the OANDA connection.
- [ ] **Static UI Layout**: Implement a non-floating, organized UI.
- [ ] **SEP Engine Integration**: Feed real-time data into the SEP engine.
- [ ] **Live account support**: (currently demo only)
- [ ] **Multiple currency pair selection**
- [ ] **Historical data backtesting**
- [ ] **Trading signal generation**
- [ ] **Risk management integration**

### Integration Points
- Pattern metrics can trigger trading signals
- Quantum coherence maps to market volatility
- Memory tiers track pattern evolution over time

## Technical Details

### API Endpoints Used
- Account info: `/v3/accounts/{accountID}`
- Instruments: `/v3/accounts/{accountID}/instruments`
- Pricing stream: `/v3/accounts/{accountID}/pricing/stream`
- Candles: `/v3/instruments/{instrument}/candles`

### Data Flow
```
OANDA API → OandaConnector → OANDA Trader App → UI Display
                                     ↓ (Future)
                                  SEP Engine → Pattern Analysis
                                     ↓
                              Quantum Algorithms
                                     ↓
                              Trading Insights
```

## Security Notes
- Never commit API keys to version control
- Use environment variables for all credentials
- Practice account recommended for development
- Implement proper error handling for production use

## References
- [OANDA v20 API Documentation](https://developer.oanda.com/rest-live-v20/introduction/)
- [SEP Engine Documentation](./SEP_FINANCIAL_QUICK_REFERENCE.md)
- [Financial Pipeline Overview](./FINANCIAL_PIPELINE_GAMEPLAN.md)