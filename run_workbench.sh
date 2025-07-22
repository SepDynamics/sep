#!/bin/bash

# SEP Workbench Runner with OANDA Credentials
# This script sets up the environment and runs the SEP workbench

echo "==================================="
echo "SEP Workbench with OANDA Trading"
echo "==================================="

# Source the OANDA credentials
if [ -f "keys.txt" ]; then
    echo "Loading OANDA credentials..."
    source keys.txt
else
    echo "Error: keys.txt not found!"
    echo "Please create keys.txt with:"
    export OANDA_API_KEY="9a5380d0af7dc6d3cdd0c9b29cc5917a-02ceee9244b286c586239697d1ab8b95a"
    export OANDA_ACCOUNT_ID="101-001-31229774-001"
    exit 1
fi

# Verify credentials are loaded
if [ -z "$OANDA_API_KEY" ] || [ -z "$OANDA_ACCOUNT_ID" ]; then
    echo "Error: OANDA credentials not properly loaded!"
    exit 1
fi

echo "OANDA Account: $OANDA_ACCOUNT_ID"
echo "Using practice/demo server: api-fxpractice.oanda.com"
echo ""

# Check if build directory exists
if [ ! -d "build" ]; then
    echo "Build directory not found. Running build.sh first..."
    if [ -f "build.sh" ]; then
        ./build.sh
    else
        echo "Error: build.sh not found!"
        exit 1
    fi
fi

# Check if oanda_trader executable exists
if [ ! -f "build/src/apps/oanda_trader/oanda_trader" ]; then
    echo "Error: oanda_trader executable not found!"
    echo "Please build the project first with: ./build.sh"
    exit 1
fi

# Run the oanda_trader with OANDA credentials
echo "Starting OANDA Trader..."
echo "Press Ctrl+C to exit"
echo ""

cd build/src/apps/oanda_trader
./oanda_trader