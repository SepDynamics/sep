#!/bin/bash

set -e

# Run include-what-you-use analysis on the SEP Engine codebase
# This script helps identify unnecessary includes and missing forward declarations

SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
PROJECT_ROOT="${SCRIPT_DIR}/.."
BUILD_DIR="${PROJECT_ROOT}/build"
OUTPUT_DIR="${PROJECT_ROOT}/output"

# Ensure output directory exists
mkdir -p "${OUTPUT_DIR}"

echo "Running include-what-you-use analysis..."

# Check if IWYU is available
if ! command -v include-what-you-use &> /dev/null; then
    echo "ERROR: include-what-you-use not found. Install with: apt install iwyu"
    exit 1
fi

# Ensure build directory exists and has compile_commands.json
if [ ! -f "${BUILD_DIR}/compile_commands.json" ]; then
    echo "ERROR: compile_commands.json not found. Run build first."
    exit 1
fi

# Run IWYU on source files
echo "Analyzing source files..."

# Focus on our source files, exclude external dependencies
IWYU_OUTPUT="${OUTPUT_DIR}/iwyu_analysis.txt"
IWYU_FIXES="${OUTPUT_DIR}/iwyu_fixes.py"

# Clear previous output
> "${IWYU_OUTPUT}"

# Analyze engine source files
echo "=== ENGINE FILES ===" >> "${IWYU_OUTPUT}"
find "${PROJECT_ROOT}/src/engine" -name "*.cpp" -o -name "*.cu" | while read -r file; do
    echo "Analyzing: $file"
    include-what-you-use -p "${BUILD_DIR}" "$file" >> "${IWYU_OUTPUT}" 2>&1 || true
done

# Analyze quantum source files  
echo "=== QUANTUM FILES ===" >> "${IWYU_OUTPUT}"
find "${PROJECT_ROOT}/src/quantum" -name "*.cpp" -o -name "*.cu" | while read -r file; do
    echo "Analyzing: $file" 
    include-what-you-use -p "${BUILD_DIR}" "$file" >> "${IWYU_OUTPUT}" 2>&1 || true
done

# Analyze workbench source files
echo "=== WORKBENCH FILES ===" >> "${IWYU_OUTPUT}"
find "${PROJECT_ROOT}/src/apps/workbench" -name "*.cpp" | while read -r file; do
    echo "Analyzing: $file"
    include-what-you-use -p "${BUILD_DIR}" "$file" >> "${IWYU_OUTPUT}" 2>&1 || true
done

echo "IWYU analysis complete. Results saved to: ${IWYU_OUTPUT}"

# Try to extract actionable suggestions
echo "Extracting actionable suggestions..."
SUGGESTIONS="${OUTPUT_DIR}/iwyu_suggestions.txt"

grep -E "(should add these lines:|should remove these lines:|full include-list for)" "${IWYU_OUTPUT}" > "${SUGGESTIONS}" || true

echo "Actionable suggestions saved to: ${SUGGESTIONS}"
echo ""
echo "To apply automatic fixes:"
echo "1. Install iwyu tool: apt install iwyu" 
echo "2. Generate fixes: iwyu_tool.py -p ${BUILD_DIR} > ${IWYU_FIXES}"
echo "3. Apply fixes: python3 ${IWYU_FIXES}"
