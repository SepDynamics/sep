# FindHiredis.cmake

# Find Hiredis library and headers
find_library(HIREDIS_LIBRARIES
    NAMES
        hiredis
        hiredis_standalone
    PATHS
        /usr/lib
        /usr/lib64
        /usr/local/lib
        /usr/local/lib64
        ${BLENDER_ROOT}/lib
)

find_path(HIREDIS_INCLUDE_DIRS
    NAMES
        Hiredis/device/device.h
    PATHS
        /usr/include
        /usr/local/include
        ${BLENDER_ROOT}/src
)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Hiredis
    REQUIRED_VARS
        HIREDIS_LIBRARIES
        HIREDIS_INCLUDE_DIRS
)

mark_as_advanced(HIREDIS_LIBRARIES HIREDIS_INCLUDE_DIRS)