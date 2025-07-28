#!/bin/bash
# Simple helper to start the SEP API server in foreground for testing

SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"
cd "$SCRIPT_DIR/../.." || exit 1

if [ ! -d "build" ]; then
    echo "Build directory not found. Run ./build.sh first."
    exit 1
fi

ENGINE_BIN="build/sep_engine"
if [ ! -f "$ENGINE_BIN" ]; then
    echo "Engine binary not found at $ENGINE_BIN"
    exit 1
fi

# Launch the engine with default config
"$ENGINE_BIN" --foreground &
ENGINE_PID=$!

echo "SEP engine started with PID $ENGINE_PID"

echo "Press Ctrl+C to stop"
trap 'kill $ENGINE_PID' INT TERM
wait $ENGINE_PID
