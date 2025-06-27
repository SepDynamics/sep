#!/bin/bash
set -eo pipefail
clear

echo "==== SEP Engine Build Script with FULL Cycles and PipeWire Support ===="
echo "Setting up build environment with real library paths - NO STUBS!"

# Create build directory
mkdir -p /sep/build

# Create symbolic link for Cycles if needed
if [ ! -d "/sep/extern/cycles" ]; then
  echo "Creating Cycles directory structure..."
  mkdir -p /sep/extern
  
  # Try to find Cycles in common locations
  if [ -d "/sep/cycles" ]; then
    echo "Found Cycles at /sep/cycles, creating symlink..."
    ln -sf /sep/cycles /sep/extern/cycles
  else
    echo "Error: Could not find Cycles directory"
    exit 1
  fi
fi

# Instead of sourcing setup_cycles_env_fixed.sh which creates stubs,
# directly set needed environment variables for real libraries

# Direct system paths for Python - NO STUBS!
export PYTHON_VERSION="3.11"
export PYTHON_LIBRARY="/usr/lib64/libpython3.11.so"
export PYTHON_LIBPATH="/usr/lib64"
export PYTHON_INCLUDE_DIR="/usr/include/python3.11"
export PYTHON_INCLUDE_CONFIG_DIR="/usr/include/python3.11"
export PYTHONLIBS_FOUND=TRUE
export PYTHONLIBS_VERSION_STRING="3.11"

# Direct system paths for libraries - NO STUBS!
# Set up zlib
export ZLIB_INCLUDE_DIR="/usr/include"
export ZLIB_LIBRARY="/usr/lib/libz.so"

# Set up OpenImageIO - use real system paths
export OPENIMAGEIO_ROOT_DIR="/usr"
export OPENIMAGEIO_INCLUDE_DIR="/usr/include"
export OPENIMAGEIO_LIBRARY="/usr/lib64/libOpenImageIO.so"
export OPENIMAGEIO_UTIL_LIBRARY="/usr/lib64/libOpenImageIO_Util.so"

# Set up OpenJPEG
export OPENJPEG_INCLUDE_DIR="/usr/include/openjpeg-2.5"
export OPENJPEG_LIBRARY="/usr/lib64/libopenjp2.so"

# Set up JPEG
export JPEG_INCLUDE_DIR="/usr/include"
export JPEG_LIBRARY="/usr/lib64/libjpeg.so"

# Set up TIFF
export TIFF_INCLUDE_DIR="/usr/include"
export TIFF_LIBRARY="/usr/lib64/libtiff.so"

# Set up PNG
export PNG_INCLUDE_DIR="/usr/include"
export PNG_LIBRARY="/usr/lib64/libpng.so"

# Set up Jemalloc
export JEMALLOC_INCLUDE_DIR="/usr/include/jemalloc"
export JEMALLOC_LIBRARY="/usr/lib64/libjemalloc.so.2"

# Set up OpenColorIO and its dependencies
export OPENCOLORIO_INCLUDE_DIR="/usr/include"
export OPENCOLORIO_LIBRARY="/usr/lib64/libOpenColorIO.so"

# Set up OpenSubdiv
export OPENSUBDIV_INCLUDE_DIR="/usr/include"
export OPENSUBDIV_OSDCPU_LIBRARY="/usr/lib64/libosdCPU.so"
export OPENSUBDIV_OSDGPU_LIBRARY="/usr/lib64/libosdGPU.so"

# Set up OpenEXR
export OPENEXR_INCLUDE_DIR="/usr/include/OpenEXR"
export OPENEXR_LIBRARY="/usr/lib64/libOpenEXR.so"
export OPENEXR_ROOT_DIR="/usr"

# Set up Boost
export BOOST_ROOT="/usr"
export Boost_INCLUDE_DIR="/usr/include"
export Boost_LIBRARY_DIR="/usr/lib64"

# Set up TBB
export TBB_INCLUDE_DIR="/usr/include"
export TBB_LIBRARY="/usr/lib64/libtbb.so"

# Set up OptiX
export OPTIX_ROOT_DIR="/sep/extern/optix-dev"

# Print Python settings to confirm
echo "Using Python settings (REAL PATHS, NO STUBS):"
echo "  PYTHON_VERSION=$PYTHON_VERSION"
echo "  PYTHON_LIBRARY=$PYTHON_LIBRARY (exists: $([ -f $PYTHON_LIBRARY ] && echo YES || echo NO))"
echo "  PYTHON_LIBPATH=$PYTHON_LIBPATH (exists: $([ -d $PYTHON_LIBPATH ] && echo YES || echo NO))"
echo "  PYTHON_INCLUDE_DIR=$PYTHON_INCLUDE_DIR (exists: $([ -d $PYTHON_INCLUDE_DIR ] && echo YES || echo NO))"
echo "  PYTHON_INCLUDE_CONFIG_DIR=$PYTHON_INCLUDE_CONFIG_DIR (exists: $([ -d $PYTHON_INCLUDE_CONFIG_DIR ] && echo YES || echo NO))"

# Check for PipeWire headers using pkg-config
echo "Checking for PipeWire development headers..."
PIPEWIRE_FOUND=0
PIPEWIRE_CMAKE_ARGS=""

# Try using pkg-config first (most reliable)
if command -v pkg-config >/dev/null 2>&1; then
  echo "Using pkg-config to find PipeWire..."
  if pkg-config --exists libpipewire-0.3; then
    PIPEWIRE_CFLAGS=$(pkg-config --cflags libpipewire-0.3)
    PIPEWIRE_LIBS=$(pkg-config --libs libpipewire-0.3)
    PIPEWIRE_INCLUDE_DIR=$(pkg-config --variable=includedir libpipewire-0.3)
    PIPEWIRE_LIB_DIR=$(pkg-config --variable=libdir libpipewire-0.3)
    PIPEWIRE_FOUND=1
    
    # Create CMake arguments for PipeWire
    PIPEWIRE_CMAKE_ARGS="-DSEP_HAS_PIPEWIRE=ON -DPIPEWIRE_INCLUDE_DIR=${PIPEWIRE_INCLUDE_DIR} -DPIPEWIRE_LIBRARY=${PIPEWIRE_LIB_DIR}/libpipewire-0.3.so"
    echo "PipeWire found via pkg-config: ${PIPEWIRE_INCLUDE_DIR}"
  fi
fi

# Clean build directory to ensure clean state
echo "Cleaning build directory..."
rm -rf /sep/build/*
cp -r /sep/cmake /sep/build

# Configure the SEP Engine with CMake using explicit paths
# Force our Python paths to be used by passing them directly to CMake
cd /sep/build
echo "Running CMake configuration with REAL LIBRARY PATHS - NO STUBS..."



# Fix path to Cycles root directory
if [ -d "/sep/extern/cycles" ]; then
  echo "Found Cycles at /sep/extern/cycles"
  CYCLES_ROOT="/sep/extern/cycles"
elif [ -d "/sep/cycles" ]; then
  echo "Found Cycles at /sep/cycles"
  CYCLES_ROOT="/sep/cycles"
else
  echo "Error: Could not find Cycles directory"
  exit 1
fi



CYCLES_FLAG="-DSEP_HAS_CYCLES=ON -DCYCLES_ROOT_DIR=${CYCLES_ROOT}"
# Run CMake with all flags properly set
cmake -S /sep -B /sep/build \
  -DCMAKE_C_COMPILER=/usr/bin/gcc-14 \
  -DCMAKE_CXX_COMPILER=/usr/bin/g++-14 \
  ${CYCLES_FLAG} \
  -DWITH_CYCLES_DEVICE_OPTIX=ON \
  -DOPTIX_ROOT_DIR="/sep/extern/optix-dev" \
  -DPYTHON_VERSION="3.11" \
  -DPYTHON_LIBRARY="/usr/lib64/libpython3.11.so" \
  -DPYTHON_INCLUDE_DIR="/usr/include/python3.11" \
  -DPYTHON_LIBPATH="/usr/lib64" \
  -DPYTHON_INCLUDE_CONFIG_DIR="/usr/include/python3.11" \
  -DPYTHONLIBS_FOUND=TRUE \
  -DPYTHONLIBS_VERSION_STRING="3.11" \
  ${PIPEWIRE_CMAKE_ARGS} \
  -DWITH_OPENIMAGEDENOISE=OFF \
  -DCMAKE_EXPORT_COMPILE_COMMANDS=ON

# Build the project
echo "Building SEP Engine..."
make -j$(nproc)

sudo systemctl stop sep-engine 

sudo ln -sf /sep/build/sep_engine /usr/local/bin/sep_engine

sudo systemctl start sep-engine
sudo systemctl status sep-engine

echo "==== Build Complete ===="
echo "If successful, the SEP Engine binary is at: /sep/build/sep_engine"

# Check if Cycles was enabled properly
if grep -q "Cycles build failed or libraries not found" /sep/build/CMakeFiles/CMakeOutput.log 2>/dev/null || \
   grep -q "Cycles build failed or libraries not found" /sep/build/CMakeFiles/CMakeError.log 2>/dev/null; then
  echo "WARNING: Cycles integration was not enabled properly. Using stub implementation."
  echo "Check /sep/build/CMakeFiles/CMakeOutput.log and /sep/build/CMakeFiles/CMakeError.log for details."
else
  echo "Cycles integration appears to be working with REAL IMPLEMENTATION - NO STUBS!"
fi

# Check if PipeWire was enabled
if [ $PIPEWIRE_FOUND -eq 1 ]; then
  echo "PipeWire support is enabled with REAL IMPLEMENTATION. Audio capture should be available."
else
  echo "WARNING: PipeWire support is disabled. Audio capture will not be available."
fi

echo "Run the engine to verify both Cycles renderer and PipeWire audio functionality."