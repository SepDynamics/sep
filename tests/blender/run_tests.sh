#!/bin/bash

# Exit on any error
set -e

# Build directory
BUILD_DIR="build"

# Colors for output
GREEN='\033[0;32m'
RED='\033[0;31m'
NC='\033[0m'

# Create build directory if it doesn't exist
mkdir -p ${BUILD_DIR}
cd ${BUILD_DIR}

# Configure with testing enabled
echo -e "${GREEN}Configuring CMake...${NC}"
cmake -DBUILD_TESTING=ON -DENABLE_COVERAGE=ON -DCMAKE_BUILD_TYPE=Debug ..

# Build tests
echo -e "${GREEN}Building tests...${NC}"
cmake --build . --target blender_tests -j$(nproc)

# Run individual test suites
echo -e "${GREEN}Running Memory Tier tests...${NC}"
./src/blender/tests/blender_tests --gtest_filter="MemoryTierTest.*" || exit 1

echo -e "${GREEN}Running Compression tests...${NC}"
./src/blender/tests/blender_tests --gtest_filter="CompressionTest.*" || exit 1

echo -e "${GREEN}Running Pattern Data tests...${NC}"
./src/blender/tests/blender_tests --gtest_filter="PatternDataTest.*" || exit 1

echo -e "${GREEN}Running API tests...${NC}"
./src/blender/tests/blender_tests --gtest_filter="APITest.*" || exit 1

echo -e "${GREEN}Running Audio Processor tests...${NC}"
./src/blender/tests/blender_tests --gtest_filter="AudioProcessorTest.*" || exit 1

echo -e "${GREEN}Running Audio Metrics tests...${NC}"
./src/blender/tests/blender_tests --gtest_filter="AudioMetricsTest.*" || exit 1

# Generate coverage report if lcov is available
if command -v lcov &> /dev/null; then
    echo -e "${GREEN}Generating coverage report...${NC}"
    lcov --capture --directory . --output-file coverage.info
    lcov --remove coverage.info '/usr/*' --output-file coverage.info
    genhtml coverage.info --output-directory coverage_report
    echo -e "${GREEN}Coverage report generated in ${BUILD_DIR}/coverage_report${NC}"
fi

# Run sanitizer tests
echo -e "${GREEN}Running sanitizer tests...${NC}"
ASAN_OPTIONS=detect_leaks=1 ./src/blender/tests/blender_tests || exit 1

echo -e "${GREEN}All tests passed successfully!${NC}"