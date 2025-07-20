#!/bin/bash
cd /sep

echo "Testing SEP standalone executable inside Docker..."

# Test the executable that was already built
docker run --gpus all --rm -v $(pwd):/sep sep-engine-builder /bin/bash -c '
    cd /sep/build
    
    echo "=== Testing SEP Standalone Executable ==="
    echo "Current directory: $(pwd)"
    echo "Files in directory:"
    ls -la
    
    if [ -f "./sep" ]; then
        echo -e "\n--- Testing help command ---"
        ./sep --help
        
        echo -e "\n--- Testing demo command ---"
        ./sep demo --gpu
        
        echo -e "\n--- Testing process command with test data ---"
        echo "{\"patterns\": [{\"id\": 1, \"data\": [1,2,3,4,5]}]}" > /tmp/test.json
        ./sep process /tmp/test.json --json
        
        echo -e "\n--- Testing benchmark command ---"
        ./sep benchmark --iterations 5
        
        echo -e "\n=== All tests completed ==="
    else
        echo "ERROR: sep executable not found!"
        exit 1
    fi
'

echo "Test complete!"