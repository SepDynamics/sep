# FindGLEW.cmake
# --------------
# Finds the GLEW library and creates both the standard variables
# and a GLEW::GLEW imported target for modern CMake usage.
#
# IMPORTED Targets
# ----------------
# This module defines the following IMPORTED targets:
#  GLEW::GLEW     - The GLEW library if found
#
# Result Variables
# ---------------
#  GLEW_FOUND          - System has GLEW
#  GLEW_INCLUDE_DIRS   - GLEW include directories
#  GLEW_LIBRARIES      - Libraries needed to use GLEW

# First try to use pkg-config to find GLEW
find_package(PkgConfig QUIET)
if(PKG_CONFIG_FOUND)
  pkg_check_modules(PC_GLEW QUIET glew)
endif()

# Look for the header
find_path(GLEW_INCLUDE_DIR
  NAMES GL/glew.h
  PATHS ${PC_GLEW_INCLUDE_DIRS}
  PATH_SUFFIXES GLEW
)

# Look for the library
find_library(GLEW_LIBRARY
  NAMES GLEW glew glew32 glew32s
  PATHS ${PC_GLEW_LIBRARY_DIRS}
)

# Handle the QUIETLY and REQUIRED arguments and set GLEW_FOUND
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(GLEW
  REQUIRED_VARS GLEW_LIBRARY GLEW_INCLUDE_DIR
)

# Set the output variables
if(GLEW_FOUND)
  set(GLEW_LIBRARIES ${GLEW_LIBRARY})
  set(GLEW_INCLUDE_DIRS ${GLEW_INCLUDE_DIR})
  
  # Create imported target GLEW::GLEW
  if(NOT TARGET GLEW::GLEW)
    add_library(GLEW::GLEW UNKNOWN IMPORTED)
    set_target_properties(GLEW::GLEW PROPERTIES
      IMPORTED_LOCATION "${GLEW_LIBRARY}"
      IMPORTED_LOCATION_RELEASE "${GLEW_LIBRARY}"
      IMPORTED_LOCATION_DEBUG "${GLEW_LIBRARY}"
      INTERFACE_INCLUDE_DIRECTORIES "${GLEW_INCLUDE_DIR}"
    )
  endif()
  
  # Also create GLEW::glew for compatibility with pkg_search_module
  if(NOT TARGET GLEW::glew)
    add_library(GLEW::glew UNKNOWN IMPORTED)
    set_target_properties(GLEW::glew PROPERTIES
      IMPORTED_LOCATION "${GLEW_LIBRARY}"
      IMPORTED_LOCATION_RELEASE "${GLEW_LIBRARY}"
      IMPORTED_LOCATION_DEBUG "${GLEW_LIBRARY}"
      INTERFACE_INCLUDE_DIRECTORIES "${GLEW_INCLUDE_DIR}"
    )
  endif()
  
  message(STATUS "Found GLEW: ${GLEW_LIBRARY}")
endif()

# Don't show these variables in the GUI
mark_as_advanced(GLEW_INCLUDE_DIR GLEW_LIBRARY)

# Don't show these variables in the GUI
mark_as_advanced(GLEW_INCLUDE_DIR GLEW_LIBRARY)