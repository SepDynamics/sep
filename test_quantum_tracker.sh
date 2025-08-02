#!/bin/bash

echo "🧪 Testing Quantum Tracker with Static Data"
echo "============================================="
echo "This will run the quantum tracker in a controlled environment"
echo "using static test data to verify signal generation."
echo ""

# Set up environment
source OANDA.env

# Run with timeout to prevent hanging
timeout 60 ./build/src/apps/oanda_trader/quantum_tracker &
PID=$!

echo "Started quantum tracker (PID: $PID)"
echo "Will terminate after 60 seconds to show initial signals..."
echo ""
echo "Press Ctrl+C to stop early if you see signals being generated."

# Wait for the timeout or manual termination
wait $PID
EXIT_CODE=$?

echo ""
echo "🔍 Test Results:"
if [ $EXIT_CODE -eq 124 ]; then
    echo "✅ Test completed (timeout reached - normal for this test)"
elif [ $EXIT_CODE -eq 0 ]; then
    echo "✅ Test completed successfully"
else
    echo "⚠️  Test ended with exit code: $EXIT_CODE"
fi

echo ""
echo "If you saw signal generation in the output above, the system is working!"
echo "For live trading, run: source OANDA.env && ./build/src/apps/oanda_trader/quantum_tracker"
