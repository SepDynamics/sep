#!/bin/bash
set -uo pipefail

# Configuration
# export CC=/usr/bin/gcc-14
# export CXX=/usr/bin/g++-14
ROOT_DIR="$(cd "$(dirname "$0")" && pwd)"
BUILD_DIR="$ROOT_DIR/build"
LOG_DIR="$BUILD_DIR/logs"
BUILD_LOG="$LOG_DIR/build_log.txt"
CMAKE_LOG="$LOG_DIR/cmake_log.txt"
ERROR_LOG="$LOG_DIR/error_summary.txt"

cd "$ROOT_DIR"

clear 

echo "cleaning files..."

sleep 1

rm -rf compile_commands.json \
    Makefile reports CMakeCache.txt \
    build build.ninja out \
    .codechecker .codacy \
    Makefiles CMakeFiles \
    .ninja_* Testing *.log \
    shutil output/* report.md \
    CTestTestfile* MakeFile 
  
echo "emptying trash..."
sudo rm -rf /workspace/.Trash-1000
cmake \
    --log-level=TRACE \
    --no-warn-unused-cli \
    --compile-no-warning-as-error \
    -B "$BUILD_DIR" -S "$ROOT_DIR" . \
    -DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
    -Wno-dev -Wno-deprecated \
    -Wno-error=deprecated
# Parse command line arguments
# Parse positional arguments for command and target
if [ "$#" -ge 1 ]; then
    COMMAND="$1"
    shift
else
    COMMAND="build"
fi

if [ "$#" -ge 1 ]; then
    TARGET="$1"
    shift
else
    TARGET="all"
fi

# Compilers can be set via environment or --cc/--cxx options
# C_COMPILER="${CC:-/usr/bin/gcc-14}"
# CXX_COMPILER="${CXX:-/usr/bin/g++-14}"

# Parse optional compiler overrides
while [ "$#" -gt 0 ]; do
    case "$1" in
        --cc=*)
            C_COMPILER="${1#*=}"
            ;;
        --cxx=*)
            CXX_COMPILER="${1#*=}"
            ;;
        *)
            echo "[i] Ignoring unknown option: $1"
            ;;
    esac
    shift
done

# Function to show usage
show_usage() {
    echo "Usage: $0 [COMMAND] [TARGET] [--cc=PATH] [--cxx=PATH]"
    echo ""
    echo "Commands:"
    echo "  build    - Build the project (default)"
    echo "  clean    - Clean build directory"
    echo "  rebuild  - Clean and build"
    echo "  test     - Build and run tests"
    echo "  help     - Show this help"
    echo ""
    echo "Targets:"
    echo "  all      - Build all targets (default)"
    echo "  <target> - Build specific target"
    echo ""
    echo "Options:"
    echo "  --cc=PATH   - Path to C compiler (overrides \$CC)"
    echo "  --cxx=PATH  - Path to C++ compiler (overrides \$CXX)"
    echo ""
    echo "Environment variables:"
    echo "  CC   - Default C compiler path"
    echo "  CXX  - Default C++ compiler path"
    echo ""
    echo "Examples:"
    echo "  $0 build              # Build all targets"
    echo "  $0 clean              # Clean build directory"
    echo "  $0 rebuild            # Clean and rebuild all"
}

# Function to clean build directory
clean_build() {
    echo "[*] Cleaning build directory..."
    if [ -d "$BUILD_DIR" ]; then
        rm -rf "$BUILD_DIR"
        echo "[✓] Build directory cleaned"
    else
        echo "[i] Build directory already clean"
    fi
}

# Function to setup build directory
setup_build_dir() {
    echo "[*] Setting up build directory..."
    mkdir -p "$BUILD_DIR" "$LOG_DIR"
    cd "$BUILD_DIR"
}

# Function to configure with CMake
configure_cmake() {
    echo "[*] Configuring CMake with Ninja generator..."

    echo "[i] Using compilers: C=$C_COMPILER, CXX=$CXX_COMPILER"
    
    local cmake_c_compiler="$C_COMPILER"
    local cmake_cxx_compiler="$CXX_COMPILER"

    cmake -G "Unix Makefiles" .. \
        -DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
        -DCMAKE_CUDA_COMPILER=nvcc \
        > "$CMAKE_LOG" 2>&1 || {
        echo "[!!] CMake configuration failed. See $CMAKE_LOG"
        echo "Last 10 lines of CMake log:"
        tail -n 10 "$CMAKE_LOG"
        exit 1
    }
    
    echo "[✓] CMake configuration successful"
}

# Function to build with Ninja
build_project() {
    echo "[*] Building target: $TARGET with CMake..."
    
    # Check if Makefile -v exists in build dir, if not check workspace root
    local CMakeFile=""
    if [ -f "$BUILD_DIR/Makefile" ]; then
        CMakeFile="$BUILD_DIR/Makefile"
        cd "$BUILD_DIR"
    elif [ -f "$ROOT_DIR/Makefile" ]; then
        CMakeFile="$ROOT_DIR/Makefile"
        cd "$ROOT_DIR"
        echo "[i] Using Makefile from workspace root"
    else
        echo "[!!] No Makefile file found in $BUILD_DIR or $ROOT_DIR"
        return 1
    fi
    
    # Set compiler environment variables to ensure consistent compiler usage
    # export CC="$C_COMPILER"
    # export CXX="$CXX_COMPILER"
    export CUDACXX="nvcc"
    # Force the use of libc++ instead of libc++
    echo "[i] Using compilers: CC=${CC:-}, CXX=${CXX:-}"
    echo "[i] Forcing standard library: libc++"
    
    
    # Stream build progress, capture all to log, continue on failure
    stdbuf -oL make -j"$(nproc)" "$TARGET" 2>&1 | tee "$BUILD_LOG" || true
    
    # Summarize errors by file and type
    echo -e "\n[!] Condensed Error Summary:\n"
    grep -E "error:|undefined reference|not declared|invalid use|no matching function|no member named|has no member|cannot convert" "$BUILD_LOG" \
      | sed -E 's/^(.*):([0-9]+):[0-9]*: error: (.*)$/\1:\2: \3/' \
      | sort -u | awk -F ':' '
        BEGIN { print "Grouped by file and error type:" }
        {
          file = $1
          line = $2
          msg = $3
          if (length(msg) > 100) msg = substr(msg, 1, 97) "..."
          key = file ":" msg
          if (key != prev_key && prev_key != "") {
            print "    ↪ " count " occurrence(s)"
          }
          if (file != prev_file && prev_file != "") {
            print ""
          }
          if (file != prev_file) {
            print "→ " file
            count = 0
          }
          if (key != prev_key) {
            print "  - " msg " (line " line ")"
            count = 0
          }
          count++
          prev_file = file
          prev_key = key
          files[file] = 1
          if (msg ~ /sin.*not a member|sqrt.*not a member/) math_files[file] = 1
        }
        END {
          if (count > 0) print "    ↪ " count " occurrence(s)"
          print "\nSuggested next target to fix:"
          for (f in math_files) {
            print "  - " f " (math-related errors)"
            exit
          }
          for (f in files) {
            print "  - " f
            exit
          }
        }'
    
    echo "[✓] Build completed"
}

# Function to generate error summary
generate_error_summary() {
    echo "[*] Generating error summary..."
    
    # Check for missing compatibility headers in files with std namespace errors
    echo "[*] Checking for missing compatibility headers..."
    grep -E "no member named '(strtod|strtol|strtoul|strtof|strtold|strtoull|strtoll)'" "$BUILD_LOG" 2>/dev/null | \
    awk -F ':' '{print $1}' | sort -u | while read -r file; do
        if [ -f "$file" ]; then
            if ! grep -q "#include.*cuda_unified_fix.h" "$file" && ! grep -q "#include.*sep/math_common.h" "$file"; then
                echo "[!] File $file may need to include cuda_unified_fix.h or sep/math_common.h"
            fi
        fi
    done
    
    # Extract and summarize errors
    {
        echo "=== BUILD ERROR SUMMARY ==="
        echo "Generated: $(date)"
        echo "Target: $TARGET"
        echo ""
        
        # Look for common error patterns
        grep -E "error:|undefined reference|not declared|invalid use|no matching function|no member named|has no member|cannot convert" "$BUILD_LOG" 2>/dev/null | \
        sed -E 's/^(.*):([0-9]+):[0-9]*: error: (.*)$/\1:\2: \3/' | \
        sort -u | awk -F ':' '
            BEGIN { 
                print "Errors grouped by file:"
                print "======================"
            }
            {
                file = $1
                line = $2
                msg = $3
                if (length(msg) > 100) msg = substr(msg, 1, 97) "..."
                
                if (file != prev_file && prev_file != "") {
                    print ""
                }
                if (file != prev_file) {
                    print "→ " file
                    prev_file = file
                }
                print "  Line " line ": " msg
                
                # Track file types for suggestions
                files[file] = 1
                if (msg ~ /sin.*not a member|sqrt.*not a member|math/) math_files[file] = 1
            }
            END {
                print ""
                print "Suggested fixes:"
                print "==============="
                for (f in math_files) {
                    print "  - " f " (add #include <cmath>)"
                }
                if (length(files) > 0 && length(math_files) == 0) {
                    for (f in files) {
                        print "  - Check " f " for missing includes or declarations"
                        break
                    }
                }
            }'
    } > "$ERROR_LOG"
    
    echo "[!] Error summary saved to: $ERROR_LOG"
    echo ""
    echo "=== QUICK ERROR OVERVIEW ==="
    head -n 20 "$ERROR_LOG"
    echo ""
    echo "Full error details in: $ERROR_LOG"
}

# Function to show build status
show_status() {
    echo ""
    echo "=== BUILD STATUS ==="
    echo "Build directory: $BUILD_DIR"
    echo "Logs directory: $LOG_DIR"
    echo ""
    
    if [ -f "$BUILD_LOG" ]; then
        echo "Recent build log: $BUILD_LOG"
        echo "Build log size: $(du -h "$BUILD_LOG" | cut -f1)"
    fi
    
    if [ -f "$ERROR_LOG" ]; then
        echo "Error summary: $ERROR_LOG"
    fi
    
    if [ -f "$BUILD_DIR/compile_commands.json" ]; then
        echo "[✓] Compile commands generated for IDE support"
    fi
}

# Function to run tests
run_tests() {
    echo "[*] Running tests..."
    
    # Check if the test script exists
    if [ -f "$ROOT_DIR/scripts/run_tests.sh" ]; then
        echo "[i] Using test script: $ROOT_DIR/scripts/run_tests.sh"
        
        # Make sure it's executable
        chmod +x "$ROOT_DIR/scripts/run_tests.sh"
        
        # Run the test script
        "$ROOT_DIR/scripts/run_tests.sh"
    else
        echo "[!!] Test script not found: $ROOT_DIR/scripts/run_tests.sh"
        echo "[i] Running CTest directly..."
        
        # Fall back to running CTest directly
        cd "$BUILD_DIR"
        ctest --output-on-failure
    fi
}

# Main execution
main() {
    case "$COMMAND" in
        "help"|"-h"|"--help")
            show_usage
            exit 0
            ;;
        "clean")
            clean_build
            exit 0
            ;;
        "rebuild")
            clean_build
            setup_build_dir
            configure_cmake
            build_project
            ;;
        "test")
            # First build the project
            setup_build_dir
            
            # Only reconfigure if needed
            if [ ! -f "$BUILD_DIR/Makefile" ]; then
                configure_cmake
            else
                echo "[i] Using existing Makefile configuration"
                cd "$BUILD_DIR"
            fi
            
            # Build with BUILD_TESTING enabled
            cmake -DBUILD_TESTING=ON .
            build_project
            
            # Run the tests
            run_tests
            ;;
        "build"|"")
            setup_build_dir
            
            # Only reconfigure if needed
            if [ ! -f "$BUILD_DIR/Makefile" ]; then
                configure_cmake
            else
                echo "[i] Using existing configuration"
                cd "$BUILD_DIR"
            fi
            
            build_project
            ;;
        *)
            echo "[!!] Unknown command: $COMMAND"
            show_usage
            exit 1
            ;;
    esac
    
    show_status
}

# Trap to ensure we return to workspace directory
trap 'cd "$ROOT_DIR"' EXIT

# Run main function
main "$@"