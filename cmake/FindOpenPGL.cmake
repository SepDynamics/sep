# FindOpenPGL.cmake
# Find the OpenPGL library and its headers.
#
# This module defines:
#  OpenPGL_FOUND - True if OpenPGL is found
#  OpenPGL_INCLUDE_DIRS - The OpenPGL include directory
#  OpenPGL_LIBRARIES - The libraries needed to use OpenPGL
#  OpenPGL::openpgl - Imported target for the OpenPGL library
#
# This module will look for OpenPGL in standard system locations and can be
# influenced by setting the OPENPGL_ROOT environment variable.

# Try to find OpenPGL using pkg-config first
find_package(PkgConfig QUIET)
if(PKG_CONFIG_FOUND)
  pkg_check_modules(PC_OPENPGL QUIET openpgl) # Assuming pkg-config might provide an openpgl module
endif()

# Find the OpenPGL include directory
find_path(OPENPGL_INCLUDE_DIR
  NAMES openpgl/openpgl.h
  PATHS
    ${PC_OPENPGL_INCLUDE_DIRS}
    $ENV{OPENPGL_ROOT}/include
    $ENV{OPENPGL_DIR}/include
    $ENV{OPENPGL_PATH}/include
    /usr/include
    /usr/local/include
    /opt/local/include
    /opt/openpgl/include
    ${CMAKE_SOURCE_DIR}/extern/openpgl/include
    ${CMAKE_SOURCE_DIR}/third_party/openpgl/include
  DOC "OpenPGL include directory"
)

# Find the OpenPGL library
find_library(OPENPGL_LIBRARY
  NAMES openpgl
  PATHS
    ${PC_OPENPGL_LIBDIR}
    $ENV{OPENPGL_ROOT}/lib
    $ENV{OPENPGL_DIR}/lib
    $ENV{OPENPGL_PATH}/lib
    /usr/lib
    /usr/lib64
    /usr/local/lib
    /usr/local/lib64
    /opt/local/lib
    /opt/openpgl/lib
    ${CMAKE_SOURCE_DIR}/extern/openpgl/lib
    ${CMAKE_SOURCE_DIR}/third_party/openpgl/lib
  DOC "OpenPGL library"
)

# Set standard CMake find_package variables
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(OpenPGL
  REQUIRED_VARS OPENPGL_LIBRARY OPENPGL_INCLUDE_DIR
)

if(OpenPGL_FOUND)
  set(OpenPGL_LIBRARIES ${OPENPGL_LIBRARY})
  set(OpenPGL_INCLUDE_DIRS ${OPENPGL_INCLUDE_DIR})
  
  # Create imported target
  if(NOT TARGET OpenPGL::openpgl)
    add_library(OpenPGL::openpgl UNKNOWN IMPORTED)
    set_target_properties(OpenPGL::openpgl PROPERTIES
      IMPORTED_LOCATION "${OPENPGL_LIBRARY}"
      INTERFACE_INCLUDE_DIRECTORIES "${OPENPGL_INCLUDE_DIR}"
    )
  endif()
  message(STATUS "Found OpenPGL: ${OpenPGL_LIBRARIES}")
  message(STATUS "OpenPGL include dirs: ${OpenPGL_INCLUDE_DIRS}")
  
  # Try to use the CMake config file directly if it exists
  if(EXISTS "/usr/lib64/cmake/openpgl-0.5.0/openpglConfig.cmake")
    message(STATUS "Found OpenPGL CMake config file, using it directly")
    include("/usr/lib64/cmake/openpgl-0.5.0/openpglConfig.cmake")
    
    # Even when using the config file, ensure we have the library path set
    if(NOT OPENPGL_LIBRARY AND TARGET openpgl::openpgl)
      get_target_property(OPENPGL_LIBRARY openpgl::openpgl LOCATION)
      message(STATUS "Using OpenPGL library from target: ${OPENPGL_LIBRARY}")
    endif()
  endif()
  
  # Additional search for the library in standard locations
  if(NOT OPENPGL_LIBRARY)
    find_library(OPENPGL_LIBRARY_EXTRA
      NAMES openpgl
      PATHS
        /usr/lib
        /usr/lib64
        /usr/local/lib
        /usr/local/lib64
      NO_DEFAULT_PATH
    )
    
    if(OPENPGL_LIBRARY_EXTRA)
      set(OPENPGL_LIBRARY ${OPENPGL_LIBRARY_EXTRA})
      message(STATUS "Found OpenPGL library in standard location: ${OPENPGL_LIBRARY}")
    endif()
  endif()
else()
  message(STATUS "OpenPGL not found. OpenPGL-related functionality in Blender module may be disabled or cause errors.")
  set(OpenPGL_LIBRARIES "")
  set(OpenPGL_INCLUDE_DIRS "")
endif()

mark_as_advanced(OPENPGL_INCLUDE_DIR OPENPGL_LIBRARY)