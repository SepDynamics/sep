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
)

find_path(HIREDIS_INCLUDE_DIRS
    NAMES
        hiredis/hiredis.h
    PATHS
        /usr/include
        /usr/local/include
)

# Create imported target
if(NOT TARGET hiredis::hiredis)
    add_library(hiredis::hiredis UNKNOWN IMPORTED)
    set_target_properties(hiredis::hiredis PROPERTIES
        IMPORTED_LOCATION "${HIREDIS_LIBRARIES}"
        INTERFACE_INCLUDE_DIRECTORIES "${HIREDIS_INCLUDE_DIRS}"
    )
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(hiredis
    REQUIRED_VARS
        HIREDIS_LIBRARIES
        HIREDIS_INCLUDE_DIRS
)

mark_as_advanced(HIREDIS_LIBRARIES HIREDIS_INCLUDE_DIRS)