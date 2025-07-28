#!/bin/bash

# Run the Quantum Tracker - Live Alpha Strategy Monitor

echo "🔮 Starting SEP Quantum Tracker"
echo "======================================"
echo "Monitor live quantum alpha strategy performance"
echo "Press Ctrl+C to exit"
echo ""

# Set environment variables if needed
export DISPLAY=${DISPLAY:-:0}

# Run the quantum tracker
./build/src/apps/oanda_trader/quantum_tracker
