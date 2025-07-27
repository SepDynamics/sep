#!/bin/bash
# Test script for building and running pme_testbed

set -uo pipefail

echo "Building and running pme_testbed..."

USER_ID=$(id -u)
GROUP_ID=$(id -g)

docker run --gpus all --rm \
    -v $(pwd):/sep \
    -e CUDA_HOME=/usr/local/cuda \
    -e CUDA_TOOLKIT_ROOT_DIR=/usr/local/cuda \
    -e CUDA_BIN_PATH=/usr/local/cuda/bin \
    -e CMAKE_CUDA_COMPILER=/usr/local/cuda/bin/nvcc \
    -e PATH=/usr/local/cuda/bin:${PATH} \
    -e LD_LIBRARY_PATH=/usr/local/cuda/lib64:${LD_LIBRARY_PATH-} \
    sep-engine-builder bash -c '
    cd /sep/build
    
    # Build the pme_testbed target
    ninja pme_testbed
    
    # Run the test with sample data
    ./examples/pme_testbed /sep/assets/test_data/eur_usd_m1_48h.json
'

# Fix ownership of generated files
sudo chown -R $USER_ID:$GROUP_ID /sep/build

echo "Build complete!"

# Run the alpha analysis script
echo "Running alpha analysis..."
python3 /sep/scripts/alpha_analysis.py
