# FindOpenColorIO.cmake
# Find OpenColorIO library and include paths
#
# This module defines:
# OpenColorIO_FOUND - True if OpenColorIO was found
# OpenColorIO_INCLUDE_DIRS - OpenColorIO include directories
# OpenColorIO_LIBRARIES - OpenColorIO libraries

find_path(OpenColorIO_INCLUDE_DIR
    NAMES OpenColorIO/OpenColorIO.h
    PATHS
        /usr/include
        /usr/local/include
        $ENV{OCIO_ROOT}/src
)

find_library(OpenColorIO_LIBRARY
    NAMES OpenColorIO
    PATHS
        /usr/lib
        /usr/lib64
        /usr/local/lib
        /usr/local/lib64
        $ENV{OCIO_ROOT}/lib
)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(OpenColorIO
    REQUIRED_VARS 
        OpenColorIO_LIBRARY
        OpenColorIO_INCLUDE_DIR
)

if(OpenColorIO_FOUND)
    set(OpenColorIO_LIBRARIES ${OpenColorIO_LIBRARY})
    set(OpenColorIO_INCLUDE_DIRS ${OpenColorIO_INCLUDE_DIR})
endif()

mark_as_advanced(
    OpenColorIO_INCLUDE_DIR
    OpenColorIO_LIBRARY
)