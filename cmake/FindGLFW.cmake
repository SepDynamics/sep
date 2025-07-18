# FindGLFW.cmake
# - Try to find GLFW
# Once done this will define
#  GLFW_FOUND - System has GLFW
#  GLFW_INCLUDE_DIRS - The GLFW include directories
#  GLFW_LIBRARIES - The libraries needed to use GLFW

find_path(GLFW_INCLUDE_DIR
  NAMES GLFW/glfw3.h
  PATH_SUFFIXES include
  PATHS
  /usr/include
  /usr/local/include
  /opt/local/include
  /sw/include
)

find_library(GLFW_LIBRARY
  NAMES glfw glfw3
  PATH_SUFFIXES lib64 lib
  PATHS
  /usr/lib
  /usr/local/lib
  /opt/local/lib
  /sw/lib
)

include(FindPackageHandleStandardArgs)
# Handle the QUIETLY and REQUIRED arguments and set GLFW_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(GLFW DEFAULT_MSG
  GLFW_LIBRARY GLFW_INCLUDE_DIR)

mark_as_advanced(GLFW_INCLUDE_DIR GLFW_LIBRARY)

if(GLFW_FOUND)
  set(GLFW_LIBRARIES ${GLFW_LIBRARY})
  set(GLFW_INCLUDE_DIRS ${GLFW_INCLUDE_DIR})
endif()