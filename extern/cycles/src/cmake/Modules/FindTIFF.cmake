# SPDX-FileCopyrightText: 2011 Blender Authors
#
# SPDX-License-Identifier: BSD-3-Clause

# - Find TIFF library
# Find the native TIFF includes and library
# This module defines
#  TIFF_INCLUDE_DIRS, where to find tiff.h, Set when
#                     TIFF_INCLUDE_DIR is found.
#  TIFF_LIBRARIES, libraries to link against to use TIFF.
#  TIFF_ROOT_DIR, The base directory to search for TIFF.
#                 This can also be an environment variable.
#  TIFF_FOUND, If false, do not try to use TIFF.
#
# also defined, but not for general use are
#  TIFF_LIBRARY, where to find the TIFF library.

# If `TIFF_ROOT_DIR` was defined in the environment, use it.
if(DEFINED TIFF_ROOT_DIR)
  # Pass.
elseif(DEFINED ENV{TIFF_ROOT_DIR})
  set(TIFF_ROOT_DIR $ENV{TIFF_ROOT_DIR})
else()
  set(TIFF_ROOT_DIR "")
endif()

set(_tiff_SEARCH_DIRS
  ${TIFF_ROOT_DIR}
  /usr
  /usr/local
)

find_path(TIFF_INCLUDE_DIR
  NAMES
    tiff.h
  HINTS
    ${_tiff_SEARCH_DIRS}
  PATH_SUFFIXES
    include
)

find_library(TIFF_LIBRARY
  NAMES
    tiff
  HINTS
    ${_tiff_SEARCH_DIRS}
  PATH_SUFFIXES
    lib64 lib
)

# Explicitly set paths for system TIFF
if(NOT TIFF_INCLUDE_DIR)
  set(TIFF_INCLUDE_DIR "/usr/include")
endif()

if(NOT TIFF_LIBRARY)
  set(TIFF_LIBRARY "/usr/lib64/libtiff.so")
endif()

# handle the QUIETLY and REQUIRED arguments and set TIFF_FOUND to TRUE if
# all listed variables are TRUE
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(TIFF DEFAULT_MSG
  TIFF_LIBRARY TIFF_INCLUDE_DIR)

if(TIFF_FOUND)
  set(TIFF_LIBRARIES ${TIFF_LIBRARY})
  set(TIFF_INCLUDE_DIRS ${TIFF_INCLUDE_DIR})
endif()

mark_as_advanced(
  TIFF_INCLUDE_DIR
  TIFF_LIBRARY
)

unset(_tiff_SEARCH_DIRS)