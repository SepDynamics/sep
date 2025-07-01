# SPDX-FileCopyrightText: 2011 Blender Authors
#
# SPDX-License-Identifier: BSD-3-Clause

# - Find PNG library
# Find the native PNG includes and library
# This module defines
#  PNG_INCLUDE_DIRS, where to find png.h, Set when
#                    PNG_INCLUDE_DIR is found.
#  PNG_LIBRARIES, libraries to link against to use PNG.
#  PNG_ROOT_DIR, The base directory to search for PNG.
#                This can also be an environment variable.
#  PNG_FOUND, If false, do not try to use PNG.
#
# also defined, but not for general use are
#  PNG_LIBRARY, where to find the PNG library.

# If `PNG_ROOT_DIR` was defined in the environment, use it.
if(DEFINED PNG_ROOT_DIR)
  # Pass.
elseif(DEFINED ENV{PNG_ROOT_DIR})
  set(PNG_ROOT_DIR $ENV{PNG_ROOT_DIR})
else()
  set(PNG_ROOT_DIR "")
endif()

set(_png_SEARCH_DIRS
  ${PNG_ROOT_DIR}
  /usr
  /usr/local
)

find_path(PNG_INCLUDE_DIR
  NAMES
    png.h
  HINTS
    ${_png_SEARCH_DIRS}
  PATH_SUFFIXES
    include
    include/libpng16
)

find_library(PNG_LIBRARY
  NAMES
    png
    png16
  HINTS
    ${_png_SEARCH_DIRS}
  PATH_SUFFIXES
    lib64 lib
)

# Explicitly set paths for system PNG
if(NOT PNG_INCLUDE_DIR)
  set(PNG_INCLUDE_DIR "/usr/include")
endif()

if(NOT PNG_LIBRARY)
  set(PNG_LIBRARY "/usr/lib64/libpng.so")
endif()

# handle the QUIETLY and REQUIRED arguments and set PNG_FOUND to TRUE if
# all listed variables are TRUE
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(PNG DEFAULT_MSG
  PNG_LIBRARY PNG_INCLUDE_DIR)

if(PNG_FOUND)
  set(PNG_LIBRARIES ${PNG_LIBRARY})
  set(PNG_INCLUDE_DIRS ${PNG_INCLUDE_DIR})
  
  # For compatibility with FindPNG.cmake from CMake
  set(PNG_VERSION_STRING "1.6.44")
  set(PNG_MAJOR_VERSION "1")
  set(PNG_MINOR_VERSION "6")
endif()

mark_as_advanced(
  PNG_INCLUDE_DIR
  PNG_LIBRARY
)

unset(_png_SEARCH_DIRS)