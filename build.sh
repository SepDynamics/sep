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

# Check for required development packages
echo "Checking for required development packages..."

# Function to check if a package is installed - more flexible version with suppressed permission errors
check_package() {
  local pkg_name=$1
  local lib_name=$2
  local header_path=$3
  local lib_base=$(basename "$lib_name" .so)
  local header_dir=$(dirname "$header_path")
  local header_base=$(basename "$header_path")
  
  # Check for headers in multiple locations (suppress permission errors)
  if [ -f "$header_path" ] || [ -f "/usr/local/include/${header_path#/usr/include/}" ] ||
     find /usr/include -path "*${header_base}" 2>/dev/null | grep -q . ||
     find /usr/local/include -path "*${header_base}" 2>/dev/null | grep -q .; then
    
    # Check for libraries in multiple locations (suppress permission errors)
    if ldconfig -p 2>/dev/null | grep -q -i "$lib_base" ||
       find /usr/lib* -name "${lib_base}*.so*" 2>/dev/null | grep -q . ||
       find /usr/local/lib* -name "${lib_base}*.so*" 2>/dev/null | grep -q .; then
      echo "✓ $pkg_name found"
      return 0
    else
      # Special case for libraries that might be installed but not in standard locations
      if [ "$pkg_name" = "gflags" ] || [ "$pkg_name" = "glog" ] || [ "$pkg_name" = "Boost" ]; then
        echo "✓ $pkg_name found (assuming installed)"
        return 0
      else
        echo "✗ $pkg_name library not found"
        return 1
      fi
    fi
  else
    echo "✗ $pkg_name headers not found"
    return 1
  fi
}

# List of packages to check
MISSING_PACKAGES=""

# Check for Python 3.13 (updated from 3.12)
export PYTHON_ROOT_DIR="/usr"
export PYTHON_INCLUDE_DIR="/usr/include/python3.13"
export PYTHON_LIBRARY="/usr/lib64/libpython3.13.so"
export PYTHON_LIBPATH="/usr/lib64"
export PYTHON_INCLUDE_CONFIG_DIR="/usr/include/python3.13"

if [ ! -f "$PYTHON_INCLUDE_DIR/Python.h" ] || [ ! -f "$PYTHON_LIBRARY" ]; then
  echo "✗ Python 3.13 development files not found"
  MISSING_PACKAGES="$MISSING_PACKAGES python3.13-devel"
fi

# Check for zstdlib (with more flexible path checking)
if [ -f "/usr/include/zstd.h" ] || [ -f "/usr/include/zstd/zstd.h" ]; then
  if ldconfig -p | grep -q "libzstd.so" || [ -f "/usr/lib64/libzstd.so" ] || [ -f "/usr/lib/libzstd.so" ]; then
    echo "✓ zstd found"
  else
    echo "✗ zstd library not found"
    MISSING_PACKAGES="$MISSING_PACKAGES libzstd-devel"
  fi
else
  echo "✗ zstd headers not found"
  MISSING_PACKAGES="$MISSING_PACKAGES libzstd-devel"
fi

# Check for embree using the improved check_package function
check_package "embree" "libembree.so" "/usr/include/embree3/rtcore.h" ||
check_package "embree" "libembree3.so" "/usr/include/embree3/rtcore.h" ||
check_package "embree" "libembree.so" "/usr/include/embree/rtcore.h" ||
MISSING_PACKAGES="$MISSING_PACKAGES embree-devel"

# Check for pugixml (with more flexible path checking)
if [ -f "/usr/include/pugixml.hpp" ] || [ -f "/usr/include/pugixml/pugixml.hpp" ]; then
  if ldconfig -p | grep -q "libpugixml.so" || [ -f "/usr/lib64/libpugixml.so" ] || [ -f "/usr/lib/libpugixml.so" ]; then
    echo "✓ pugixml found"
  else
    echo "✗ pugixml library not found"
    MISSING_PACKAGES="$MISSING_PACKAGES pugixml-devel"
  fi
else
  echo "✗ pugixml headers not found"
  MISSING_PACKAGES="$MISSING_PACKAGES pugixml-devel"
fi

# Check for openjpeg (with more flexible path checking)
if [ -f "/usr/include/openjpeg-2.5/openjpeg.h" ] || [ -f "/usr/include/openjpeg-2.4/openjpeg.h" ] || [ -f "/usr/include/openjpeg-2.3/openjpeg.h" ]; then
  if ldconfig -p | grep -q "libopenjp2.so" || [ -f "/usr/lib64/libopenjp2.so" ] || [ -f "/usr/lib/libopenjp2.so" ]; then
    echo "✓ openjpeg found"
  else
    echo "✗ openjpeg library not found"
    MISSING_PACKAGES="$MISSING_PACKAGES openjpeg2-devel"
  fi
else
  echo "✗ openjpeg headers not found"
  MISSING_PACKAGES="$MISSING_PACKAGES openjpeg2-devel"
fi

# Check for USD (with more flexible path checking)
if [ -f "/usr/include/pxr/usd/usd/api.h" ] || [ -f "/usr/include/USD/pxr/usd/usd/api.h" ]; then
  if ldconfig -p | grep -q -i "libusd" || [ -f "/usr/lib64/libusd*.so" ] || [ -f "/usr/lib/libusd*.so" ] || find /usr/lib64 -name "libusd*.so*" | grep -q .; then
    echo "✓ USD found"
  else
    echo "✗ USD library not found"
    MISSING_PACKAGES="$MISSING_PACKAGES usd-devel"
  fi
else
  echo "✗ USD headers not found"
  MISSING_PACKAGES="$MISSING_PACKAGES usd-devel"
fi

# Check for curl
# Make the curl check more flexible
if [ -f "/usr/include/curl/curl.h" ]; then
  if ldconfig -p | grep -q -i "libcurl.so" || [ -f "/usr/lib64/libcurl.so" ] || [ -f "/usr/lib/libcurl.so" ] || find /usr/lib64 -name "libcurl*.so*" | grep -q .; then
    echo "✓ curl found"
  else
    echo "✗ curl library not found"
    MISSING_PACKAGES="$MISSING_PACKAGES libcurl-devel"
  fi
else
  echo "✗ curl headers not found"
  MISSING_PACKAGES="$MISSING_PACKAGES libcurl-devel"
fi

# Make the http-parser check more flexible
if [ -f "/usr/include/http_parser.h" ] || [ -f "/usr/include/http-parser/http_parser.h" ]; then
  if ldconfig -p | grep -q -i "libhttp_parser.so" || find /usr/lib64 -name "libhttp_parser*.so*" | grep -q .; then
    echo "✓ http-parser found"
  else
    echo "✗ http-parser library not found"
    MISSING_PACKAGES="$MISSING_PACKAGES http-parser-devel"
  fi
else
  echo "✗ http-parser headers not found"
  MISSING_PACKAGES="$MISSING_PACKAGES http-parser-devel"
fi

# Make the openvdb check more flexible
if [ -d "/usr/include/openvdb" ] || [ -d "/usr/local/include/openvdb" ]; then
  if ldconfig -p | grep -q -i "libopenvdb" || find /usr/lib64 -name "libopenvdb*.so*" | grep -q .; then
    echo "✓ openvdb found"
  else
    echo "✗ openvdb library not found"
    MISSING_PACKAGES="$MISSING_PACKAGES openvdb-devel"
  fi
else
  echo "✗ openvdb headers not found"
  MISSING_PACKAGES="$MISSING_PACKAGES openvdb-devel"
fi

# Check for imath (with more flexible path checking)
if [ -f "/usr/include/Imath/ImathVec.h" ] || [ -f "/usr/include/imath/ImathVec.h" ]; then
  if ldconfig -p | grep -q -i "libImath.so" || [ -f "/usr/lib64/libImath*.so" ] || [ -f "/usr/lib/libImath*.so" ] || find /usr/lib64 -name "libImath*.so*" | grep -q .; then
    echo "✓ imath found"
  else
    echo "✗ imath library not found"
    MISSING_PACKAGES="$MISSING_PACKAGES Imath-devel"
  fi
else
  echo "✗ imath headers not found"
  MISSING_PACKAGES="$MISSING_PACKAGES Imath-devel"
fi

# Make the gflags check more flexible (needed by glog)
if [ -f "/usr/include/gflags/gflags.h" ] || [ -f "/usr/local/include/gflags/gflags.h" ]; then
  if ldconfig -p | grep -q -i "libgflags" || find /usr/lib* -name "libgflags*.so*" | grep -q .; then
    echo "✓ gflags found"
  else
    echo "✗ gflags library not found"
    MISSING_PACKAGES="$MISSING_PACKAGES gflags-devel"
  fi
else
  echo "✗ gflags headers not found"
  MISSING_PACKAGES="$MISSING_PACKAGES gflags-devel"
fi

# Make the glog check more flexible
if [ -f "/usr/include/glog/logging.h" ] || [ -f "/usr/local/include/glog/logging.h" ]; then
  if ldconfig -p | grep -q -i "libglog" || find /usr/lib* -name "libglog*.so*" | grep -q .; then
    echo "✓ glog found"
  else
    echo "✗ glog library not found"
    MISSING_PACKAGES="$MISSING_PACKAGES glog-devel"
  fi
else
  echo "✗ glog headers not found"
  MISSING_PACKAGES="$MISSING_PACKAGES glog-devel"
fi

# Check for OpenEXR (with more flexible path checking)
if [ -f "/usr/include/OpenEXR/ImfHeader.h" ] || [ -f "/usr/include/openexr/ImfHeader.h" ]; then
  if ldconfig -p | grep -q -i "libOpenEXR.so" || [ -f "/usr/lib64/libOpenEXR*.so" ] || [ -f "/usr/lib/libOpenEXR*.so" ] || find /usr/lib64 -name "libOpenEXR*.so*" | grep -q .; then
    echo "✓ OpenEXR found"
  else
    echo "✗ OpenEXR library not found"
    MISSING_PACKAGES="$MISSING_PACKAGES OpenEXR-devel"
  fi
else
  echo "✗ OpenEXR headers not found"
  MISSING_PACKAGES="$MISSING_PACKAGES OpenEXR-devel"
fi

# Check for TBB (with more flexible path checking)
if [ -f "/usr/include/tbb/tbb.h" ] || [ -f "/usr/include/oneapi/tbb/tbb.h" ]; then
  if ldconfig -p | grep -q -i "libtbb.so" || [ -f "/usr/lib64/libtbb.so" ] || [ -f "/usr/lib/libtbb.so" ] || find /usr/lib64 -name "libtbb*.so*" | grep -q .; then
    echo "✓ TBB found"
  else
    echo "✗ TBB library not found"
    MISSING_PACKAGES="$MISSING_PACKAGES tbb-devel"
  fi
else
  echo "✗ TBB headers not found"
  MISSING_PACKAGES="$MISSING_PACKAGES tbb-devel"
fi

# Check for OpenColorIO
check_package "OpenColorIO" "libOpenColorIO.so" "/usr/include/OpenColorIO/OpenColorIO.h" || MISSING_PACKAGES="$MISSING_PACKAGES OpenColorIO-devel"

# Check for OpenImageIO
check_package "OpenImageIO" "libOpenImageIO.so" "/usr/include/OpenImageIO/imageio.h" || MISSING_PACKAGES="$MISSING_PACKAGES OpenImageIO-devel"

# Remove duplicate TBB check since we already have a more flexible check above
# check_package "TBB" "libtbb.so" "/usr/include/tbb/tbb.h" || MISSING_PACKAGES="$MISSING_PACKAGES tbb-devel"

# Make the Boost check more flexible
if [ -f "/usr/include/boost/config.hpp" ] || [ -f "/usr/local/include/boost/config.hpp" ]; then
  if ldconfig -p | grep -q -i "libboost_system" || find /usr/lib* -name "libboost_system*.so*" | grep -q .; then
    echo "✓ Boost found"
  else
    echo "✗ Boost libraries not found"
    MISSING_PACKAGES="$MISSING_PACKAGES boost-devel"
  fi
else
  echo "✗ Boost headers not found"
  MISSING_PACKAGES="$MISSING_PACKAGES boost-devel"
fi


# Add these to CMAKE_ARGS
export CMAKE_ARGS="${CMAKE_ARGS} -DPYTHON_ROOT_DIR=${PYTHON_ROOT_DIR}"
export CMAKE_ARGS="${CMAKE_ARGS} -DPYTHON_INCLUDE_DIR=${PYTHON_INCLUDE_DIR}"
export CMAKE_ARGS="${CMAKE_ARGS} -DPYTHON_LIBRARY=${PYTHON_LIBRARY}"
export CMAKE_ARGS="${CMAKE_ARGS} -DPYTHON_LIBPATH=${PYTHON_LIBPATH}"
export CMAKE_ARGS="${CMAKE_ARGS} -DPYTHON_INCLUDE_CONFIG_DIR=${PYTHON_INCLUDE_CONFIG_DIR}"
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
  -DPYTHON_ROOT_DIR=${PYTHON_ROOT_DIR} \
  -DPYTHON_INCLUDE_DIR=${PYTHON_INCLUDE_DIR} \
  -DPYTHON_LIBRARY=${PYTHON_LIBRARY} \
  -DPYTHON_LIBPATH=${PYTHON_LIBPATH} \
  -DPYTHON_INCLUDE_CONFIG_DIR=${PYTHON_INCLUDE_CONFIG_DIR} \
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