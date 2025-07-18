#!/bin/bash

# Exit on error
set -e

# --- Configuration ---
# Path to the SEP Engine executable
ENGINE_EXECUTABLE="./build/examples/pattern_metric_example"

# Path to the raw market data
MARKET_DATA_FILE="/workspace/train_data_2021_to_2025"

# Path for the output metrics from the engine
METRICS_OUTPUT_FILE="metrics.json"

# Path to the backtesting script
BACKTEST_SCRIPT="financial_backtest.py"

# --- Step 1: Run the SEP Engine ---
echo "--- Running SEP Engine to generate metrics ---"
if [ ! -f "$ENGINE_EXECUTABLE" ]; then
    echo "Error: Engine executable not found at $ENGINE_EXECUTABLE"
    echo "Please build the project first using ./build_and_test.sh"
    exit 1
fi

# Run the engine, processing the market data and outputting metrics
$ENGINE_EXECUTABLE $MARKET_DATA_FILE --output $METRICS_OUTPUT_FILE

echo "--- SEP Engine processing complete. Metrics saved to $METRICS_OUTPUT_FILE ---"
echo ""

# --- Step 2: Run the Financial Backtesting Script ---
echo "--- Running financial backtesting analysis ---"
if [ ! -f "$BACKTEST_SCRIPT" ]; then
    echo "Error: Backtest script not found at $BACKTEST_SCRIPT"
    exit 1
fi

# Run the Python script, passing the metrics file and the original market data
# (The market data is needed to get the 'close' prices for return calculations)
python3 $BACKTEST_SCRIPT $METRICS_OUTPUT_FILE $MARKET_DATA_FILE

echo "--- Financial analysis complete ---"