#!/usr/bin/env bash

# Script to handle Vite server restarts and port conflicts
# Usage: ./restart-vite.sh [instance-id]

# Default instance ID
INSTANCE_ID=${1:-"blender-kilocode"}
# Convert to uppercase using tr command (more compatible)
ENV_VAR_NAME="UI_SERVER_PORT_$(echo "$INSTANCE_ID" | tr '[:lower:]' '[:upper:]')"
ENV_VAR_NAME=$(echo "$ENV_VAR_NAME" | tr '-' '_')

echo "Managing Vite server for instance: $INSTANCE_ID"
echo "Looking for environment variable: $ENV_VAR_NAME"

# Check if any Vite processes are running
VITE_PIDS=$(pgrep -f vite)
if [ -n "$VITE_PIDS" ]; then
  echo "Found running Vite processes, killing them..."
  pkill -f vite
  sleep 2
fi

# Check if .env.local exists
if [ -f "/sep/.env.local" ]; then
  # Check if the instance-specific port is defined
  if grep -q "$ENV_VAR_NAME=" "/sep/.env.local"; then
    echo "Found instance-specific port configuration in .env.local"
  else
    # If not, suggest adding it
    echo "No instance-specific port found in .env.local"
    PORT_BASE=5175
    # Find a free port
    PORT=$(($PORT_BASE + $RANDOM % 100))
    while netstat -tuln | grep -q ":$PORT "; do
      PORT=$(($PORT_BASE + $RANDOM % 100))
    done
    echo "Suggesting to add: $ENV_VAR_NAME=$PORT to .env.local"
    echo "You can do this by running:"
    echo "echo \"$ENV_VAR_NAME=$PORT\" >> /sep/.env.local"
  fi
else
  echo "No .env.local file found. Creating one with basic configuration..."
  cat > /sep/.env.local << EOL
# Default port assignments for Vite servers to prevent conflicts
VITE_PORT=5173
MCP_SERVER_PORT=5174
UI_SERVER_PORT=5175
DOCS_SERVER_PORT=5176
API_DOCS_PORT=5177

# Instance-specific port assignments for webview-ui instances
UI_SERVER_PORT_BLENDER_KILOCODE=5175
EOL
  echo "Created .env.local with default configuration"
fi

# Identify the Vite config path based on the instance
VITE_CONFIG_PATH=""
case "$INSTANCE_ID" in
  "blender-kilocode")
    VITE_CONFIG_PATH="/sep/extern/blender/kilocode/webview-ui"
    ;;
  # Add more cases as needed for other instances
  *)
    echo "Unknown instance ID: $INSTANCE_ID"
    echo "Searching for matching webview-ui directories..."
    POSSIBLE_PATHS=$(find /sep -path "*/$INSTANCE_ID*/webview-ui" -type d)
    if [ -n "$POSSIBLE_PATHS" ]; then
      VITE_CONFIG_PATH=$(echo "$POSSIBLE_PATHS" | head -n 1)
      echo "Found potential config path: $VITE_CONFIG_PATH"
    else
      echo "No matching directories found."
      exit 1
    fi
    ;;
esac

if [ -d "$VITE_CONFIG_PATH" ]; then
  echo "Starting Vite server from: $VITE_CONFIG_PATH"
  cd "$VITE_CONFIG_PATH" && INSTANCE_ID="$INSTANCE_ID" npm run dev
else
  echo "Error: Could not find Vite configuration directory at $VITE_CONFIG_PATH"
  exit 1
fi