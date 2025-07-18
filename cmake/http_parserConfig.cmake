# Find http_parser library
#
# This module defines:
#  HTTP_PARSER_FOUND - Whether http_parser was found
#  HTTP_PARSER_INCLUDE_DIRS - The http_parser include directories
#  HTTP_PARSER_LIBRARIES - The http_parser libraries
#  HTTP_PARSER_VERSION - The http_parser version string

include(FindPackageHandleStandardArgs)

# Try to find the header
find_path(HTTP_PARSER_INCLUDE_DIR
    NAMES http_parser.h
    PATHS
        /usr/include
        /usr/local/include
        /opt/local/include
)

# Try to find the library
find_library(HTTP_PARSER_LIBRARY
    NAMES http_parser
    PATHS
        /usr/lib
        /usr/lib64
        /usr/local/lib
        /usr/local/lib64
        /opt/local/lib
)

# Try to find version from pkg-config
find_package(PkgConfig QUIET)
if(PKG_CONFIG_FOUND)
    pkg_check_modules(PC_HTTP_PARSER QUIET http_parser)
    if(PC_HTTP_PARSER_VERSION)
        set(HTTP_PARSER_VERSION ${PC_HTTP_PARSER_VERSION})
    endif()
endif()

# Handle the QUIETLY and REQUIRED arguments
find_package_handle_standard_args(http_parser
    REQUIRED_VARS
        HTTP_PARSER_LIBRARY
        HTTP_PARSER_INCLUDE_DIR
    VERSION_VAR HTTP_PARSER_VERSION
)

if(HTTP_PARSER_FOUND)
    set(HTTP_PARSER_LIBRARIES ${HTTP_PARSER_LIBRARY})
    set(HTTP_PARSER_INCLUDE_DIRS ${HTTP_PARSER_INCLUDE_DIR})
    
    if(NOT TARGET http_parser::http_parser)
        add_library(http_parser::http_parser UNKNOWN IMPORTED)
        set_target_properties(http_parser::http_parser PROPERTIES
            IMPORTED_LOCATION "${HTTP_PARSER_LIBRARY}"
            INTERFACE_INCLUDE_DIRECTORIES "${HTTP_PARSER_INCLUDE_DIR}"
        )
    endif()
endif()

mark_as_advanced(
    HTTP_PARSER_INCLUDE_DIR
    HTTP_PARSER_LIBRARY
)