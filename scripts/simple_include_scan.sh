#!/bin/bash

set -e

# Simple, robust include issue scanner for SEP Engine
# Focuses on finding files that won't compile due to include issues

SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
PROJECT_ROOT="${SCRIPT_DIR}/.."
OUTPUT_DIR="${PROJECT_ROOT}/output"

echo "🔍 Scanning for include issues across SEP Engine..."

mkdir -p "${OUTPUT_DIR}"

ISSUES_FILE="${OUTPUT_DIR}/include_scan_results.txt"
SUMMARY_FILE="${OUTPUT_DIR}/include_scan_summary.txt"

# Clear previous results
> "${ISSUES_FILE}"
> "${SUMMARY_FILE}"

echo "=== INCLUDE ISSUE SCAN ===" >> "${ISSUES_FILE}"
echo "Generated: $(date)" >> "${ISSUES_FILE}"
echo "" >> "${ISSUES_FILE}"

# Use basic include paths that should work
BASIC_INCLUDES="-I${PROJECT_ROOT}/src -I${PROJECT_ROOT}/src/engine -I${PROJECT_ROOT}/src/quantum -I${PROJECT_ROOT}/src/apps/workbench -I${PROJECT_ROOT}/src/common -I${PROJECT_ROOT}/src/connectors -I${PROJECT_ROOT}/src/apps/common -I/usr/local/cuda/include"

# Add build-time dependencies if they exist
if [ -d "${PROJECT_ROOT}/build/_deps/imgui-src" ]; then
    BASIC_INCLUDES="$BASIC_INCLUDES -I${PROJECT_ROOT}/build/_deps/imgui-src -I${PROJECT_ROOT}/build/_deps/imgui-src/backends"
fi

if [ -d "${PROJECT_ROOT}/build/_deps/implot-src" ]; then
    BASIC_INCLUDES="$BASIC_INCLUDES -I${PROJECT_ROOT}/build/_deps/implot-src"
fi

if [ -d "${PROJECT_ROOT}/build/_deps/spdlog-src/include" ]; then
    BASIC_INCLUDES="$BASIC_INCLUDES -I${PROJECT_ROOT}/build/_deps/spdlog-src/include"
fi

# Test compilation function
test_compilation() {
    local file="$1"
    local temp_log=$(mktemp)
    local has_error=false
    
    # Try to compile just for syntax/include checking (force C++ for .h files)
    if ! clang-15 $BASIC_INCLUDES -std=c++17 -x c++ -fsyntax-only "$file" > "$temp_log" 2>&1; then
        has_error=true
        echo "❌ $file"
        
        # Extract key error info
        echo "ERRORS IN: $file" >> "${ISSUES_FILE}"
        grep -E "(fatal error|error:|no such file|file not found|no member named|unknown type)" "$temp_log" | head -5 >> "${ISSUES_FILE}"
        echo "" >> "${ISSUES_FILE}"
        
        # Count error types for summary
        grep -E "(file not found|no member named|unknown type|namespace.*has no member)" "$temp_log" | while read -r error; do
            echo "$file: $error" >> "${SUMMARY_FILE}.tmp"
        done
    else
        echo "✅ $file"
    fi
    
    rm -f "$temp_log"
    [ "$has_error" = true ]
}

# Statistics
total_files=0
problem_files=0

echo "Scanning header files for include issues..."
echo "=== HEADER FILES ===" >> "${ISSUES_FILE}"

# Test headers first - they're the main source of include issues
while IFS= read -r -d '' file; do
    total_files=$((total_files + 1))
    if test_compilation "$file"; then
        problem_files=$((problem_files + 1))
    fi
done < <(find "${PROJECT_ROOT}/src" -name "*.h" -o -name "*.hpp" -print0 | sort -z)

echo ""
echo "Scanning critical source files..."
echo "=== SOURCE FILES ===" >> "${ISSUES_FILE}"

# Test some key source files that commonly have issues  
CRITICAL_SOURCES=(
    "src/quantum/gpu_context.h"
    "src/apps/workbench/core/workbench_core.cpp"
    "src/apps/workbench/core/alpha_tracker.cpp"
    "src/engine/cuda_api.hpp"
    "src/quantum/pattern_processor.cpp"
)

for source in "${CRITICAL_SOURCES[@]}"; do
    if [ -f "${PROJECT_ROOT}/$source" ]; then
        total_files=$((total_files + 1))
        if test_compilation "${PROJECT_ROOT}/$source"; then
            problem_files=$((problem_files + 1))
        fi
    fi
done

# Generate summary
echo "=== SCAN SUMMARY ===" > "${SUMMARY_FILE}"
echo "Generated: $(date)" >> "${SUMMARY_FILE}"
echo "" >> "${SUMMARY_FILE}"
echo "📊 RESULTS:" >> "${SUMMARY_FILE}"
echo "- Total files scanned: $total_files" >> "${SUMMARY_FILE}"
echo "- Files with issues: $problem_files" >> "${SUMMARY_FILE}"
echo "- Success rate: $(( (total_files - problem_files) * 100 / total_files ))%" >> "${SUMMARY_FILE}"
echo "" >> "${SUMMARY_FILE}"

# Analyze error patterns if temp file exists
if [ -f "${SUMMARY_FILE}.tmp" ]; then
    echo "🔥 TOP ERROR PATTERNS:" >> "${SUMMARY_FILE}"
    sort "${SUMMARY_FILE}.tmp" | uniq -c | sort -nr | head -10 >> "${SUMMARY_FILE}"
    echo "" >> "${SUMMARY_FILE}"
    
    echo "📁 MISSING HEADERS:" >> "${SUMMARY_FILE}"
    grep "file not found" "${SUMMARY_FILE}.tmp" | grep -o "'[^']*'" | sort | uniq -c | sort -nr | head -10 >> "${SUMMARY_FILE}"
    echo "" >> "${SUMMARY_FILE}"
    
    echo "🔧 NAMESPACE ISSUES:" >> "${SUMMARY_FILE}"
    grep -E "no member named|unknown type|namespace.*has no member" "${SUMMARY_FILE}.tmp" | sort | uniq -c | sort -nr | head -10 >> "${SUMMARY_FILE}"
    
    rm -f "${SUMMARY_FILE}.tmp"
fi

# Add recommended fixes
echo "" >> "${SUMMARY_FILE}"
echo "🛠️  RECOMMENDED IMMEDIATE FIXES:" >> "${SUMMARY_FILE}"
echo "1. Check if engine/standard_includes.h path is correct" >> "${SUMMARY_FILE}"
echo "2. Fix CUDA type conflicts between headers" >> "${SUMMARY_FILE}"
echo "3. Resolve workbench namespace issues" >> "${SUMMARY_FILE}"
echo "4. Add missing forward declarations" >> "${SUMMARY_FILE}"

echo ""
echo "✅ Scan complete!"
echo "📄 Results: ${ISSUES_FILE}"
echo "📋 Summary: ${SUMMARY_FILE}"
echo ""
echo "Quick stats: $problem_files/$total_files files have issues"

# Show top issues if summary exists
if [ -f "${SUMMARY_FILE}" ]; then
    echo ""
    echo "🔥 Top issues:"
    grep -A5 "TOP ERROR PATTERNS" "${SUMMARY_FILE}" | tail -5
fi
