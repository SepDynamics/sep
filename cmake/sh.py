import os

# Define the content of the new, improved shell script
# Using an f-string allows for easy variable insertion if needed later
shell_script_content = r"""#!/bin/bash
# ==============================================================================
# SEP Codebase Snapshot Tool (v2)
# ==============================================================================
# This script creates a focused, single-file snapshot of the SEP codebase,
# intended for use as context for AI agents. It intelligently includes
# source code, build scripts, and documentation while aggressively excluding
# build artifacts, temporary files, and compiled binaries.
# ==============================================================================

# --- Configuration ---
# Exit immediately if a command exits with a non-zero status.
set -e

# The root directory of the project you want to snapshot.
WORKSPACE="/sep"

# Define the output file for the snapshot.
TIMESTAMP=$(date "+%Y%m%d_%H%M%S")
OUTPUT_FILE="/workspace/sep_code_snapshot_${TIMESTAMP}.txt"

# --- Main Logic ---

echo "Starting snapshot process..."
echo "Source Directory: ${WORKSPACE}"
echo "Output File: ${OUTPUT_FILE}"
echo "------------------------------------------------------------"

# Use a single command group to redirect all output to the file
{
    # --- 1. Print the main header ---
    echo "--- START OF FILE: ${OUTPUT_FILE##*/} ---"
    echo "############################################################"
    echo "# Project Snapshot: ${WORKSPACE}"
    echo "# Generated on: $(date)"
    echo "############################################################"
    echo

    # --- 2. Generate the Directory Structure ---
    # This section provides a clean overview of the project layout.
    # It uses 'tree' if available, with a comprehensive exclusion pattern.
    TREE_EXCLUDE_PATTERN="build|CMakeFiles|*.o|*.a|*.so|*.pdb|*.dir|*.swp|*.swo|CTestTestfile.cmake|imgui.ini|link.txt|progress.*|cmake_install.cmake|install_manifest.txt|DependInfo.cmake|depend.make|flags.make|cmake_clean*|*cache*|Cache.txt"

    echo "## Directory Structure"
    echo '```'
    if command -v tree &> /dev/null; then
        # Use tree with the exclusion pattern for a clean view
        tree "$WORKSPACE" -I "$TREE_EXCLUDE_PATTERN"
    else
        # Provide a fallback if 'tree' is not installed and a message
        echo "NOTE: 'tree' command not found. Install it for a better directory view."
        echo "Using 'find' as a fallback..."
        find "$WORKSPACE" -not \( \
            -path "*/build/*" -o \
            -path "*/CMakeFiles/*" -o \
            -path "*/.git/*" -o \
            -name ".*" \
        \) | sort
    fi
    echo '```'
    echo

    # --- 3. Concatenate all important file contents ---
    # This is the core of the script. It uses a single, powerful 'find'
    # command to locate all relevant files and print their contents.
    echo "## Source Code, Build Scripts, and Configuration Files"
    echo

    # The 'find' command works as follows:
    # 1. Start at the WORKSPACE root.
    # 2. -prune: Efficiently skip entire directories we don't want to descend into.
    # 3. -type f: Only consider files.
    # 4. -o: Chain together conditions to find files by name or extension.
    # 5. -not: Exclude specific files that might otherwise match.
    find "$WORKSPACE" \
        -type d \( -name "build" -o -name "CMakeFiles" -o -name ".git" \) -prune \
        -o \
        -type f \( \
            -name "*.h" -o -name "*.hpp" -o -name "*.cuh" \
            -o -name "*.c" -o -name "*.cpp" -o -name "*.cu" \
            -o -name "CMakeLists.txt" -o -name "*.cmake" \
            -o -name "*.sh" -o -name "*.py" \
            -o -name "*.json" -o -name "*.md" -o -name "*.txt" \
            -o -name "Makefile" -o -name "*.ini" -o -name "*.ts" \
        \) \
        -not -name "*_snapshot_*.txt" \
        -print | sort | while read -r file; do
        if [[ -f "$file" ]]; then
            echo "--- START OF FILE ${file} ---"
            echo
            # Use 'cat -n' to add line numbers for better context
            cat -n "$file"
            echo
            echo "--- END OF FILE ${file} ---"
            echo
            echo
        fi
    done

} > "$OUTPUT_FILE"

echo "------------------------------------------------------------"
echo "✅ Snapshot complete!"
echo "Focused codebase snapshot has been written to:"
echo "${OUTPUT_FILE}"

"""

def create_snapshot_script():
    """Generates the improved shell script."""
    script_filename = "create_snapshot.sh"
    try:
        with open(script_filename, "w") as f:
            f.write(shell_script_content)
        
        # Make the script executable
        os.chmod(script_filename, 0o755)
        
        print(f"✅ Successfully created '{script_filename}'.")
        print("\nThis new script is much smarter:")
        print("  - It aggressively prunes build directories like 'build/' and 'CMakeFiles/'.")
        print("  - It focuses only on source, script, and config files.")
        print("  - The output includes line numbers for better AI context.")
        print("\nTo run it, use the following command in your terminal:")
        print(f"  ./{script_filename}")
        
    except Exception as e:
        print(f"❌ Error creating script: {e}")

if __name__ == "__main__":
    create_snapshot_script()