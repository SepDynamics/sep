#!/bin/bash
cd /sep

# --- Configuration ---
IMAGE_NAME="sep-engine-builder"

echo "Testing SEP standalone executable..."

# Run tests inside Docker container
docker run --gpus all --rm -v $(pwd):/sep $IMAGE_NAME /bin/bash -c '
    cd /project/build
    
    echo "=== Testing SEP Standalone Executable ==="
    
    if [ -f "./sep" ]; then
        echo "Found standalone executable at: $(pwd)/sep"
        echo "File info:"
        ls -la ./sep
        
        echo -e "\n--- Testing help command ---"
        ./sep --help || echo "Help command failed with exit code: $?"
        
        echo -e "\n--- Testing demo command ---"
        ./sep demo || echo "Demo command failed with exit code: $?"
        
        echo -e "\n--- Testing process command with test data ---"
        echo "{\"patterns\": [{\"id\": 1, \"data\": [1,2,3,4,5]}]}" > /tmp/test.json
        ./sep process /tmp/test.json --json || echo "Process command failed with exit code: $?"
        
        echo -e "\n--- Testing benchmark command ---"
        ./sep benchmark --iterations 5 || echo "Benchmark command failed with exit code: $?"
        
        echo -e "\n--- Copying executable to host ---"
        cp ./sep /sep/build/
        echo "Executable copied to /sep/build/sep"
    else
        echo "ERROR: Standalone executable not found!"
        exit 1
    fi
'

echo "Test complete!"