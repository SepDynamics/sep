# FindImath.cmake
# Find Imath library and include paths
#
# This module defines:
# Imath_FOUND - True if Imath was found
# Imath_INCLUDE_DIRS - Imath include directories
# Imath_LIBRARIES - Imath libraries

find_path(Imath_INCLUDE_DIR
    NAMES Imath/ImathConfig.h
    PATHS
        /usr/include
        /usr/local/include
        $ENV{IMATH_ROOT}/src
)

find_library(Imath_LIBRARY
    NAMES Imath Imath-3_1
    PATHS
        /usr/lib
        /usr/lib64
        /usr/local/lib
        /usr/local/lib64
        $ENV{IMATH_ROOT}/lib
)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Imath
    REQUIRED_VARS 
        Imath_LIBRARY
        Imath_INCLUDE_DIR
)

if(Imath_FOUND)
    set(Imath_LIBRARIES ${Imath_LIBRARY})
    set(Imath_INCLUDE_DIRS ${Imath_INCLUDE_DIR})
endif()

mark_as_advanced(
    Imath_INCLUDE_DIR
    Imath_LIBRARY
)