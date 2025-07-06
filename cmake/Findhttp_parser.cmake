# Try to locate http_parser library

find_path(http_parser_INCLUDE_DIR http_parser.h)
find_library(http_parser_LIBRARY NAMES http_parser)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(http_parser DEFAULT_MSG
    http_parser_LIBRARY http_parser_INCLUDE_DIR)

if(http_parser_FOUND)
    set(http_parser_LIBRARIES ${http_parser_LIBRARY})
    set(http_parser_INCLUDE_DIRS ${http_parser_INCLUDE_DIR})
    if(NOT TARGET http_parser::http_parser)
        add_library(http_parser::http_parser UNKNOWN IMPORTED)
        set_target_properties(http_parser::http_parser PROPERTIES
            IMPORTED_LOCATION "${http_parser_LIBRARY}"
            INTERFACE_INCLUDE_DIRECTORIES "${http_parser_INCLUDE_DIR}"
        )
    endif()
endif()
