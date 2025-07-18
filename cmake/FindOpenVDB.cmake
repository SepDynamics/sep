# FindOpenVDB.cmake
# Find OpenVDB library and include paths
#
# This module defines:
# OpenVDB_FOUND - True if OpenVDB was found
# OpenVDB_INCLUDE_DIRS - OpenVDB include directories
# OpenVDB_LIBRARIES - OpenVDB libraries

find_path(OpenVDB_INCLUDE_DIR
    NAMES openvdb/openvdb.h
    PATHS
        /usr/include
        /usr/local/include
        $ENV{OPENVDB_ROOT}/src
)

find_library(OpenVDB_LIBRARY
    NAMES openvdb
    PATHS
        /usr/lib
        /usr/lib64
        /usr/local/lib
        /usr/local/lib64
        $ENV{OPENVDB_ROOT}/lib
)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(OpenVDB
    REQUIRED_VARS 
        OpenVDB_LIBRARY
        OpenVDB_INCLUDE_DIR
)

if(OpenVDB_FOUND)
    set(OpenVDB_LIBRARIES ${OpenVDB_LIBRARY})
    set(OpenVDB_INCLUDE_DIRS ${OpenVDB_INCLUDE_DIR})
endif()

mark_as_advanced(
    OpenVDB_INCLUDE_DIR
    OpenVDB_LIBRARY
)