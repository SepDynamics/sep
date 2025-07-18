# FindCycles.cmake

# Find Cycles library and headers
find_library(CYCLES_LIBRARIES
    NAMES
        cycles
        cycles_standalone
    PATHS
        /usr/lib
        /usr/lib64
        /usr/local/lib
        /usr/local/lib64
        ${BLENDER_ROOT}/lib
)

find_path(CYCLES_INCLUDE_DIRS
    NAMES
        cycles/device/device.h
    PATHS
        /usr/include
        /usr/local/include
        ${BLENDER_ROOT}/src
)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Cycles
    REQUIRED_VARS
        CYCLES_LIBRARIES
        CYCLES_INCLUDE_DIRS
)

mark_as_advanced(CYCLES_LIBRARIES CYCLES_INCLUDE_DIRS)