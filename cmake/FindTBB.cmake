# FindTBB.cmake
# Find Intel Threading Building Blocks (TBB) library
#
# This module defines:
#  TBB_FOUND - True if TBB is found
#  TBB_INCLUDE_DIRS - The TBB include directory
#  TBB_LIBRARIES - The libraries needed to use TBB
#  TBB::tbb - Imported target for the TBB library
#
# This module will look for TBB in standard system locations and can be
# influenced by setting the TBB_ROOT environment variable.

# Try to find TBB using pkg-config first
find_package(PkgConfig QUIET)
if(PKG_CONFIG_FOUND)
  pkg_check_modules(PC_TBB QUIET tbb)
endif()

# Find the TBB include directory
find_path(TBB_INCLUDE_DIR
  NAMES tbb/tbb.h
  PATHS
    ${PC_TBB_INCLUDE_DIRS}
    $ENV{TBB_ROOT}/include
    /usr/include
    /usr/local/include
  DOC "TBB include directory"
)

# Find the TBB library - prioritize newer version (libtbb.so.12)
find_library(TBB_LIBRARY
  NAMES tbb.12 tbb
  PATHS
    ${PC_TBB_LIBRARY_DIRS}
    $ENV{TBB_ROOT}/lib
    $ENV{TBB_ROOT}/lib64
    /usr/lib
    /usr/lib64
    /usr/local/lib
    /usr/local/lib64
  DOC "TBB library"
)

# Set standard CMake find_package variables
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(TBB
  REQUIRED_VARS TBB_LIBRARY TBB_INCLUDE_DIR
)

if(TBB_FOUND)
  set(TBB_LIBRARIES ${TBB_LIBRARY})
  set(TBB_INCLUDE_DIRS ${TBB_INCLUDE_DIR})
  
  # Create imported target
  if(NOT TARGET TBB::tbb)
    add_library(TBB::tbb UNKNOWN IMPORTED)
    set_target_properties(TBB::tbb PROPERTIES
      IMPORTED_LOCATION "${TBB_LIBRARY}"
      INTERFACE_INCLUDE_DIRECTORIES "${TBB_INCLUDE_DIR}"
    )
  endif()
endif()

mark_as_advanced(TBB_INCLUDE_DIR TBB_LIBRARY)