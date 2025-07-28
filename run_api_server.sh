#!/bin/bash

# Launch the standalone SEP API server
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"
cd "$SCRIPT_DIR"

if [ ! -d "build" ]; then
    echo "Build directory not found. Running build.sh first..."
    ./build.sh || exit 1
fi

# Wait for the executable to be created
echo "Waiting for sep_api_server executable..."
while [ ! -f "build/src/sep_api_server" ]; do
    sleep 1
done

echo "Starting SEP API Server..."
./build/src/sep_api_server "$@"
