#!/bin/bash

# auto_analyze_includes.sh: Automatically analyze include dependencies for an entire C++ project.
# Finds and reports files with include issues, offering actionable insights.

set -euo pipefail # Exit on error, unset variables, and pipeline errors

# --- Configuration ---
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
PROJECT_ROOT="${SCRIPT_DIR}/.."
DEFAULT_OUTPUT_DIR="${PROJECT_ROOT}/output" # Changed output directory name for clarity
TEMP_DIR="" # To be created by mktemp -d

# --- Colors for output ---
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[0;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# --- Variables for output files ---
INCLUDE_ISSUES=""
INCLUDE_SUMMARY=""
DETAILED_ERRORS=""

# --- Helper Functions ---

cleanup() {
    echo -e "\n${YELLOW}Cleaning up temporary files...${NC}"
    if [[ -n "${TEMP_DIR}" && -d "${TEMP_DIR}" ]]; then
        rm -rf "${TEMP_DIR}"
    fi
}
trap cleanup EXIT # Ensure cleanup on script exit

check_command() {
    if ! command -v "$1" &> /dev/null; then
        echo -e "${RED}Error: '$1' command not found. Please install it.${NC}"
        exit 1
    fi
}

get_compile_flags() {
    local compile_flags=""
    local compile_commands_path="${PROJECT_ROOT}/compile_commands.json"

    if [[ -f "${compile_commands_path}" ]]; then
        echo -e "${BLUE}Extracting compile flags from compile_commands.json...${NC}"
        # Using grep -P (Perl regex) for more advanced patterns and sed for cleanup
        # This is a more robust way to extract flags than just -I or -D
        compile_flags=$(jq -r '.[].command' "${compile_commands_path}" | \
                        grep -oP '\s(?:-I|-D|--std=|--include=)[^[:space:]]+' | \
                        sort | uniq | tr '\n' ' ')
        
        # Add a default C++ standard if not already present
        if [[ ! "$compile_flags" =~ -std=c\+\+ ]]; then
            compile_flags+=" -std=c++17"
        fi
    else
        echo -e "${YELLOW}Warning: compile_commands.json not found. Using fallback include paths.${NC}"
        compile_flags="-I/sep/src -I/sep/src/engine -I/sep/src/quantum -I/sep/src/apps/workbench -I/sep/src/common -I/sep/src/connectors -I/usr/local/cuda/include -std=c++17"
    fi
    echo "$compile_flags"
}

# Function to test a single file for include issues
# Returns 0 for no issues, 1 for issues
test_file_includes() {
    local file_path="$1"
    local relative_file_path="${file_path#${PROJECT_ROOT}/}" # Path relative to project root
    local temp_output="${TEMP_DIR}/$(basename "${file_path}").log"
    
    # Use clang++ for C++ files and clang for C/CUDA if necessary, though clang-15 should handle most
    local compiler="clang-15" 
    
    echo -n "  ${relative_file_path}: "

    # Attempt to compile the file for syntax only
    if ! "${compiler}" ${COMPILE_FLAGS} -fsyntax-only "${file_path}" > "${temp_output}" 2>&1; then
        echo -e "${RED}❌ ISSUES${NC}"
        
        # Log the specific errors to detailed log
        echo "=== ISSUES IN: ${relative_file_path} ===" >> "${DETAILED_ERRORS}"
        cat "${temp_output}" >> "${DETAILED_ERRORS}"
        echo "" >> "${DETAILED_ERRORS}"
        
        # Extract specific error types for summary, prefixing with file
        grep -o "file not found\\|no member named\\|unknown type name\\|namespace.*has no member\\|fatal error" "${temp_output}" | while read -r error_line; do
            echo "${relative_file_path}: ${error_line}" >> "${INCLUDE_ISSUES}"
        done
        return 1 # Indicate issues found
    else
        echo -e "${GREEN}✅ OK${NC}"
        return 0 # Indicate no issues
    fi
}

# --- Main Script Logic ---

echo -e "${BLUE}Auto-analyzing include dependencies across entire project...${NC}"
echo -e "${BLUE}This may take some time for large codebases.${NC}"

# 1. Check prerequisites
check_command "clang-15"
check_command "jq" # For more robust compile_commands.json parsing

# 2. Setup output directories
TEMP_DIR=$(mktemp -d "${DEFAULT_OUTPUT_DIR}/temp_analysis.XXXXXX")
mkdir -p "${DEFAULT_OUTPUT_DIR}"

INCLUDE_ISSUES="${DEFAULT_OUTPUT_DIR}/include_issues.txt"    # Stores specific file errors
INCLUDE_SUMMARY="${DEFAULT_OUTPUT_DIR}/analysis_summary.txt" # High-level summary and recommendations
DETAILED_ERRORS="${DEFAULT_OUTPUT_DIR}/detailed_compile_errors.txt" # Raw compiler output for problematic files

# Clear previous output
> "${INCLUDE_ISSUES}"
> "${INCLUDE_SUMMARY}"
> "${DETAILED_ERRORS}"

echo "=== AUTOMATED INCLUDE ISSUE DETECTION ===" >> "${INCLUDE_ISSUES}"
echo "Generated: $(date)" >> "${INCLUDE_ISSUES}"
echo "" >> "${INCLUDE_ISSUES}"

echo "=== DETAILED COMPILATION ERRORS ===" >> "${DETAILED_ERRORS}"
echo "Generated: $(date)" >> "${DETAILED_ERRORS}"
echo "" >> "${DETAILED_ERRORS}"


# 3. Get global compile flags
COMPILE_FLAGS=$(get_compile_flags)
echo "Using compile flags: ${COMPILE_FLAGS}" >> "${INCLUDE_ISSUES}"
echo "Using compile flags: ${COMPILE_FLAGS}" >> "${DETAILED_ERRORS}"
echo "" >> "${INCLUDE_ISSUES}"
echo "" >> "${DETAILED_ERRORS}"


# 4. Scan files
TOTAL_FILES=0
PROBLEM_FILES=0
HEADER_FILES_SCANNED=0
SOURCE_FILES_SCANNED=0
HEADER_PROBLEM_FILES=0
SOURCE_PROBLEM_FILES=0

echo -e "\n${BLUE}Scanning header files (.h, .hpp, .cuh)...${NC}"
echo "=== HEADER FILE ANALYSIS ===" >> "${DETAILED_ERRORS}"

find "${PROJECT_ROOT}/src" -name "*.h" -o -name "*.hpp" -o -name "*.cuh" | sort | while read -r file; do
    TOTAL_FILES=$((TOTAL_FILES + 1))
    HEADER_FILES_SCANNED=$((HEADER_FILES_SCANNED + 1))
    if test_file_includes "$file"; then # Check exit status directly
        : # No issues, do nothing
    else
        PROBLEM_FILES=$((PROBLEM_FILES + 1))
        HEADER_PROBLEM_FILES=$((HEADER_PROBLEM_FILES + 1))
    fi
done

echo -e "\n${BLUE}Scanning source files (.cpp, .cu)...${NC}"
echo "=== SOURCE FILE ANALYSIS ===" >> "${DETAILED_ERRORS}"

find "${PROJECT_ROOT}/src" -name "*.cpp" -o -name "*.cu" | sort | while read -r file; do
    TOTAL_FILES=$((TOTAL_FILES + 1))
    SOURCE_FILES_SCANNED=$((SOURCE_FILES_SCANNED + 1))
    if test_file_includes "$file"; then # Check exit status directly
        : # No issues, do nothing
    else
        PROBLEM_FILES=$((PROBLEM_FILES + 1))
        SOURCE_PROBLEM_FILES=$((SOURCE_PROBLEM_FILES + 1))
    fi
done

# 5. Generate summary report
echo -e "\n${BLUE}Generating analysis summary...${NC}"
echo "=== AUTOMATED INCLUDE ANALYSIS SUMMARY ===" > "${INCLUDE_SUMMARY}"
echo "Generated: $(date)" >> "${INCLUDE_SUMMARY}"
echo "Analysis performed from: $(pwd)" >> "${INCLUDE_SUMMARY}"
echo "Project root: ${PROJECT_ROOT}" >> "${INCLUDE_SUMMARY}"
echo "" >> "${INCLUDE_SUMMARY}"

echo "SCAN RESULTS:" >> "${INCLUDE_SUMMARY}"
echo "- Total files scanned: ${TOTAL_FILES}" >> "${INCLUDE_SUMMARY}"
echo "  - Header files scanned: ${HEADER_FILES_SCANNED}" >> "${INCLUDE_SUMMARY}"
echo "  - Source files scanned: ${SOURCE_FILES_SCANNED}" >> "${INCLUDE_SUMMARY}"
echo "- Files with include issues: ${PROBLEM_FILES}" >> "${INCLUDE_SUMMARY}"
echo "  - Header files with issues: ${HEADER_PROBLEM_FILES}" >> "${INCLUDE_SUMMARY}"
echo "  - Source files with issues: ${SOURCE_PROBLEM_FILES}" >> "${INCLUDE_SUMMARY}"
echo "" >> "${INCLUDE_SUMMARY}"

# Analyze error patterns from include_issues.txt (per-file summary)
if [[ -s "${INCLUDE_ISSUES}" ]]; then # Check if file is not empty
    echo "TOP ERROR PATTERNS (occurrences per file):" >> "${INCLUDE_SUMMARY}"
    grep -o ": file not found\\|: no member named\\|: unknown type name\\|: namespace.*has no member\\|: fatal error" "${INCLUDE_ISSUES}" | \
        sed 's/^: //g' | sort | uniq -c | sort -nr | head -10 | \
        sed 's/^\s*\([0-9]*\)\s*\(.*\)/\1 instances of "\2"/' >> "${INCLUDE_SUMMARY}"
    echo "" >> "${INCLUDE_SUMMARY}"
    
    echo "MOST PROBLEMATIC MISSING HEADERS (file not found):" >> "${INCLUDE_SUMMARY}"
    grep "file not found" "${INCLUDE_ISSUES}" | \
        grep -o "'[^']*'" | sort | uniq -c | sort -nr | head -10 | \
        sed 's/^\s*\([0-9]*\)\s*'\''\(.*\)'\''/\1 instances of missing header \2/' >> "${INCLUDE_SUMMARY}"
    echo "" >> "${INCLUDE_SUMMARY}"
    
    echo "COMMON UNKNOWN TYPE/NAMESPACE ISSUES:" >> "${INCLUDE_SUMMARY}"
    grep -E "unknown type name|namespace.*has no member|no member named" "${INCLUDE_ISSUES}" | \
        grep -v "file not found" | sort | uniq -c | sort -nr | head -10 | \
        sed 's/^\s*\([0-9]*\)\s*\(.*\)/\1 instances of "\2"/' >> "${INCLUDE_SUMMARY}"
    echo "" >> "${INCLUDE_SUMMARY}"
fi

# Analyze compile_commands.json patterns
if [[ -f "${PROJECT_ROOT}/compile_commands.json" ]]; then
    echo "PROJECT-WIDE COMPILE FLAG ANALYSIS:" >> "${INCLUDE_SUMMARY}"
    echo "Most common include paths used in build:" >> "${INCLUDE_SUMMARY}"
    jq -r '.[].command' "${PROJECT_ROOT}/compile_commands.json" | \
        grep -oP '\s-I[^[:space:]]+' | sort | uniq -c | sort -nr | head -10 >> "${INCLUDE_SUMMARY}"
    echo "" >> "${INCLUDE_SUMMARY}"
    
    echo "Most common defines used in build:" >> "${INCLUDE_SUMMARY}"
    jq -r '.[].command' "${PROJECT_ROOT}/compile_commands.json" | \
        grep -oP '\s-D[^[:space:]]+' | sort | uniq -c | sort -nr | head -10 >> "${INCLUDE_SUMMARY}"
    echo "" >> "${INCLUDE_SUMMARY}"
fi

# Generate actionable recommendations
echo "=== RECOMMENDED FIXES & NEXT STEPS ===" >> "${INCLUDE_SUMMARY}"
echo "" >> "${INCLUDE_SUMMARY}"

if [[ "${PROBLEM_FILES}" -gt 0 ]]; then
    echo "Based on the analysis, here are some prioritized recommendations:" >> "${INCLUDE_SUMMARY}"
    echo "" >> "${INCLUDE_SUMMARY}"

    # General recommendations based on error types
    if grep -q "file not found" "${INCLUDE_ISSUES}"; then
        echo "1. ${RED}HIGH PRIORITY: Resolve 'file not found' errors.${NC}" >> "${INCLUDE_SUMMARY}"
        echo "   - This indicates missing or incorrect #include paths. Check your build system's include paths." >> "${INCLUDE_SUMMARY}"
        echo "   - Verify the exact filename and path. Case sensitivity matters, especially on Linux/macOS." >> "${INCLUDE_SUMMARY}"
        echo "   - If it's a system header (e.g., <string>), ensure your compiler setup is correct." >> "${INCLUDE_SUMMARY}"
        echo "   - Inspect '${INCLUDE_ISSUES}' for specific files reporting this error." >> "${INCLUDE_SUMMARY}"
        echo "" >> "${INCLUDE_SUMMARY}"
    fi

    if grep -q "unknown type name\\|no member named\\|namespace.*has no member" "${INCLUDE_ISSUES}"; then
        echo "2. ${YELLOW}MEDIUM PRIORITY: Address 'unknown type/member/namespace' issues.${NC}" >> "${INCLUDE_SUMMARY}"
        echo "   - In header files (.h/.hpp), consider using ${BLUE}forward declarations${NC} (e.g., 'class MyClass;') instead of full '#include \"MyClass.h\"' whenever only a pointer or reference to a type is needed." >> "${INCLUDE_SUMMARY}"
        echo "   - The full '#include' for a type's definition should primarily be in the .cpp file where its members are accessed or objects are created by value." >> "${INCLUDE_SUMMARY}"
        echo "   - This significantly reduces compilation times and improves build parallelism." >> "${INCLUDE_SUMMARY}"
        echo "   - Review '${INCLUDE_ISSUES}' for instances of this problem, especially in header files." >> "${INCLUDE_SUMMARY}"
        echo "" >> "${INCLUDE_SUMMARY}"
    fi

    # Specific recommendations based on patterns in your original script
    if grep -q "standard_includes.h.*not found" "${INCLUDE_ISSUES}"; then
        echo "3. MEDIUM PRIORITY: Investigate issues with 'engine/standard_includes.h'." >> "${INCLUDE_SUMMARY}"
        echo "   - Ensure this common header exists and its path is correctly configured in your compile flags." >> "${INCLUDE_SUMMARY}"
        echo "   - If it's meant to be a precompiled header, check that it's being handled correctly by the build system." >> "${INCLUDE_SUMMARY}"
        echo "" >> "${INCLUDE_SUMMARY}"
    fi

    if grep -q "cudaStream_t" "${INCLUDE_ISSUES}"; then
        echo "4. LOW PRIORITY: Resolve CUDA type definition conflicts/missing headers." >> "${INCLUDE_SUMMARY}"
        echo "   - Ensure CUDA headers (e.g., 'cuda_runtime.h' or specific ones for types like 'cudaStream_t') are correctly included where CUDA types are used." >> "${INCLUDE_SUMMARY}"
        echo "   - Check for potential conflicts if multiple CUDA versions or libraries are linked." >> "${INCLUDE_SUMMARY}"
        echo "" >> "${INCLUDE_SUMMARY}"
    fi

    if grep -q "sep::workbench::sep" "${INCLUDE_ISSUES}"; then
        echo "5. LOW PRIORITY: Review nested namespace declarations in 'workbench'." >> "${INCLUDE_SUMMARY}"
        echo "   - This pattern 'namespace sep::workbench::sep' often indicates a typo or misunderstanding of namespace nesting." >> "${INCLUDE_SUMMARY}"
        echo "   - It could also suggest missing 'using namespace' directives or incorrect fully qualified names." >> "${INCLUDE_SUMMARY}"
        echo "" >> "${INCLUDE_SUMMARY}"
    fi

    if grep -q "signal\\|SIGINT\\|SIGTERM" "${INCLUDE_ISSUES}"; then
        echo "6. LOW PRIORITY: Add missing signal handling includes (e.g., <csignal> or <signal.h>)." >> "${INCLUDE_SUMMARY}"
        echo "   - If your code uses POSIX signals, ensure the appropriate headers are included." >> "${INCLUDE_SUMMARY}"
        echo "" >> "${INCLUDE_SUMMARY}"
    fi
    
    if grep -q "stddef.h.*not found" "${INCLUDE_ISSUES}"; then
        echo "7. LOW PRIORITY: System header path issues (e.g., 'stddef.h')." >> "${INCLUDE_SUMMARY}"
        echo "   - This usually points to a misconfigured compiler installation or environment variables (e.g., CPATH, CPLUS_INCLUDE_PATH)." >> "${INCLUDE_SUMMARY}"
        echo "   - Less common in modern setups, but check your compiler's default include paths." >> "${INCLUDE_SUMMARY}"
        echo "" >> "${INCLUDE_SUMMARY}"
    fi
    
    echo "Further Actionable Steps:" >> "${INCLUDE_SUMMARY}"
    echo "  - Implement the 'Include What You Use' (IWYU) principle. Every .cpp and .h file should explicitly include all headers it needs, and nothing more." >> "${INCLUDE_SUMMARY}"
    echo "  - Consider using the ${BLUE}include-what-you-use (IWYU) tool${NC} (https://github.com/include-what-you-use/include-what-you-use)." >> "${INCLUDE_SUMMARY}"
    echo "    - IWYU is a clang-based tool specifically designed to automate fixing include problems." >> "${INCLUDE_SUMMARY}"
    echo "    - It can suggest which headers to remove, add, or replace with forward declarations." >> "${INCLUDE_SUMMARY}"
    echo "  - For classes with complex private implementations, consider the ${BLUE}Pimpl (Pointer to Implementation) idiom${NC} to reduce header dependencies." >> "${INCLUDE_SUMMARY}"
    echo "  - Regularly review newly added code for correct include hygiene." >> "${INCLUDE_SUMMARY}"
    echo "" >> "${INCLUDE_SUMMARY}"

else
    echo "${GREEN}Congratulations! No major include issues were automatically detected.${NC}" >> "${INCLUDE_SUMMARY}"
    echo "  - Keep practicing good include hygiene." >> "${INCLUDE_SUMMARY}"
    echo "  - Consider running 'include-what-you-use' for more fine-grained optimizations." >> "${INCLUDE_SUMMARY}"
    echo "" >> "${INCLUDE_SUMMARY}"
fi


echo -e "\n${BLUE}Analysis complete!${NC}"
echo -e "📊 Results saved to:${NC}"
echo -e "   - ${YELLOW}Detailed issues (per-file error summary):${NC} ${INCLUDE_ISSUES}"
echo -e "   - ${YELLOW}Comprehensive summary & recommendations:${NC} ${INCLUDE_SUMMARY}"
echo -e "   - ${YELLOW}Raw compiler output for problematic files:${NC} ${DETAILED_ERRORS}"

# Show quick summary on console
echo -e "\n${BLUE}📋 Quick Summary:${NC}"
if [[ -f "${INCLUDE_SUMMARY}" ]]; then
    grep -E "Total files|Files with|HIGH PRIORITY:|MEDIUM PRIORITY:|Congratulations!" "${INCLUDE_SUMMARY}" | head -10
else
    echo "Summary file not found: ${INCLUDE_SUMMARY}"
fi