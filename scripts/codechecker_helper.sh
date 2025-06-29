#!/usr/bin/env bash
set -e

# Configuration
CODECHECKER_PORT=8001
CODECHECKER_WORKSPACE="$PWD/.codechecker"
REPORTS_DIR="$CODECHECKER_WORKSPACE/reports"
COMPILE_COMMANDS="$PWD/cmake-make/compile_commands.json"  # Using $PWD is appropriate in shell scripts

# Function to show usage
show_usage() {
    echo "CodeChecker Helper Script"
    echo "========================="
    echo ""
    echo "Usage: $0 [COMMAND]"
    echo ""
    echo "Commands:"
    echo "  start-server   - Start the CodeChecker server"
    echo "  stop-server    - Stop the CodeChecker server"
    echo "  analyze        - Run analysis on the codebase"
    echo "  store          - Store analysis results"
    echo "  web            - Open the CodeChecker web interface"
    echo "  help           - Show this help"
    echo ""
    echo "Examples:"
    echo "  $0 start-server    # Start the CodeChecker server"
    echo "  $0 analyze         # Run analysis"
    echo "  $0 store           # Store results"
}

# Ensure the CodeChecker workspace exists
ensure_workspace() {
    mkdir -p "$CODECHECKER_WORKSPACE"
    mkdir -p "$REPORTS_DIR"
}

# Start the CodeChecker server
start_server() {
    echo "[*] Starting CodeChecker server on port $CODECHECKER_PORT..."
    ensure_workspace
    
    # Check if server is already running
    if pgrep -f "CodeChecker server.*$CODECHECKER_PORT" > /dev/null; then
        echo "[!] CodeChecker server is already running on port $CODECHECKER_PORT"
        return 0
    fi
    
    # Start the server
    CodeChecker server \
        --workspace "$CODECHECKER_WORKSPACE" \
        --port "$CODECHECKER_PORT" \
        --not-host-only \
        &> "$CODECHECKER_WORKSPACE/server.log" &
    
    # Wait for server to start
    sleep 2
    if pgrep -f "CodeChecker server.*$CODECHECKER_PORT" > /dev/null; then
        echo "[✓] CodeChecker server started successfully"
        echo "[i] Server log: $CODECHECKER_WORKSPACE/server.log"
        echo "[i] Web interface: http://localhost:$CODECHECKER_PORT"
    else
        echo "[!!] Failed to start CodeChecker server"
        cat "$CODECHECKER_WORKSPACE/server.log"
        return 1
    fi
}

# Stop the CodeChecker server
stop_server() {
    echo "[*] Stopping CodeChecker server..."
    
    # Find and kill the server process
    pkill -f "CodeChecker server.*$CODECHECKER_PORT" || {
        echo "[!] No running CodeChecker server found on port $CODECHECKER_PORT"
        return 0
    }
    
    echo "[✓] CodeChecker server stopped"
}

# Run analysis
run_analysis() {
    echo "[*] Running CodeChecker analysis..."
    ensure_workspace
    
    # Check if compile_commands.json exists
    if [ ! -f "$COMPILE_COMMANDS" ]; then
        echo "[!!] Compile commands file not found: $COMPILE_COMMANDS"
        echo "[i] Run the build first to generate compile_commands.json"
        return 1
    fi
    
    # Run the analysis
    CodeChecker analyze \
        "$COMPILE_COMMANDS" \
        --output "$REPORTS_DIR" \
        --analyzers clangsa clang-tidy
    
    echo "[✓] Analysis completed"
    echo "[i] Reports saved to: $REPORTS_DIR"
}

# Store analysis results
store_results() {
    echo "[*] Storing analysis results..."
    
    # Check if reports directory exists and is not empty
    if [ ! -d "$REPORTS_DIR" ] || [ -z "$(ls -A "$REPORTS_DIR" 2>/dev/null)" ]; then
        echo "[!!] No reports found in: $REPORTS_DIR"
        echo "[i] Run analysis first to generate reports"
        return 1
    fi
    
    # Check if server is running
    if ! pgrep -f "CodeChecker server.*$CODECHECKER_PORT" > /dev/null; then
        echo "[!] CodeChecker server is not running"
        echo "[i] Starting server..."
        start_server
    fi
    
    # Store the results
    CodeChecker store \
        "$REPORTS_DIR" \
        --name "$(basename "$PWD")" \
        --url "http://localhost:$CODECHECKER_PORT/Default"
    
    echo "[✓] Results stored successfully"
}

# Open the web interface
open_web() {
    echo "[*] Opening CodeChecker web interface..."
    
    # Check if server is running
    if ! pgrep -f "CodeChecker server.*$CODECHECKER_PORT" > /dev/null; then
        echo "[!] CodeChecker server is not running"
        echo "[i] Starting server..."
        start_server
    fi
    
    # Open the web interface
    if command -v xdg-open &> /dev/null; then
        xdg-open "http://localhost:$CODECHECKER_PORT"
    elif command -v open &> /dev/null; then
        open "http://localhost:$CODECHECKER_PORT"
    else
        echo "[i] Web interface available at: http://localhost:$CODECHECKER_PORT"
    fi
}

# Main execution
if [ "$#" -eq 0 ]; then
    show_usage
    exit 0
fi

case "$1" in
    "start-server")
        start_server
        ;;
    "stop-server")
        stop_server
        ;;
    "analyze")
        run_analysis
        ;;
    "store")
        store_results
        ;;
    "web")
        open_web
        ;;
    "help"|"-h"|"--help")
        show_usage
        ;;
    *)
        echo "[!!] Unknown command: $1"
        show_usage
        exit 1
        ;;
esac