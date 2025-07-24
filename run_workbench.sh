#!/bin/bash

# Get the directory of the script
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"
cd "$SCRIPT_DIR"

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
    echo "keys.txt not found. Creating with placeholder values."
    echo "export OANDA_API_KEY=\"<your_api_key>\"" > keys.txt
    echo "export OANDA_ACCOUNT_ID=\"<your_account_id>\"" >> keys.txt
    echo "Please edit keys.txt with your actual OANDA credentials."
    source keys.txt
fi

# Verify credentials are loaded
if [ -z "$OANDA_API_KEY" ] || [ -z "$OANDA_ACCOUNT_ID" ]; then
    echo "Error: OANDA credentials not properly loaded!"
    exit 1
fi

echo "OANDA Account: $OANDA_ACCOUNT_ID"
echo "Using PRACTICE server: api-fxpractice.oanda.com"
echo "This is REAL money trading - be careful!"
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

# Wait for the executable to be created
echo "Waiting for sep_workbench executable..."
while [ ! -f "build/src/apps/workbench/sep_workbench" ]; do
    sleep 1
done

# Run the sep_workbench with OANDA credentials
echo "Starting SEP Workbench..."
echo "Press Ctrl+C to exit"
echo ""

./build/src/apps/workbench/sep_workbench --api-key "$OANDA_API_KEY" --account-id "$OANDA_ACCOUNT_ID"