#!/bin/bash
set -eo pipefail

# Clear the terminal screen at the start of the script
clear

# Determine repository root dynamically so the script works regardless
# of where the project directory is located on disk.
REPO_ROOT="$(cd "$(dirname "$0")" && pwd)"

# Define the output directory and log file path
OUTPUT_DIR="${REPO_ROOT}/output"
LOG_FILE="${OUTPUT_DIR}/build_log.txt"

# Create the output directory if it doesn't exist
mkdir -p "${OUTPUT_DIR}"

# Clean previous output files in the output directory
rm -rf "${OUTPUT_DIR}/*" || true

# Redirect all output to both console and log file
exec 1> >(tee "${LOG_FILE}") 2>&1

echo "==== SEP Engine Build Script with FULL Cycles and PipeWire Support ===="
echo "Setting up build environment with real library paths - NO STUBS!"
echo "Build started at $(date)"

# Source user-provided library paths if available
if [ -f "scripts/local_env_paths.sh" ]; then
  echo "Loading local library paths from scripts/local_env_paths.sh"
  source scripts/local_env_paths.sh
fi

# Ensure proper user directories exist
USER_LOCAL_BIN="$HOME/.local/bin"
USER_LOCAL_LIB="$HOME/.local/lib"
USER_CONFIG_DIR="$HOME/.config/systemd/user"

mkdir -p "$USER_LOCAL_BIN" "$USER_LOCAL_LIB" "$USER_CONFIG_DIR"

# --- Environment Setup ---
BUILD_DIR="${REPO_ROOT}/cmake-make"
COMPILE_COMMANDS="${BUILD_DIR}/compile_commands.json"
SRC_DIR="${REPO_ROOT}"
LIB_DIR="${SRC_DIR}/lib"
CYCLES_ROOT_DIR="${SRC_DIR}/extern/cycles"
# Resolve compilers. Use GCC/G++ 14 if available, otherwise
# fall back to the default versions installed on the system.
C_COMPILER="/usr/bin/gcc-14"
CXX_COMPILER="/usr/bin/g++-14"
if [ ! -x "$C_COMPILER" ]; then
  C_COMPILER="/usr/bin/gcc"
fi
if [ ! -x "$CXX_COMPILER" ]; then
  CXX_COMPILER="/usr/bin/g++"
fi

# Create build directory
mkdir -p "${BUILD_DIR}"

# Create symbolic link for Cycles if it exists in the primary location
if [ ! -L "${CYCLES_ROOT_DIR}" ] && [ -d "${SRC_DIR}/cycles" ]; then
  echo "Found Cycles at ${SRC_DIR}/cycles, creating symlink..."
  mkdir -p "$(dirname "${CYCLES_ROOT_DIR}")"
  ln -sf "${SRC_DIR}/cycles" "${CYCLES_ROOT_DIR}"
fi

if [ ! -d "${CYCLES_ROOT_DIR}" ]; then
  echo "Error: Cycles directory not found at ${SRC_DIR}/cycles or ${CYCLES_ROOT_DIR}."
  exit 1
fi
echo "Using Cycles root: ${CYCLES_ROOT_DIR}"

# --- Dependency Detection ---
# Check for PipeWire using pkg-config (most reliable method)
echo "Checking for PipeWire development headers..."
export PKG_CONFIG_PATH=/usr/lib64/pkgconfig:$PKG_CONFIG_PATH
PIPEWIRE_CMAKE_ARGS=""
if command -v pkg-config >/dev/null && pkg-config --exists libpipewire-0.3; then
  # Rely on pkg-config to locate PipeWire; avoid manually setting unused variables
  echo "PipeWire found via pkg-config"
  # PIPEWIRE_CMAKE_ARGS can remain empty because the CMake scripts handle
  # discovery internally via pkg-config.
else
  echo "PipeWire not found via pkg-config. Audio capture will be disabled."
fi

# --- CMake Configuration ---
# Clean build directory to ensure a fresh state
echo "Cleaning build directory..."

# Create user-owned build directory
USER_BUILD_DIR="$HOME/.cache/sep/build"
mkdir -p "$USER_BUILD_DIR"

# Clean old build files with error handling
if [ -d "$USER_BUILD_DIR" ]; then
    find "$USER_BUILD_DIR" -type f -exec rm -f {} + 2>/dev/null || true
    find "$USER_BUILD_DIR" -type d -empty -delete 2>/dev/null || true
fi

# Copy cmake modules needed by the project
cp -r "${SRC_DIR}/cmake" "$USER_BUILD_DIR"

# Update build directory reference
BUILD_DIR="$USER_BUILD_DIR"

# Setup CMAKE_PREFIX_PATH to help find system libraries
# This is crucial for dependencies like OpenVDB, Alembic, TBB, etc.
CMAKE_PREFIX_PATH="/usr;/usr/local;/opt/local;/opt/homebrew;${LIB_DIR}"

echo "Running CMake configuration..."
cd "${BUILD_DIR}"

cmake -S "${SRC_DIR}" -B "${BUILD_DIR}" \
  -DCMAKE_C_COMPILER=${C_COMPILER} \
  -DCMAKE_CXX_COMPILER=${CXX_COMPILER} \
  -DCMAKE_PREFIX_PATH="${CMAKE_PREFIX_PATH}" \
  -DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
  -DCMAKE_EXE_LINKER_FLAGS="-Wl,--no-as-needed" \
  ${PIPEWIRE_CMAKE_ARGS}

# Link compile_commands.json to the repository root for tool integration
ln -sf "${COMPILE_COMMANDS}" "${REPO_ROOT}/compile_commands.json"

# --- Build and Install ---
echo "Building SEP Engine..."
# Use all available processor cores for a faster build
make -j$(nproc)

echo "Installing SEP Engine binary..."
# Example of systemd service management. Use 'sudo' only if necessary.
if command -v systemctl >/dev/null; then

    # Setup Blender addon symlinks in user directory
    BLENDER_ADDON_DIR="$HOME/.local/blender/5.0/scripts/addons/sep_engine/libs"
    mkdir -p "$BLENDER_ADDON_DIR"
    rm -f "$BLENDER_ADDON_DIR/libsep_blender.so"
    ln -sf "${BUILD_DIR}/lib/libsep_blender.so" "$BLENDER_ADDON_DIR/libsep_blender.so"

    # Install binary to user's local bin
    ln -sf "${BUILD_DIR}/sep_engine" "$USER_LOCAL_BIN/sep_engine"

    # Setup systemd user service with proper permissions
    SERVICE_FILE="$USER_CONFIG_DIR/sep-engine.service"
    cat > "$SERVICE_FILE" << EOF
[Unit]
Description=SEP Engine Service (User)
After=network.target pipewire.service

[Service]
Type=simple
ExecStart=$USER_LOCAL_BIN/sep_engine --server --port 8080 --host 0.0.0.0 --debug
Restart=on-failure
Environment=XDG_RUNTIME_DIR=/run/user/$(id -u)
Environment=HOME=$HOME
Environment=USER=$(whoami)
Environment=LOGNAME=$(whoami)
Environment=PATH=/usr/local/bin:/usr/bin:/bin
WorkingDirectory=$HOME

[Install]
WantedBy=default.target
EOF

    chmod 644 "$SERVICE_FILE"

    # Set proper permissions for binary
    chmod 755 "$USER_LOCAL_BIN/sep_engine"
    
    # Check and setup audio group membership
    if ! getent group audio >/dev/null; then
        echo "Error: 'audio' group does not exist. Audio capture will not work."
        echo "This system may not be properly configured for audio support."
    elif ! groups | grep -q audio; then
        echo "Warning: Current user is not in the 'audio' group. Audio capture may not work."
        echo "To fix this:"
        echo "1. Run: sudo usermod -aG audio $USER"
        echo "2. Log out completely (including all terminals)"
        echo "3. Log back in"
        echo "4. Verify with: groups | grep audio"
        
        # Create audio access file for systemd user service
        cat > "$USER_CONFIG_DIR/sep-engine-audio.conf" << EOF
[Service]
SupplementaryGroups=audio
EOF
        chmod 644 "$USER_CONFIG_DIR/sep-engine-audio.conf"
        
        # Reload systemd user configuration
        systemctl --user daemon-reload
    fi

    # Manage user service
    systemctl --user stop sep-engine || true
    systemctl --user daemon-reload
    systemctl --user enable sep-engine
    systemctl --user start sep-engine
    sleep 2 # Wait for service to start
    systemctl --user status sep-engine --no-pager

    # Start server directly if service fails
    if ! systemctl --user is-active sep-engine >/dev/null; then
        echo "Starting server directly since service failed..."
        "$USER_LOCAL_BIN/sep_engine" --server --port 8080 --host 0.0.0.0 --debug
    fi

else
    echo "systemctl not found. Manual installation required for sep_engine binary."
    echo "Binary is at: ${BUILD_DIR}/sep_engine"
    echo "You can manually copy it to $USER_LOCAL_BIN/sep_engine"
fi

echo "==== Build Complete at $(date) ===="

echo "==== Generating Snapshot from totxt.save ===="
# IMPORTANT: Replace 'totxt.save' with the actual command to run totxt.save
# This example assumes 'totxt.save' is an executable or a function/alias available in your shell.
# If it's a Python script, you might need 'python /path/to/totxt.save.py'
# If it's a binary, you might need '/path/to/totxt.save'
# Replace 'snapshot.txt' with the desired filename for the snapshot.
totxt.save > "${OUTPUT_DIR}/snapshot.txt" 2>&1 || echo "Warning: totxt.save command failed or had output. Check ${OUTPUT_DIR}/snapshot.txt"

echo "==== Build Complete ===="
if [ -f "${BUILD_DIR}/sep_engine" ]; then
    echo "SEP Engine binary is located at: ${BUILD_DIR}/sep_engine"
    echo "Cycles test executable is at: ${BUILD_DIR}/src/tests/cycles_test"
else
    echo "Build failed. Please check the output above for errors."
    exit 1
fi