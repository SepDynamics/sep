# Find http_parser library
# This module defines:
#  HTTP_PARSER_FOUND - if http_parser was found
#  HTTP_PARSER_INCLUDE_DIRS - the http_parser include directories
#  HTTP_PARSER_LIBRARIES - the http_parser libraries

find_path(HTTP_PARSER_INCLUDE_DIR
  NAMES http_parser.h
  PATHS /usr/include /usr/local/include
)

find_library(HTTP_PARSER_LIBRARY
  NAMES http_parser
  PATHS /usr/lib /usr/lib64 /usr/local/lib /usr/local/lib64
)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(HttpParser
  REQUIRED_VARS HTTP_PARSER_LIBRARY HTTP_PARSER_INCLUDE_DIR
)

if(HTTP_PARSER_FOUND)
  set(HTTP_PARSER_LIBRARIES ${HTTP_PARSER_LIBRARY})
  set(HTTP_PARSER_INCLUDE_DIRS ${HTTP_PARSER_INCLUDE_DIR})
endif()

mark_as_advanced(HTTP_PARSER_INCLUDE_DIR HTTP_PARSER_LIBRARY)