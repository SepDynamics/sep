#!/usr/bin/env bash
set -e

# Build OpenShadingLanguage if not installed system-wide
OSL_ROOT=/sep/extern/osl
OSL_VERSION_TAG=v1.13.12.0

if [ ! -f "${OSL_ROOT}/install/lib/liboslcomp.so" ]; then
  echo "--- Building OpenShadingLanguage ${OSL_VERSION_TAG} ---"
  mkdir -p /sep/extern
  cd /sep/extern
  if [ ! -d "osl" ]; then
    git clone --depth 1 --branch ${OSL_VERSION_TAG} https://github.com/AcademySoftwareFoundation/OpenShadingLanguage.git osl
  fi
  cd osl
  mkdir -p build install
  cd build
  cmake .. -G Ninja \
    -DCMAKE_INSTALL_PREFIX=${OSL_ROOT}/install \
    -DBUILD_SHARED_LIBS=ON \
    -DBUILD_TESTING=OFF \
    -DCMAKE_CXX_STANDARD=17
  cmake --build . --target install
fi

export OSL_ROOT_DIR=${OSL_ROOT}/install
export OSL_INCLUDE_DIR=${OSL_ROOT_DIR}/include
export OSL_OSLCOMP_LIBRARY=${OSL_ROOT_DIR}/lib/liboslcomp.so
export OSL_OSLEXEC_LIBRARY=${OSL_ROOT_DIR}/lib/liboslexec.so
export OSL_OSLQUERY_LIBRARY=${OSL_ROOT_DIR}/lib/liboslquery.so
export OSL_OSLNOISE_LIBRARY=${OSL_ROOT_DIR}/lib/liboslnoise.so
