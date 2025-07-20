# FindGMock.cmake
#
# Finds the Google Mock (GMock) library
#
# This will define the following variables:
#   GMock_FOUND          - True if GMock is found
#   GMock_INCLUDE_DIRS   - Include directories for GMock
#   GMock_LIBRARIES      - Libraries to link for GMock
#
# and the following imported targets:
#   GMock::gmock         - The GMock library
#   GMock::gmock_main    - The GMock main library

find_package(PkgConfig QUIET)
if(PkgConfig_FOUND)
    pkg_check_modules(PC_GMOCK QUIET gmock)
endif()

# Find the include directory
find_path(GMock_INCLUDE_DIR
    NAMES gmock/gmock.h
    PATHS
        ${PC_GMOCK_INCLUDE_DIRS}
        /usr/include
        /usr/local/include
        /opt/local/include
        ${CMAKE_INSTALL_PREFIX}/include
    PATH_SUFFIXES
        gmock
        google-mock
        googlemock
)

# Find the library
find_library(GMock_LIBRARY
    NAMES gmock libgmock
    PATHS
        ${PC_GMOCK_LIBRARY_DIRS}
        /usr/lib64
        /usr/lib
        /usr/local/lib
        /opt/local/lib
        ${CMAKE_INSTALL_PREFIX}/lib
        /usr/lib/x86_64-linux-gnu
    PATH_SUFFIXES
        lib
)

# Find the main library (optional)
find_library(GMock_MAIN_LIBRARY
    NAMES gmock_main libgmock_main
    PATHS
        ${PC_GMOCK_LIBRARY_DIRS}
        /usr/lib64
        /usr/lib
        /usr/local/lib
        /opt/local/lib
        ${CMAKE_INSTALL_PREFIX}/lib
        /usr/lib/x86_64-linux-gnu
    PATH_SUFFIXES
        lib
)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(GMock
    FOUND_VAR GMock_FOUND
    REQUIRED_VARS
        GMock_LIBRARY
        GMock_INCLUDE_DIR
    VERSION_VAR PC_GMOCK_VERSION
)

if(GMock_FOUND)
    set(GMock_LIBRARIES ${GMock_LIBRARY})
    set(GMock_INCLUDE_DIRS ${GMock_INCLUDE_DIR})

    # Create imported target for gmock
    if(NOT TARGET GMock::gmock)
        add_library(GMock::gmock UNKNOWN IMPORTED)
        set_target_properties(GMock::gmock PROPERTIES
            IMPORTED_LOCATION "${GMock_LIBRARY}"
            INTERFACE_INCLUDE_DIRECTORIES "${GMock_INCLUDE_DIR}"
        )
    endif()

    # Create imported target for gmock_main (if found)
    if(GMock_MAIN_LIBRARY AND NOT TARGET GMock::gmock_main)
        add_library(GMock::gmock_main UNKNOWN IMPORTED)
        set_target_properties(GMock::gmock_main PROPERTIES
            IMPORTED_LOCATION "${GMock_MAIN_LIBRARY}"
            INTERFACE_INCLUDE_DIRECTORIES "${GMock_INCLUDE_DIR}"
            INTERFACE_LINK_LIBRARIES "GMock::gmock"
        )
    endif()
endif()

mark_as_advanced(
    GMock_INCLUDE_DIR
    GMock_LIBRARY
    GMock_MAIN_LIBRARY
)
