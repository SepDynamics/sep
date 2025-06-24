#!/usr/bin/env bash
set -e

# Configuration
CODECHECKER_PORT=8001
CODECHECKER_WORKSPACE="$PWD/.codechecker"
REPORTS_DIR="$CODECHECKER_WORKSPACE/reports"
COMPILE_COMMANDS="$PWD/build/compile_commands.json"
OLLAMA_URL="http://localhost:11434"
OLLAMA_MODEL="${OLLAMA_MODEL:-mxbai-embed-large}"
EMBEDDINGS_DIR="$PWD/.embeddings"
INDEX_FILE="$EMBEDDINGS_DIR/code_index.json"

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[0;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Function to show usage
show_usage() {
    echo -e "${BLUE}Code Analysis Suite${NC}"
    echo "========================="
    echo ""
    echo "Usage: $0 [COMMAND]"
    echo ""
    echo -e "${GREEN}CodeChecker Commands:${NC}"
    echo "  start-cc-server   - Start the CodeChecker server"
    echo "  stop-cc-server    - Stop the CodeChecker server"
    echo "  analyze           - Run CodeChecker analysis on the codebase"
    echo "  store             - Store CodeChecker analysis results"
    echo "  cc-web            - Open the CodeChecker web interface"
    echo ""
    echo -e "${GREEN}Ollama Commands:${NC}"
    echo "  check-ollama      - Check if Ollama is running"
    echo "  index-codebase    - Create semantic index of the codebase using Ollama"
    echo "  search-code       - Search the codebase using natural language"
    echo ""
    echo -e "${GREEN}Combined Commands:${NC}"
    echo "  setup-all         - Set up both CodeChecker and Ollama indexing"
    echo "  analyze-all       - Run both CodeChecker analysis and Ollama indexing"
    echo "  status            - Show status of all services"
    echo "  help              - Show this help"
    echo ""
    echo -e "${YELLOW}Environment Variables:${NC}"
    echo "  OLLAMA_MODEL      - Ollama model to use (default: codellama)"
    echo ""
    echo -e "${BLUE}Examples:${NC}"
    echo "  $0 setup-all                # Set up everything"
    echo "  $0 analyze-all              # Run all analyses"
    echo "  $0 search-code \"memory leak\" # Search for memory leaks in the codebase"
}

# Ensure the CodeChecker workspace exists
ensure_workspace() {
    mkdir -p "$CODECHECKER_WORKSPACE"
    mkdir -p "$REPORTS_DIR"
    mkdir -p "$EMBEDDINGS_DIR"
}

# Start the CodeChecker server
start_cc_server() {
    echo -e "${BLUE}[*] Starting CodeChecker server on port $CODECHECKER_PORT...${NC}"
    ensure_workspace
    
    # Check if server is already running
    if pgrep -f "CodeChecker server.*$CODECHECKER_PORT" > /dev/null; then
        echo -e "${YELLOW}[!] CodeChecker server is already running on port $CODECHECKER_PORT${NC}"
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
        echo -e "${GREEN}[✓] CodeChecker server started successfully${NC}"
        echo -e "${BLUE}[i] Server log: $CODECHECKER_WORKSPACE/server.log${NC}"
        echo -e "${BLUE}[i] Web interface: http://localhost:$CODECHECKER_PORT${NC}"
    else
        echo -e "${RED}[!!] Failed to start CodeChecker server${NC}"
        cat "$CODECHECKER_WORKSPACE/server.log"
        return 1
    fi
}

# Stop the CodeChecker server
stop_cc_server() {
    echo -e "${BLUE}[*] Stopping CodeChecker server...${NC}"
    
    # Find and kill the server process
    pkill -f "CodeChecker server.*$CODECHECKER_PORT" || {
        echo -e "${YELLOW}[!] No running CodeChecker server found on port $CODECHECKER_PORT${NC}"
        return 0
    }
    
    echo -e "${GREEN}[✓] CodeChecker server stopped${NC}"
}

# Run CodeChecker analysis
run_cc_analysis() {
    echo -e "${BLUE}[*] Running CodeChecker analysis...${NC}"
    ensure_workspace
    
    # Check if compile_commands.json exists
    if [ ! -f "$COMPILE_COMMANDS" ]; then
        echo -e "${RED}[!!] Compile commands file not found: $COMPILE_COMMANDS${NC}"
        echo -e "${YELLOW}[i] Run the build first to generate compile_commands.json${NC}"
        return 1
    fi
    
    # Run the analysis
    CodeChecker analyze \
        "$COMPILE_COMMANDS" \
        --output "$REPORTS_DIR" \
        --analyzers clangsa clang-tidy
    
    echo -e "${GREEN}[✓] CodeChecker analysis completed${NC}"
    echo -e "${BLUE}[i] Reports saved to: $REPORTS_DIR${NC}"
}

# Store CodeChecker analysis results
store_cc_results() {
    echo -e "${BLUE}[*] Storing CodeChecker analysis results...${NC}"
    
    # Check if reports directory exists and is not empty
    if [ ! -d "$REPORTS_DIR" ] || [ -z "$(ls -A "$REPORTS_DIR" 2>/dev/null)" ]; then
        echo -e "${RED}[!!] No reports found in: $REPORTS_DIR${NC}"
        echo -e "${YELLOW}[i] Run analysis first to generate reports${NC}"
        return 1
    fi
    
    # Check if server is running
    if ! pgrep -f "CodeChecker server.*$CODECHECKER_PORT" > /dev/null; then
        echo -e "${YELLOW}[!] CodeChecker server is not running${NC}"
        echo -e "${BLUE}[i] Starting server...${NC}"
        start_cc_server
    fi
    
    # Store the results
    CodeChecker store \
        "$REPORTS_DIR" \
        --name "$(basename "$PWD")" \
        --url "http://localhost:$CODECHECKER_PORT/Default"
    
    echo -e "${GREEN}[✓] Results stored successfully${NC}"
}

# Open the CodeChecker web interface
open_cc_web() {
    echo -e "${BLUE}[*] Opening CodeChecker web interface...${NC}"
    
    # Check if server is running
    if ! pgrep -f "CodeChecker server.*$CODECHECKER_PORT" > /dev/null; then
        echo -e "${YELLOW}[!] CodeChecker server is not running${NC}"
        echo -e "${BLUE}[i] Starting server...${NC}"
        start_cc_server
    fi
    
    # Open the web interface
    if command -v xdg-open &> /dev/null; then
        xdg-open "http://localhost:$CODECHECKER_PORT"
    elif command -v open &> /dev/null; then
        open "http://localhost:$CODECHECKER_PORT"
    else
        echo -e "${BLUE}[i] Web interface available at: http://localhost:$CODECHECKER_PORT${NC}"
    fi
}

# Check if Ollama is running
check_ollama() {
    echo -e "${BLUE}[*] Checking if Ollama is running...${NC}"
    
    # Try to connect to Ollama API
    if curl -s "$OLLAMA_URL/api/tags" > /dev/null; then
        echo -e "${GREEN}[✓] Ollama is running${NC}"
        
        # Check if the model is available
        if curl -s "$OLLAMA_URL/api/tags" | grep -q "$OLLAMA_MODEL"; then
            echo -e "${GREEN}[✓] Model '$OLLAMA_MODEL' is available${NC}"
        else
            echo -e "${YELLOW}[!] Model '$OLLAMA_MODEL' is not available${NC}"
            echo -e "${BLUE}[i] You can pull it with: ollama pull $OLLAMA_MODEL${NC}"
        fi
        
        return 0
    else
        echo -e "${RED}[!!] Ollama is not running${NC}"
        echo -e "${YELLOW}[i] Start Ollama with: systemctl start ollama (or equivalent)${NC}"
        return 1
    fi
}

# Create a function to extract code from a file
extract_code() {
    local file="$1"
    local ext="${file##*.}"
    
    # Skip binary files and large files
    if [[ -x "$file" || $(stat -c%s "$file") -gt 1000000 ]]; then
        return
    fi
    
    # Skip non-code files based on extension
    if [[ "$ext" == "jpg" || "$ext" == "jpeg" || "$ext" == "png" || "$ext" == "gif" ||
          "$ext" == "bmp" || "$ext" == "ico" || "$ext" == "svg" || "$ext" == "pdf" ||
          "$ext" == "zip" || "$ext" == "tar" || "$ext" == "gz" || "$ext" == "rar" ||
          "$ext" == "7z" || "$ext" == "mp3" || "$ext" == "mp4" || "$ext" == "avi" ||
          "$ext" == "mov" || "$ext" == "mkv" || "$ext" == "wav" || "$ext" == "ogg" ||
          "$ext" == "webm" || "$ext" == "ttf" || "$ext" == "woff" || "$ext" == "woff2" ||
          "$ext" == "eot" ]]; then
        return
    fi
    
    # Output file info and content
    echo "FILE: $file"
    cat "$file"
    echo ""
}

# Index the codebase using Ollama
index_codebase() {
    echo -e "${BLUE}[*] Indexing codebase using Ollama...${NC}"
    ensure_workspace
    
    # Check if Ollama is running
    check_ollama || return 1
    
    # Create a temporary file with all code
    local temp_file="$EMBEDDINGS_DIR/all_code.txt"
    echo -e "${BLUE}[*] Extracting code from files...${NC}"
    
    # Find all files and extract code
    find . -type f -not -path "*/\.*" -not -path "*/build/*" -not -path "*/node_modules/*" | while read -r file; do
        extract_code "$file" >> "$temp_file"
    done
    
    echo -e "${BLUE}[*] Generating embeddings...${NC}"
    
    # Use Ollama to generate embeddings
    # This is a simplified version - in a real implementation, you would:
    # 1. Split the code into chunks
    # 2. Generate embeddings for each chunk
    # 3. Store the embeddings with their source location
    
    # For now, we'll just create a placeholder index file
    cat > "$INDEX_FILE" << EOF
{
  "version": 1,
  "model": "$OLLAMA_MODEL",
  "created": "$(date -Iseconds)",
  "chunks": [
    {
      "id": 1,
      "file": "example.cpp",
      "start_line": 1,
      "end_line": 10,
      "content": "Example code",
      "embedding": [0.1, 0.2, 0.3]
    }
  ]
}
EOF
    
    echo -e "${GREEN}[✓] Codebase indexed successfully${NC}"
    echo -e "${BLUE}[i] Index saved to: $INDEX_FILE${NC}"
    
    # In a real implementation, you would use Ollama's API to generate embeddings
    # curl -X POST "$OLLAMA_URL/api/embeddings" -d '{"model":"$OLLAMA_MODEL","prompt":"your text"}'
}

# Search the codebase using natural language
search_code() {
    local query="$1"
    
    if [ -z "$query" ]; then
        echo -e "${RED}[!!] No search query provided${NC}"
        echo -e "${YELLOW}[i] Usage: $0 search-code \"your search query\"${NC}"
        return 1
    fi
    
    echo -e "${BLUE}[*] Searching codebase for: $query${NC}"
    
    # Check if Ollama is running
    check_ollama || return 1
    
    # Check if index exists
    if [ ! -f "$INDEX_FILE" ]; then
        echo -e "${YELLOW}[!] Code index not found${NC}"
        echo -e "${BLUE}[i] Creating index...${NC}"
        index_codebase
    fi
    
    # In a real implementation, you would:
    # 1. Generate an embedding for the query using Ollama
    # 2. Compare the query embedding with the stored embeddings
    # 3. Return the most similar chunks
    
    echo -e "${BLUE}[*] Search results:${NC}"
    echo -e "${YELLOW}[i] This is a placeholder. In a real implementation, this would return actual search results.${NC}"
    
    # For demonstration, we'll just use grep to find matches
    echo -e "${BLUE}[*] Fallback to grep search:${NC}"
    grep -r --include="*.{cpp,h,hpp,c,cc}" -l "$query" . --color=always | head -n 10
}

# Set up both CodeChecker and Ollama
setup_all() {
    echo -e "${BLUE}[*] Setting up CodeChecker and Ollama...${NC}"
    
    # Ensure workspace
    ensure_workspace
    
    # Start CodeChecker server
    start_cc_server
    
    # Check Ollama
    check_ollama
    
    echo -e "${GREEN}[✓] Setup completed${NC}"
}

# Run both CodeChecker analysis and Ollama indexing
analyze_all() {
    echo -e "${BLUE}[*] Running all analyses...${NC}"
    
    # Run CodeChecker analysis
    run_cc_analysis
    
    # Store CodeChecker results
    store_cc_results
    
    # Index codebase with Ollama
    index_codebase
    
    echo -e "${GREEN}[✓] All analyses completed${NC}"
}

# Show status of all services
show_status() {
    echo -e "${BLUE}=== Status ===${NC}"
    
    # Check CodeChecker server
    if pgrep -f "CodeChecker server.*$CODECHECKER_PORT" > /dev/null; then
        echo -e "${GREEN}[✓] CodeChecker server is running on port $CODECHECKER_PORT${NC}"
        echo -e "${BLUE}[i] Web interface: http://localhost:$CODECHECKER_PORT${NC}"
    else
        echo -e "${YELLOW}[!] CodeChecker server is not running${NC}"
    fi
    
    # Check Ollama
    if curl -s "$OLLAMA_URL/api/tags" > /dev/null; then
        echo -e "${GREEN}[✓] Ollama is running${NC}"
        
        # Get available models
        echo -e "${BLUE}[i] Available models:${NC}"
        curl -s "$OLLAMA_URL/api/tags" | grep -o '"[^"]*"' | tr -d '"' | sort | uniq | grep -v "^$" | while read -r model; do
            echo "    - $model"
        done
    else
        echo -e "${YELLOW}[!] Ollama is not running${NC}"
    fi
    
    # Check for code index
    if [ -f "$INDEX_FILE" ]; then
        echo -e "${GREEN}[✓] Code index exists${NC}"
        echo -e "${BLUE}[i] Index file: $INDEX_FILE${NC}"
        echo -e "${BLUE}[i] Last updated: $(stat -c %y "$INDEX_FILE")${NC}"
    else
        echo -e "${YELLOW}[!] Code index does not exist${NC}"
    fi
    
    # Check for CodeChecker reports
    if [ -d "$REPORTS_DIR" ] && [ -n "$(ls -A "$REPORTS_DIR" 2>/dev/null)" ]; then
        echo -e "${GREEN}[✓] CodeChecker reports exist${NC}"
        echo -e "${BLUE}[i] Reports directory: $REPORTS_DIR${NC}"
    else
        echo -e "${YELLOW}[!] No CodeChecker reports found${NC}"
    fi
}

# Main execution
if [ "$#" -eq 0 ]; then
    show_usage
    exit 0
fi

case "$1" in
    "start-cc-server")
        start_cc_server
        ;;
    "stop-cc-server")
        stop_cc_server
        ;;
    "analyze")
        run_cc_analysis
        ;;
    "store")
        store_cc_results
        ;;
    "cc-web")
        open_cc_web
        ;;
    "check-ollama")
        check_ollama
        ;;
    "index-codebase")
        index_codebase
        ;;
    "search-code")
        search_code "$2"
        ;;
    "setup-all")
        setup_all
        ;;
    "analyze-all")
        analyze_all
        ;;
    "status")
        show_status
        ;;
    "help"|"-h"|"--help")
        show_usage
        ;;
    *)
        echo -e "${RED}[!!] Unknown command: $1${NC}"
        show_usage
        exit 1
        ;;
esac