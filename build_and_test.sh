#!/bin/bash
cd /sep

# --- Configuration ---
IMAGE_NAME="sep-engine-builder"
LOG_FILE="output/build_log.txt"

# --- Cleanup and Final Reporting Function ---
# (Existing cleanup_and_report function remains the same, but will now parse new logs)
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

    # --- Section: Build Status & Test Results ---
    if [ "$EXIT_CODE" -ne 0 ]; then
        echo -e "\033[1;31mBUILD OR TEST FAILED. Condensing errors by file:\033[0m"
        echo "--------------------------------------------------"

        FAILED_FILES=$(grep -E ':[0-9]+:[0-9]+: (error|warning):' "$LOG_FILE" | awk -F':' '{print $1}' | sort -u)

        if [ -z "$FAILED_FILES" ]; then
            echo "Could not automatically extract failed files. Showing raw log summary:"
            grep -i -E "error|fail|warning|FAILED|compilation terminated" "$LOG_FILE" || true
        else
            for FILE in $FAILED_FILES; do
                FILENAME=$(basename "$FILE")
                echo
                echo -e "\033[1;36m>>> Errors/Warnings for: $FILENAME \033[0;90m($FILE)\033[0m"
                echo
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
        # Show CTest summary
        grep "Running tests" "$LOG_FILE" | tail -n 1
        grep "100% tests passed" "$LOG_FILE" || echo "Tests passed (check log for details)."
        
        echo -e "\n\033[1;34m--- Financial Pipeline & Benchmarks Status ---\033[0m"
        if [ -f "output/metrics_output.json" ]; then
            echo -e "\033[1;32m✓ SEP Engine metrics generated to output/metrics_output.json\033[0m"
        else
            echo -e "\033[1;31m✗ Failed to generate SEP Engine metrics.\033[0m"
        fi
        
        if [ -f "output/alpha_results.json" ]; then
            echo -e "\033[1;32m✓ Alpha prediction results available in output/alpha_results.json\033[0m"
            ALPHA_RESULT=$(grep "total_return_alpha" output/alpha_results.json | awk '{print $2}' | tr -d ',')
            if [ -n "$ALPHA_RESULT" ]; then
                 echo -e "\033[1;32m  Predicted Alpha: $ALPHA_RESULT\033[0m"
            fi
        else
            echo -e "\033[1;31m✗ Alpha prediction pipeline did not run or failed.\033[0m"
        fi

        echo -e "\n\033[1;34m--- Benchmark Results ---\033[0m"
        grep "BM_ProcessFile/CPU" "$LOG_FILE" | tail -n 1
        grep "BM_ProcessFile/GPU" "$LOG_FILE" | tail -n 1
        echo -e "\033[1;34m--------------------------------------------------\033[0m\n"
    fi

    exit $EXIT_CODE
}

# --- Main Script ---

trap cleanup_and_report EXIT
set -e

if ! command -v totxt &> /dev/null; then
    echo "Error: 'totxt' command not found. Please ensure it is in your PATH." >&2
    exit 1
fi

mkdir -p output
totxt.save
> "$LOG_FILE"

echo "Build and test process started. Full log is in $LOG_FILE"

echo "Building Docker image: $IMAGE_NAME..."
docker build -t $IMAGE_NAME . &> "$LOG_FILE"

echo "Running build and test inside Docker container..."
docker run --gpus all --rm $IMAGE_NAME /bin/bash -c "
    set -e
    echo '--- Cleaning up previous build artifacts... ---'
    rm -rf build && mkdir build && cd build
    
    echo '--- Configuring project with CMake... ---'
    cmake .. -G Ninja \
        -DCMAKE_BUILD_TYPE=Release \
        -DCMAKE_C_COMPILER=clang-15 \
        -DCMAKE_CXX_COMPILER=clang++-15 \
        -DSEP_USE_CUDA=ON

    echo '--- Building project with Ninja... ---'
    ninja

    echo '--- Running unit tests with CTest... ---'
    ctest --output-on-failure --test-dir ./tests

    echo '--- Running benchmarks... ---'
    # Ensure the benchmark executable is built and run directly or via ctest if labeled
    ./examples/pattern_metric_example --benchmark &>> /dev/null || true # Run benchmarks but suppress their verbose output from pipe

    echo '--- Processing 280MB data for financial pipeline (JSON output)... ---'
    # Assuming OANDA_full_history.json is in assets/test_data/
    ./examples/pattern_metric_example assets/test_data/OANDA_full_history.json --json --output output/metrics_output.json

    echo '--- Executing Alpha Prediction Pipeline... ---'
    # Assuming run_alpha_experiment.py is in a 'scripts' directory relative to /sep
    python3 scripts/run_alpha_experiment.py --input output/metrics_output.json --output output/alpha_results.json

" &>> "$LOG_FILE"    