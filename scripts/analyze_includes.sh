#!/bin/bash

set -e

# Analyze include dependencies for SEP Engine using clang-15's -H flag
# This shows the include tree and helps identify problematic dependencies

SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
PROJECT_ROOT="${SCRIPT_DIR}/.."
OUTPUT_DIR="${PROJECT_ROOT}/output"

echo "Analyzing include dependencies with clang-15..."

# Ensure output directory exists
mkdir -p "${OUTPUT_DIR}"

# Build with include dependency tracking
echo "Running build with include dependency analysis..."

# Modify CMAKE flags to add -H for include tree output
export EXTRA_CXX_FLAGS="-H"
export EXTRA_CUDA_FLAGS="-Xcompiler=-H"

# Run a focused build on problem files to get include trees
INCLUDE_LOG="${OUTPUT_DIR}/include_analysis.txt"
INCLUDE_SUMMARY="${OUTPUT_DIR}/include_summary.txt"

# Clear previous output
> "${INCLUDE_LOG}"
> "${INCLUDE_SUMMARY}"

echo "=== INCLUDE DEPENDENCY ANALYSIS ===" >> "${INCLUDE_LOG}"
echo "Generated: $(date)" >> "${INCLUDE_LOG}"
echo "" >> "${INCLUDE_LOG}"

# Test specific problem files
PROBLEM_FILES=(
#    "/sep/src/quantum/gpu_context.h"
    "/sep/src/apps/workbench/core/workbench_core.hpp"
    "/sep/src/apps/workbench/core/alpha_tracker.h"
    "/sep/src/engine/cuda_api.hpp"
)

echo "Analyzing problematic header files..."

for file in "${PROBLEM_FILES[@]}"; do
    if [ -f "$file" ]; then
        echo "=== ANALYZING: $file ===" >> "${INCLUDE_LOG}"
        echo "Analyzing: $file"
        
        # Use clang to show include dependencies
        clang-15 -H -I/sep/src -I/sep/src/engine -I/sep/src/quantum -I/sep/src/apps/workbench \
                 -I/usr/local/cuda/include -fsyntax-only "$file" >> "${INCLUDE_LOG}" 2>&1 || true
        
        echo "" >> "${INCLUDE_LOG}"
    fi
done

# Analyze compile_commands.json for repeated includes
if [ -f "${PROJECT_ROOT}/compile_commands.json" ]; then
    echo "Analyzing compile_commands.json for include patterns..."
    echo "=== INCLUDE PATTERN ANALYSIS ===" >> "${INCLUDE_SUMMARY}"
    
    # Extract all -I flags to see include paths
    echo "Include paths used:" >> "${INCLUDE_SUMMARY}"
    grep -o '\-I[^"]*' "${PROJECT_ROOT}/compile_commands.json" | sort | uniq -c | sort -nr >> "${INCLUDE_SUMMARY}"
    echo "" >> "${INCLUDE_SUMMARY}"
    
    # Look for problematic header patterns
    echo "Files with many includes (potential issues):" >> "${INCLUDE_SUMMARY}"
    grep -o '"[^"]*\.\(h\|hpp\|cuh\)"' "${PROJECT_ROOT}/compile_commands.json" | \
        sort | uniq -c | sort -nr | head -20 >> "${INCLUDE_SUMMARY}"
fi

# Analyze current error patterns
if [ -f "${OUTPUT_DIR}/errors.txt" ]; then
    echo "=== ERROR PATTERN ANALYSIS ===" >> "${INCLUDE_SUMMARY}"
    echo "Most common error patterns:" >> "${INCLUDE_SUMMARY}"
    
    # Count error types
    grep -o "file not found\|no member named\|unknown type name\|namespace.*has no member" "${OUTPUT_DIR}/errors.txt" | \
        sort | uniq -c | sort -nr >> "${INCLUDE_SUMMARY}"
    echo "" >> "${INCLUDE_SUMMARY}"
    
    # Find missing headers
    echo "Missing headers:" >> "${INCLUDE_SUMMARY}"
    grep "file not found" "${OUTPUT_DIR}/errors.txt" | \
        grep -o "'[^']*'" | sort | uniq -c | sort -nr >> "${INCLUDE_SUMMARY}"
fi

echo "Include analysis complete!"
echo "Results saved to:"
echo "  - Detailed analysis: ${INCLUDE_LOG}"
echo "  - Summary: ${INCLUDE_SUMMARY}"

# Create actionable recommendations
RECOMMENDATIONS="${OUTPUT_DIR}/include_recommendations.txt"
echo "=== INCLUDE DEPENDENCY RECOMMENDATIONS ===" > "${RECOMMENDATIONS}"
echo "Generated: $(date)" >> "${RECOMMENDATIONS}"
echo "" >> "${RECOMMENDATIONS}"

echo "IMMEDIATE FIXES NEEDED:" >> "${RECOMMENDATIONS}"
echo "1. Missing standard_includes.h - many engine files expect this" >> "${RECOMMENDATIONS}"
echo "2. CUDA header conflicts between cuda_api.hpp and cuda_base.h" >> "${RECOMMENDATIONS}"
echo "3. Workbench namespace issues - nested sep::workbench::sep" >> "${RECOMMENDATIONS}"
echo "4. Signal handling includes missing for workbench" >> "${RECOMMENDATIONS}"
echo "" >> "${RECOMMENDATIONS}"

echo "RECOMMENDED ACTIONS:" >> "${RECOMMENDATIONS}"
echo "1. Create or fix engine/standard_includes.h" >> "${RECOMMENDATIONS}"
echo "2. Consolidate CUDA type definitions" >> "${RECOMMENDATIONS}"
echo "3. Fix workbench namespace declarations" >> "${RECOMMENDATIONS}"
echo "4. Add missing forward declarations" >> "${RECOMMENDATIONS}"
echo "5. Use precompiled headers for common includes" >> "${RECOMMENDATIONS}"

echo ""
echo "Recommendations saved to: ${RECOMMENDATIONS}"
