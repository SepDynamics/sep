# SPDX-FileCopyrightText: 2011 Blender Authors
#
# SPDX-License-Identifier: BSD-3-Clause

# - Find JPEG library
# Find the native JPEG includes and library
# This module defines
#  JPEG_INCLUDE_DIRS, where to find jpeglib.h, Set when
#                     JPEG_INCLUDE_DIR is found.
#  JPEG_LIBRARIES, libraries to link against to use JPEG.
#  JPEG_ROOT_DIR, The base directory to search for JPEG.
#                 This can also be an environment variable.
#  JPEG_FOUND, If false, do not try to use JPEG.
#
# also defined, but not for general use are
#  JPEG_LIBRARY, where to find the JPEG library.

# If `JPEG_ROOT_DIR` was defined in the environment, use it.
if(DEFINED JPEG_ROOT_DIR)
  # Pass.
elseif(DEFINED ENV{JPEG_ROOT_DIR})
  set(JPEG_ROOT_DIR $ENV{JPEG_ROOT_DIR})
else()
  set(JPEG_ROOT_DIR "")
endif()

set(_jpeg_SEARCH_DIRS
  ${JPEG_ROOT_DIR}
  /usr
  /usr/local
)

find_path(JPEG_INCLUDE_DIR
  NAMES
    jpeglib.h
  HINTS
    ${_jpeg_SEARCH_DIRS}
  PATH_SUFFIXES
    include
)

find_library(JPEG_LIBRARY
  NAMES
    jpeg
  HINTS
    ${_jpeg_SEARCH_DIRS}
  PATH_SUFFIXES
    lib64 lib
)

# Explicitly set paths for system JPEG
if(NOT JPEG_INCLUDE_DIR)
  set(JPEG_INCLUDE_DIR "/usr/include")
endif()

if(NOT JPEG_LIBRARY)
  set(JPEG_LIBRARY "/usr/lib64/libjpeg.so")
endif()

# handle the QUIETLY and REQUIRED arguments and set JPEG_FOUND to TRUE if
# all listed variables are TRUE
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(JPEG DEFAULT_MSG
  JPEG_LIBRARY JPEG_INCLUDE_DIR)

if(JPEG_FOUND)
  set(JPEG_LIBRARIES ${JPEG_LIBRARY})
  set(JPEG_INCLUDE_DIRS ${JPEG_INCLUDE_DIR})
endif()

mark_as_advanced(
  JPEG_INCLUDE_DIR
  JPEG_LIBRARY
)

unset(_jpeg_SEARCH_DIRS)