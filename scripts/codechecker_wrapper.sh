#!/bin/bash

# CodeChecker wrapper script for SEP Engine
# This script runs the build command and captures CodeChecker warnings

# Create necessary directories
mkdir -p ${PWD}/.codechecker/reports

# Colors for output formatting
RED='\033[0;31m'
YELLOW='\033[1;33m'
GREEN='\033[0;32m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

echo -e "${BLUE}=== Running build with CodeChecker integration ===${NC}"

# Run CodeChecker log to capture build
echo -e "${BLUE}Capturing build commands...${NC}"
CodeChecker log -b "make $*" -o ${PWD}/.codechecker/compile_commands.json

# Check if the build was successful
if [ $? -ne 0 ]; then
    echo -e "${RED}Build failed. Aborting CodeChecker analysis.${NC}"
    exit 1
fi

# Run CodeChecker analyze
echo -e "${BLUE}Running static analysis...${NC}"
CodeChecker analyze ${PWD}/.codechecker/compile_commands.json -o ${PWD}/.codechecker/reports --analyzers clangsa clang-tidy

# Parse and display results
echo -e "${BLUE}=== CodeChecker Results ===${NC}"
RESULTS=$(CodeChecker parse ${PWD}/.codechecker/reports --print-steps)

# Count warnings and errors
WARNING_COUNT=$(echo "$RESULTS" | grep -c "warning:")
ERROR_COUNT=$(echo "$RESULTS" | grep -c "error:")

# Display summary
echo -e "${YELLOW}Found $WARNING_COUNT warnings and $ERROR_COUNT errors${NC}"

# Display results with syntax highlighting
echo "$RESULTS" | sed -E "s/^(.*)(warning:)(.*)/\1${YELLOW}\2${NC}\3/g" | sed -E "s/^(.*)(error:)(.*)/\1${RED}\2${NC}\3/g"

# Generate HTML report
echo -e "${BLUE}Generating HTML report in ${PWD}/.codechecker/html${NC}"
CodeChecker parse ${PWD}/.codechecker/reports -e html -o ${PWD}/.codechecker/html

# Store results in CodeChecker server if it's running
if nc -z localhost 8001 2>/dev/null; then
    echo -e "${BLUE}Storing results in CodeChecker server...${NC}"
    CodeChecker store ${PWD}/.codechecker/reports --name sep-engine --url http://localhost:8001/Default
    echo -e "${GREEN}Results stored. View them at http://localhost:8001${NC}"
else
    echo -e "${YELLOW}CodeChecker server not running. Results not stored.${NC}"
fi

# Exit with error if there are errors
if [ $ERROR_COUNT -gt 0 ]; then
    echo -e "${RED}Build completed with $ERROR_COUNT errors.${NC}"
    exit 1
fi

echo -e "${GREEN}Build completed successfully with $WARNING_COUNT warnings.${NC}"
exit 0