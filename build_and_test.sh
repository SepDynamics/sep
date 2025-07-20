#!/bin/bash
cd /sep

# --- Configuration ---
IMAGE_NAME="sep-engine-builder"
LOG_FILE="output/build_log.txt"

# --- Cleanup and Final Reporting Function ---
cleanup_and_report() {
    EXIT_CODE=$? # Capture the exit code of the last command
    echo
    echo "--------------------------------------------------"
    echo "Build & Test Finished. Scanning Log for Issues..."
    echo "--------------------------------------------------"

    # --- Section: Server/API & Important Runtime Messages ---
    echo -e "\n\033[1;34m--- Server/API & Important Runtime Messages ---\033[0m"
    SERVER_MESSAGES=$(grep -E 'Starting SEP API Server|API routes configured|Request: .* - Status:|API Error|Failed to start SEP API Server|Listening for connections|Crow error' "$LOG_FILE" | uniq)

    if [ -n "$SERVER_MESSAGES" ]; then
        echo "$SERVER_MESSAGES" \
            | sed -E 's/(API Error)/⚡ \o033[1;31m\1\o033[0m/' \
            | sed -E 's/(Starting SEP API Server)/\o033[1;32m\1\o033[0m/' \
            | sed -E 's/(API routes configured)/\o033[1;32m\1\o033[0m/' \
            | sed -E 's/(Request: .* - Status: 2[0-9]{2})/\o033[0;37m\1\o033[0m/' \
            | sed -E 's/(Request: .* - Status: 4[0-9]{2}|5[0-9]{2})/\o033[0;33m\1\o033[0m/' \
            | sed -E 's/(Failed to start SEP API Server)/\o033[1;31m\1\o033[0m/' \
            | sed -E 's/(Crow error)/\o033[1;31m\1\o033[0m/'
    else
        echo "No significant server/API messages found."
    fi
    echo -e "\033[1;34m--------------------------------------------------\033[0m\n"


    # --- Section: Compiler Errors & Warnings (Condensing by file) ---
    if [ "$EXIT_CODE" -ne 0 ]; then
        echo "BUILD FAILED. Condensing errors by file:"
        echo "--------------------------------------------------"

        # Find all unique files that have compiler errors or warnings.
        # This works by finding the standard "file:line:col: type: message" pattern.
        FAILED_FILES=$(grep -E ':[0-9]+:[0-9]+: (error|warning):' "$LOG_FILE" | awk -F':' '{print $1}' | sort -u)

        if [ -z "$FAILED_FILES" ]; then
            echo "Could not automatically extract failed files. Showing raw log summary:"
            # Fallback to a more general search for build failures if smart parsing finds nothing
            grep -i -E "error|fail|warning|FAILED|compilation terminated" "$LOG_FILE" || true
        else
            for FILE in $FAILED_FILES; do
                # Use a more readable basename for the header
                FILENAME=$(basename "$FILE")
                echo
                echo -e "\033[1;36m>>> Errors/Warnings for: $FILENAME \033[0;90m($FILE)\033[0m"
                echo

                # Grep for all lines related to this file, then pretty-print them.
                # 1. grep: Find all error/warning lines for the specific file.
                # 2. sed: Remove the long, repetitive file path from the start of each line for readability.
                # 3. sed: Indent each line for clarity.
                # 4. sed: Add colorization for 'error' (red) and 'warning' (yellow).
                grep -E "^${FILE}:[0-9]+:[0-9]+: (error|warning):" "$LOG_FILE" \
                    | sed -E "s,^${FILE}:,,g" \
                    | sed 's/^/  /' \
                    | sed -E 's/(error:)/\o033[1;31m\1\o033[0m/' \
                    | sed -E 's/(warning:)/\o033[1;33m\1\o033[0m/'
                echo "--------------------------------------------------"
            done
        fi

        echo
        echo -e "\033[1;31mSCRIPT FAILED with exit code $EXIT_CODE.\033[0m"
        echo "Full log is available in: $LOG_FILE"
        echo "--------------------------------------------------"
    else
        echo -e "\033[1;32mScript completed successfully.\033[0m"
        echo "--------------------------------------------------"
        # Show a brief success summary from the log
        grep "Running tests" "$LOG_FILE" | tail -n 1
        grep "100% tests passed" "$LOG_FILE" || echo "Tests passed (check log for details)."
    fi

    # Exit with the original exit code
    exit $EXIT_CODE
}

# --- Main Script ---

# Set a trap to call cleanup_and_report on EXIT, ensuring it always runs.
trap cleanup_and_report EXIT

# Exit immediately if any command fails.
set -e

# Ensure the snapshot utility is available
if ! command -v totxt &> /dev/null; then
    echo "Error: 'totxt' command not found. Please ensure it is in your PATH." >&2
    exit 1
fi

# Create output directory and clear the log file
mkdir -p output
totxt.save
> "$LOG_FILE"

echo "Build and test process started. Full log is in $LOG_FILE"

# Build the Docker image, redirecting all output to the log file
echo "Building Docker image: $IMAGE_NAME..."
docker build -t $IMAGE_NAME . &> "$LOG_FILE"

# Run the build and test process inside the container, appending output to the log file
echo "Running build and test inside Docker container..."
docker run --rm $IMAGE_NAME /bin/bash -c "
    set -e
    echo '--- Cleaning up previous build artifacts... ---'
    rm -rf build && mkdir build && cd build
    
    echo '--- Configuring project with CMake... ---'
    cmake .. -G Ninja \
        -DCMAKE_BUILD_TYPE=Debug \
        -DCMAKE_C_COMPILER=clang-15 \
        -DCMAKE_CXX_COMPILER=clang++-15 \
        -DSEP_USE_CUDA=ON

    echo '--- Building project with Ninja... ---'
    ninja

    echo '--- Running tests with CTest... ---'
    ctest --output-on-failure
" &>> "$LOG_FILE"