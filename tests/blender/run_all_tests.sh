#!/bin/bash

# Exit on any error
set -e

# Build directory
BUILD_DIR="build"

# Colors for output
GREEN='\033[0;32m'
RED='\033[0;31m'
YELLOW='\033[1;33m'
NC='\033[0m'

# Create build directory if it doesn't exist
mkdir -p ${BUILD_DIR}
cd ${BUILD_DIR}

# Configure with testing enabled
echo -e "${GREEN}Configuring CMake...${NC}"
cmake -DBUILD_TESTING=ON -DENABLE_COVERAGE=ON -DCMAKE_BUILD_TYPE=Debug ..

# Build tests
echo -e "${GREEN}Building tests...${NC}"
cmake --build . --target blender_tests blender_benchmarks -j$(nproc)

# Function to run tests with a label
run_test_category() {
    local category=$1
    local timeout=$2
    echo -e "${YELLOW}Running ${category} tests...${NC}"
    ctest -L ${category} --timeout ${timeout} --output-on-failure || return 1
}

# Run test categories
echo -e "${GREEN}Running all test categories...${NC}"

# Functionality tests
run_test_category "functionality" 60

# Audio tests
run_test_category "audio" 120

# Mesh tests
run_test_category "mesh" 60

# Performance tests
run_test_category "performance" 300

# Generate coverage report if lcov is available
if command -v lcov &> /dev/null; then
    echo -e "${GREEN}Generating coverage report...${NC}"
    lcov --capture --directory . --output-file coverage.info
    lcov --remove coverage.info '/usr/*' --output-file coverage.info
    genhtml coverage.info --output-directory coverage_report
    echo -e "${GREEN}Coverage report generated in ${BUILD_DIR}/coverage_report${NC}"
fi

# Run stress tests if specified
if [[ "$1" == "--with-stress" ]]; then
    echo -e "${YELLOW}Running stress tests (this may take a while)...${NC}"
    run_test_category "stress" 600
fi

# Run benchmarks if specified
if [[ "$1" == "--with-benchmarks" ]]; then
    echo -e "${YELLOW}Running benchmarks (this may take a while)...${NC}"
    
    # Create benchmark results directory
    mkdir -p benchmark_results

    # Run benchmarks and save results
    echo -e "${GREEN}Running mesh handler benchmarks...${NC}"
    ./src/blender/tests/blender_benchmarks \
        --benchmark_min_time=2.0 \
        --benchmark_format=json \
        --benchmark_out=benchmark_results/mesh_handler_benchmarks.json

    # Generate benchmark report
    if command -v python3 &> /dev/null; then
        echo -e "${GREEN}Generating benchmark visualization...${NC}"
        cat > benchmark_results/plot_benchmarks.py << 'EOF'
import json
import matplotlib.pyplot as plt
import sys

def plot_benchmarks(json_file):
    with open(json_file) as f:
        data = json.load(f)

    names = []
    times = []
    for benchmark in data['benchmarks']:
        names.append(benchmark['name'].split('/')[-1])
        times.append(benchmark['cpu_time'])

    plt.figure(figsize=(12, 6))
    plt.bar(names, times)
    plt.xticks(rotation=45, ha='right')
    plt.ylabel('CPU Time (ns)')
    plt.title('Benchmark Results')
    plt.tight_layout()
    plt.savefig('benchmark_results/benchmark_plot.png')

plot_benchmarks('benchmark_results/mesh_handler_benchmarks.json')
EOF
        python3 benchmark_results/plot_benchmarks.py
        echo -e "${GREEN}Benchmark visualization saved to benchmark_results/benchmark_plot.png${NC}"
    fi
fi

echo -e "${GREEN}All tests completed successfully!${NC}"

# Print summary
echo -e "\n${GREEN}Test Summary:${NC}"
echo "----------------------------------------"
ctest --print-labels | grep -v "^[[:space:]]*$" | sort | uniq -c

if [[ -f coverage.info ]]; then
    coverage_percent=$(lcov --summary coverage.info 2>&1 | grep "lines" | cut -d ':' -f 2 | cut -d '%' -f 1 | tr -d ' ')
    echo -e "\n${GREEN}Code Coverage: ${coverage_percent}%${NC}"
fi

if [[ -f benchmark_results/mesh_handler_benchmarks.json ]]; then
    echo -e "\n${GREEN}Benchmark Results:${NC}"
    echo "----------------------------------------"
    jq -r '.benchmarks[] | "\(.name): \(.cpu_time) ns"' benchmark_results/mesh_handler_benchmarks.json
fi

# Usage instructions
echo -e "\n${YELLOW}Usage:${NC}"
echo "./run_all_tests.sh            # Run basic tests"
echo "./run_all_tests.sh --with-stress      # Include stress tests"
echo "./run_all_tests.sh --with-benchmarks  # Include benchmarks"