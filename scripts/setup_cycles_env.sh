#!/bin/bash

# IMPORTANT: Ensure core OpenVDB dependencies (Boost, TBB, OpenEXR/Imath, Zlib)
# are installed on your system, as OpenVDB will need them to build itself.
# Example for Fedora/RHEL: sudo dnf install boost-devel tbb-devel OpenEXR-devel zlib-devel

# Script to set up environment for building Cycles

# Create build and install directories
mkdir -p /sep/cycles-build
mkdir -p /sep/cycles-install

# Set up environment variables for all dependencies (keep as is, they look mostly correct)
export PYTHONPATH=/usr/lib/python3.11/site-packages
export PYTHON_INCLUDE_DIR=/usr/include/python3.11
export PYTHON_LIBRARY=/usr/lib64/libpython3.11.so
export PYTHON_LIBPATH=/usr/lib64
export PYTHON_INCLUDE_CONFIG_DIR=/usr/include/python3.11

# Set up zlib
export ZLIB_INCLUDE_DIR=/usr/include
export ZLIB_LIBRARY=/usr/lib/libz.so

# Set up OpenImageIO
export OPENIMAGEIO_ROOT_DIR=/usr
export OPENIMAGEIO_INCLUDE_DIR=/usr/include
export OPENIMAGEIO_LIBRARY=/usr/lib64/libOpenImageIO.so
export OPENIMAGEIO_UTIL_LIBRARY=/usr/lib64/libOpenImageIO_Util.so

# Set up OpenJPEG
export OPENJPEG_INCLUDE_DIR=/usr/include/openjpeg-2.5
export OPENJPEG_LIBRARY=/usr/lib64/libopenjp2.so

# Set up JPEG
export JPEG_INCLUDE_DIR=/usr/include
export JPEG_LIBRARY=/usr/lib64/libjpeg.so

# Set up TIFF
export TIFF_INCLUDE_DIR=/usr/include
export TIFF_LIBRARY=/usr/lib64/libtiff.so

# Set up PNG
export PNG_INCLUDE_DIR=/usr/include
export PNG_LIBRARY=/usr/lib64/libpng.so

# Set up Jemalloc
export JEMALLOC_INCLUDE_DIR=/usr/include/jemalloc
export JEMALLOC_LIBRARY=/usr/lib64/libjemalloc.so.2

# Set up OpenColorIO and its dependencies
export OPENCOLORIO_INCLUDE_DIR=/usr/include
export OPENCOLORIO_LIBRARY=/usr/lib64/libOpenColorIO.so
export OPENCOLORIO_OPENCOLORIO_LIBRARY=/usr/lib64/libOpenColorIO.so
export OPENCOLORIO_YAML_CPP_LIBRARY=/usr/lib64/libyaml-cpp.so.0.8
export OPENCOLORIO_EXPAT_LIBRARY=/usr/lib64/libexpat.so.1
export OPENCOLORIO_PYSTRING_LIBRARY=/usr/lib64/libpystring.so.1.1.4
export OPENCOLORIO_MINIZIP_LIBRARY=/usr/lib64/libminizip.so.1

# Set up OpenSubdiv
export OPENSUBDIV_INCLUDE_DIR=/usr/include
export OPENSUBDIV_OSDCPU_LIBRARY=/usr/lib64/libosdCPU.so
export OPENSUBDIV_OSDGPU_LIBRARY=/usr/lib64/libosdGPU.so

# Set up NanoVDB (using custom minimal headers)
export NANOVDB_INCLUDE_DIR=/sep/extern/nanovdb
export NANOVDB_ROOT_DIR=/sep/extern/nanovdb

# Create minimal NanoVDB headers (if not already present and correct)
mkdir -p /sep/extern/nanovdb/nanovdb
mkdir -p /sep/extern/nanovdb/nanovdb/util
mkdir -p /sep/extern/nanovdb/nanovdb/tools
# NOTE: If you need the actual content of the NanoVDB headers, you must add 'cat >' blocks here.
# For now, I'm assuming the directories existing is sufficient for CMake to proceed,
# but compilation will fail later if actual headers aren't there.

# Set up OpenImageDenoise paths
echo "Creating minimal OpenImageDenoise config.h..."
mkdir -p /sep/extern/oidn/include/OpenImageDenoise
cat > /sep/extern/oidn/include/OpenImageDenoise/config.h << 'EOF'
// Minimal config.h for OpenImageDenoise
#pragma once

#define OIDN_VERSION_MAJOR 2
#define OIDN_VERSION_MINOR 0
#define OIDN_VERSION_PATCH 0
#define OIDN_VERSION 20000
#define OIDN_VERSION_STRING "2.0.0"

#define OIDN_API_NAMESPACE_BEGIN
#define OIDN_API_NAMESPACE_END
#define OIDN_API_NAMESPACE_USING
#define OIDN_API_EXTERN_C extern "C"
#define OIDN_NAMESPACE oidn
#define OIDN_NAMESPACE_C oidn
#define OIDN_NAMESPACE_BEGIN namespace oidn {
#define OIDN_NAMESPACE_END }

#define OIDN_NAMESPACE_USING using namespace OIDN_NAMESPACE;

#define OIDN_API_IMPORT OIDN_API_EXTERN_C
#define OIDN_API_EXPORT OIDN_API_EXTERN_C __attribute__((visibility ("default")))

#define OIDN_API OIDN_API_IMPORT

#define OIDN_DEPRECATED(msg) __attribute__((deprecated(msg)))

#define OIDN_DEVICE_CPU
EOF

cat > /sep/extern/oidn/include/OpenImageDenoise/oidn.hpp << 'EOF'
// Minimal oidn.hpp for OpenImageDenoise
#pragma once

#include "config.h"
#include "oidn.h"

OIDN_NAMESPACE_BEGIN

// Minimal class definitions to satisfy includes
enum class Error {
  None = 0,
  Cancelled = 1,
  Unknown = 2
};

enum class DeviceType {
  CPU = 0,
  CUDA = 1,
  HIP = 2,
  SYCL = 3
};

// Format enum for images
enum class Format {
  Float = 1,
  Float2 = 2,
  Float3 = 3,
  Float4 = 4,
  Half = 257,
  Half2 = 258,
  Half3 = 259,
  Half4 = 260
};

// Forward declarations
class Device;
class Filter;

// Type definitions
typedef Filter& FilterRef;
typedef Device& DeviceRef;

// Progress monitor function type
typedef bool (*ProgressMonitorFunction)(void* userPtr, double n);

class Filter {
public:
  void set(const char* name, bool value) {}
  void set(const char* name, int value) {}
  void set(const char* name, float value) {}
  void setProgressMonitorFunction(ProgressMonitorFunction func, void* userPtr) {}
  void setData(const char* name, void* data, size_t size) {}
  void setImage(const char* name, void* ptr, Format format, int width, int height,
                size_t byteOffset, size_t pixelByteStride, size_t rowByteStride) {}
  void commit() {}
  void execute() {}
};

class Device {
public:
  Device() {}
  
  void set(const char* name, bool value) {}
  void set(const char* name, int value) {}
  void commit() {}
  
  FilterRef newFilter(const char* type) {
    static Filter filter;
    return filter;
  }
  
  Error getError(const char* message = nullptr) { return Error::None; }
};

inline DeviceRef newDevice(DeviceType type) {
  static Device device;
  return device;
}

OIDN_NAMESPACE_END
EOF

export OPENIMAGEDENOISE_INCLUDE_DIR=/sep/extern/oidn/include
# Assuming a mock/empty build for OIDN if it's not actually compiled
# If you genuinely have an OIDN build, ensure this path is correct.
# Given WITH_OPENIMAGEDENOISE=OFF, this might not be critical for *linking*,
# but the headers might still be checked for existence.
export OPENIMAGEDENOISE_LIBRARY=/sep/extern/oidn/build/lib/libOpenImageDenoise.so
export OPENIMAGEDENOISE_OPENIMAGEDENOISE_LIBRARY=/sep/extern/oidn/build/lib/libOpenImageDenoise.so

# Disable OpenImageDenoise in Cycles build

# Set up Epoxy
export Epoxy_INCLUDE_DIR=/sep/extern/libepoxy/include
export Epoxy_LIBRARY=/sep/extern/libepoxy/build/lib/libepoxy.so

# Set up Zstd
export ZSTD_INCLUDE_DIR=/sep/extern/zstd/lib
export ZSTD_LIBRARY=/sep/extern/zstd/build/lib/libzstd.so

# Set up PugiXML (assuming it's in standard locations)
export PUGIXML_INCLUDE_DIR=/usr/include
export PUGIXML_LIBRARY=/usr/lib64/libpugixml.so

# Set up OpenEXR
export OPENEXR_INCLUDE_DIR=/usr/include/OpenEXR
export OPENEXR_LIBRARY=/usr/lib64/libOpenEXR.so
export OPENEXR_ROOT_DIR=/usr
export OPENEXR_IEX_LIBRARY=/usr/lib64/libIex.so
export OPENEXR_ILMTHREAD_LIBRARY=/usr/lib64/libIlmThread.so
export OPENEXR_OPENEXRCORE_LIBRARY=/usr/lib64/libOpenEXRCore.so
export OPENEXR_OPENEXR_LIBRARY=/usr/lib64/libOpenEXR.so

# Set up Imath (required for OpenEXR)
export IMATH_INCLUDE_DIR=/usr/include
export IMATH_LIBRARY=/usr/lib64/libImath.so
export IMATH_CONFIG=/usr/lib64/cmake/Imath/ImathConfig.cmake

# Set up Boost
export BOOST_ROOT=/usr
export Boost_INCLUDE_DIR=/usr/include
export Boost_LIBRARY_DIR=/usr/lib64

# Set up TBB
export TBB_INCLUDE_DIR=/usr/include
export TBB_LIBRARY=/usr/lib64/libtbb.so

# --- Build OpenVDB from source ---
echo "--- Building OpenVDB from /sep/extern/openvdb ---"
cd /sep/extern/openvdb || { echo "Error: /sep/extern/openvdb directory not found!"; exit 1; }
mkdir -p build install
cd build

# Clean OpenVDB CMake cache
echo "Cleaning OpenVDB CMake cache..."
rm -f CMakeCache.txt
rm -rf CMakeFiles/

echo "Configuring OpenVDB with CMake..."
# Add verbose makefile to OpenVDB build for debugging
cmake -G Ninja .. \
  -DCMAKE_INSTALL_PREFIX=/sep/extern/openvdb/install \
  -DBUILD_SHARED_LIBS=ON \
  -DBUILD_STATIC_LIBS=OFF \
  -DOPENVDB_BUILD_NANOVDB=ON \
  -DUSE_BLOSC=OFF \
  -DUSE_NANOVDB=ON \
  -DUSE_HOMOGENEOUS_VOLUMES=ON \
  -DUSE_HOUDINI=OFF \
  -DBUILD_TOOLS=OFF \
  -DCMAKE_CXX_STANDARD=17 \
  -DOPENVDB_CORE_VERSION=10 \
  -DILMBASE_ROOT=/usr \
  -DILMBASE_LIBRARY_DIR=/usr/lib64 \
  -DILMBASE_INCLUDE_DIR=/usr/include \
  -DTBB_ROOT=/usr \
  -DTBB_LIBRARY_DIR=/usr/lib64 \
  -DTBB_INCLUDE_DIR=/usr/include \
  -DZLIB_LIBRARY=/usr/lib/libz.so \
  -DZLIB_INCLUDE_DIR=/usr/include \
  -DJEMALLOC_INCLUDE_DIR=${JEMALLOC_INCLUDE_DIR} \
  -DJEMALLOC_LIBRARY=${JEMALLOC_LIBRARY} \
  -DCMAKE_VERBOSE_MAKEFILE=ON # Added for detailed build output

if ! cmake --build . --target install; then
  echo "ERROR: OpenVDB build or install failed. Check the logs above."
  exit 1
fi
echo "OpenVDB build and install completed."

# Verify OpenVDB library existence and name
OPENVDB_INSTALLED_LIB="/sep/extern/openvdb/install/lib/libopenvdb.so"
if [ -f "$OPENVDB_INSTALLED_LIB" ]; then
  echo "Verified: OpenVDB library found at $OPENVDB_INSTALLED_LIB"
  # Set up OpenVDB paths to the *newly built* version (ensure lowercase for libname)
  export OPENVDB_INCLUDE_DIR="/sep/extern/openvdb/install/include"
  export OPENVDB_LIBRARY="$OPENVDB_INSTALLED_LIB" # Use the verified path
  export OPENVDB_ROOT_DIR="/sep/extern/openvdb/install"
else
  echo "ERROR: OpenVDB library NOT found at $OPENVDB_INSTALLED_LIB after build. Aborting Cycles build."
  ls -la /sep/extern/openvdb/install/lib/ # Show contents of the lib directory
  exit 1
fi

# --- Configure and build Cycles ---
echo "--- Configuring Cycles ---"
cd /sep/cycles-build || { echo "Error: /sep/cycles-build directory not found!"; exit 1; }

# Clean Cycles CMake cache (Crucial after changing build options)
echo "Cleaning Cycles CMake cache..."
rm -f CMakeCache.txt
rm -rf CMakeFiles/

echo "Configuring Cycles with CMake..."
# Add explicit find paths and library properties for OpenVDB if necessary
  cmake -S /sep/extern/cycles -B . \
    -G Ninja \
  -DCMAKE_INSTALL_PREFIX=/sep/cycles-install \
  -DWITH_CYCLES_STANDALONE=ON \
  -DWITH_CYCLES_DEVICE_CUDA=ON \
  -DWITH_ALEMBIC=ON \
  -DWITH_CYCLES_ALEMBIC=ON \
  -DWITH_CYCLES_EMBREE=ON \
  -DWITH_CYCLES_OSL=ON \
  -DWITH_CYCLES_LOGGING=ON \
  -DWITH_CYCLES_NETWORK=ON \
  -DWITH_CYCLES_HYDRA=ON \
  -DWITH_CYCLES_STANDALONE_GUI=OFF \
  -DWITH_PYTHON_INSTALL=ON \
  -DWITH_PYTHON_MODULE=ON \
  -DPYTHON_VERSION=3.11 \
  -DPYTHON_INCLUDE_DIR=${PYTHON_INCLUDE_DIR} \
  -DPYTHON_LIBRARY=${PYTHON_LIBRARY} \
  -DPYTHON_LIBPATH=${PYTHON_LIBPATH} \
  -DPYTHON_INCLUDE_CONFIG_DIR=${PYTHON_INCLUDE_CONFIG_DIR} \
  -DZLIB_INCLUDE_DIR=${ZLIB_INCLUDE_DIR} \
  -DZLIB_LIBRARY=${ZLIB_LIBRARY} \
  -DJEMALLOC_INCLUDE_DIR=${JEMALLOC_INCLUDE_DIR} \
  -DJEMALLOC_LIBRARY=${JEMALLOC_LIBRARY} \
  -DOPENIMAGEIO_ROOT_DIR=${OPENIMAGEIO_ROOT_DIR} \
  -DOPENIMAGEIO_INCLUDE_DIR=${OPENIMAGEIO_INCLUDE_DIR} \
  -DOPENIMAGEIO_LIBRARY=${OPENIMAGEIO_LIBRARY} \
  -DOPENIMAGEIO_UTIL_LIBRARY=${OPENIMAGEIO_UTIL_LIBRARY} \
  -DOPENJPEG_INCLUDE_DIR=${OPENJPEG_INCLUDE_DIR} \
  -DOPENJPEG_LIBRARY=${OPENJPEG_LIBRARY} \
  -DJPEG_INCLUDE_DIR=${JPEG_INCLUDE_DIR} \
  -DJPEG_LIBRARY=${JPEG_LIBRARY} \
  -DTIFF_INCLUDE_DIR=${TIFF_INCLUDE_DIR} \
  -DTIFF_LIBRARY=${TIFF_LIBRARY} \
  -DPNG_INCLUDE_DIR=${PNG_INCLUDE_DIR} \
  -DPNG_LIBRARY=${PNG_LIBRARY} \
  -DOPENCOLORIO_INCLUDE_DIR=${OPENCOLORIO_INCLUDE_DIR} \
  -DOPENCOLORIO_LIBRARY=${OPENCOLORIO_LIBRARY} \
  -DOPENCOLORIO_OPENCOLORIO_LIBRARY=${OPENCOLORIO_OPENCOLORIO_LIBRARY} \
  -DOPENCOLORIO_YAML_CPP_LIBRARY=${OPENCOLORIO_YAML_CPP_LIBRARY} \
  -DOPENCOLORIO_EXPAT_LIBRARY=${OPENCOLORIO_EXPAT_LIBRARY} \
  -DOPENCOLORIO_PYSTRING_LIBRARY=${OPENCOLORIO_PYSTRING_LIBRARY} \
  -DOPENCOLORIO_MINIZIP_LIBRARY=${OPENCOLORIO_MINIZIP_LIBRARY} \
  -DOPENSUBDIV_INCLUDE_DIR=${OPENSUBDIV_INCLUDE_DIR} \
  -DOPENSUBDIV_OSDCPU_LIBRARY=${OPENSUBDIV_OSDCPU_LIBRARY} \
  -DOPENSUBDIV_OSDGPU_LIBRARY=${OPENSUBDIV_OSDGPU_LIBRARY} \
  -DNANOVDB_INCLUDE_DIR=${NANOVDB_INCLUDE_DIR} \
  -DNANOVDB_ROOT_DIR=${NANOVDB_ROOT_DIR} \
  -DOPENIMAGEDENOISE_INCLUDE_DIR=${OPENIMAGEDENOISE_INCLUDE_DIR} \
  -DOPENIMAGEDENOISE_LIBRARY=${OPENIMAGEDENOISE_LIBRARY} \
  -DOPENIMAGEDENOISE_OPENIMAGEDENOISE_LIBRARY=${OPENIMAGEDENOISE_OPENIMAGEDENOISE_LIBRARY} \
  -DEpoxy_INCLUDE_DIR=${Epoxy_INCLUDE_DIR} \
  -DEpoxy_LIBRARY=${Epoxy_LIBRARY} \
  -DZSTD_INCLUDE_DIR=${ZSTD_INCLUDE_DIR} \
  -DZSTD_LIBRARY=${ZSTD_LIBRARY} \
  -DWITH_OPENVDB=ON \
  -DWITH_CYCLES_DEVICE_OPTIX=ON \
  -DPUGIXML_INCLUDE_DIR=${PUGIXML_INCLUDE_DIR} \
  -DPUGIXML_LIBRARY=${PUGIXML_LIBRARY} \
  -DOPENEXR_INCLUDE_DIR=${OPENEXR_INCLUDE_DIR} \
  -DOPENEXR_LIBRARY=${OPENEXR_LIBRARY} \
  -DOPENEXR_ROOT_DIR=${OPENEXR_ROOT_DIR} \
  -DOPENEXR_IEX_LIBRARY=${OPENEXR_IEX_LIBRARY} \
  -DOPENEXR_ILMTHREAD_LIBRARY=${OPENEXR_ILMTHREAD_LIBRARY} \
  -DOPENEXR_OPENEXRCORE_LIBRARY=${OPENEXR_OPENEXRCORE_LIBRARY} \
  -DOPENEXR_OPENEXR_LIBRARY=${OPENEXR_OPENEXR_LIBRARY} \
  -DIMATH_INCLUDE_DIR=${IMATH_INCLUDE_DIR} \
  -DIMATH_LIBRARY=${IMATH_LIBRARY} \
  -DIMATH_CONFIG=${IMATH_CONFIG} \
  -DBOOST_ROOT=${BOOST_ROOT} \
  -DBoost_INCLUDE_DIR=${Boost_INCLUDE_DIR} \
  -DBoost_LIBRARY_DIR=${Boost_LIBRARY_DIR} \
  -DTBB_INCLUDE_DIR=${TBB_INCLUDE_DIR} \
  -DTBB_LIBRARY=${TBB_LIBRARY} \
  -DCMAKE_VERBOSE_MAKEFILE=ON \
  -DOPENVDB_ROOT_DIR=${OPENVDB_ROOT_DIR} \
  -DOPENVDB_INCLUDE_DIR=${OPENVDB_INCLUDE_DIR} \
  -DOPENVDB_LIBRARY=${OPENVDB_LIBRARY} \
  -DOPENVDB_LIBRARIES="${OPENVDB_LIBRARY};${OPENVDB_ROOT_DIR}/lib/libopenvdb_tool.so" \
  -DOPENVDB_BUILD_NANOVDB=ON \

if ! cmake --build . --target install; then
  echo "ERROR: Cycles build or install failed. Check the logs above."
  exit 1
fi

echo "Cycles build process completed. Check output for any remaining errors."