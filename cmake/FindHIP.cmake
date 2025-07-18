# FindHIP.cmake
# This module finds the HIP libraries and headers
#
# The following variables are set:
#   HIP_FOUND - True if HIP was found
#   HIP_INCLUDE_DIRS - The HIP include directories
#   HIP_LIBRARIES - The HIP libraries
#   HIP_VERSION - The HIP version

# First check if we have a local HIP repository in extern/hip
if(EXISTS "${CMAKE_SOURCE_DIR}/extern/hip/include/hip/hip_runtime.h")
  set(HIP_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/extern/hip/include")
  message(STATUS "Found HIP in extern/hip")
  
  # Define NVIDIA platform for HIP
  add_compile_definitions(__HIP_PLATFORM_NVIDIA__=1)
  
  # Set HIP_FOUND to TRUE
  set(HIP_FOUND TRUE)
  
  # Set HIP_INCLUDE_DIRS
  set(HIP_INCLUDE_DIRS ${HIP_INCLUDE_DIR})
  
  # Set HIP_LIBRARIES to empty since we're using CUDA directly
  set(HIP_LIBRARIES "")
  
  # Set HIP_VERSION to a placeholder
  set(HIP_VERSION "5.0.0")
else()
  # Try to find HIP in standard locations
  find_path(HIP_INCLUDE_DIR
    NAMES hip/hip_runtime.h
    PATHS
      /opt/rocm/hip/include
      /usr/include
      /usr/local/include
      /opt/rocm/include
      ENV ROCM_PATH
    PATH_SUFFIXES include
  )

  # Find HIP library
  find_library(HIP_LIBRARY
    NAMES hip_hcc hip_device amdhip64
    PATHS
      /opt/rocm/hip/lib
      /usr/lib
      /usr/lib64
      /usr/local/lib
      /usr/local/lib64
      /opt/rocm/lib
      /opt/rocm/lib64
      ENV ROCM_PATH
    PATH_SUFFIXES lib lib64
  )

  # Find HIP runtime library
  find_library(HIP_RUNTIME_LIBRARY
    NAMES hiprt
    PATHS
      /opt/rocm/hip/lib
      /usr/lib
      /usr/lib64
      /usr/local/lib
      /usr/local/lib64
      /opt/rocm/lib
      /opt/rocm/lib64
      ENV ROCM_PATH
    PATH_SUFFIXES lib lib64
  )

  # Set HIP_LIBRARIES
  if(HIP_LIBRARY)
    set(HIP_LIBRARIES ${HIP_LIBRARY})
    if(HIP_RUNTIME_LIBRARY)
      list(APPEND HIP_LIBRARIES ${HIP_RUNTIME_LIBRARY})
    endif()
  endif()

  # Set HIP_INCLUDE_DIRS
  if(HIP_INCLUDE_DIR)
    set(HIP_INCLUDE_DIRS ${HIP_INCLUDE_DIR})
  endif()

  # Try to find HIP version
  if(HIP_INCLUDE_DIR)
    file(READ "${HIP_INCLUDE_DIR}/hip/hip_version.h" HIP_VERSION_CONTENT)
    
    string(REGEX MATCH "#define HIP_VERSION_MAJOR ([0-9]+)" _ ${HIP_VERSION_CONTENT})
    set(HIP_VERSION_MAJOR ${CMAKE_MATCH_1})
    
    string(REGEX MATCH "#define HIP_VERSION_MINOR ([0-9]+)" _ ${HIP_VERSION_CONTENT})
    set(HIP_VERSION_MINOR ${CMAKE_MATCH_1})
    
    string(REGEX MATCH "#define HIP_VERSION_PATCH ([0-9]+)" _ ${HIP_VERSION_CONTENT})
    set(HIP_VERSION_PATCH ${CMAKE_MATCH_1})
    
    set(HIP_VERSION "${HIP_VERSION_MAJOR}.${HIP_VERSION_MINOR}.${HIP_VERSION_PATCH}")
  endif()

  # Handle standard arguments
  include(FindPackageHandleStandardArgs)
  find_package_handle_standard_args(HIP
    REQUIRED_VARS HIP_INCLUDE_DIR
    VERSION_VAR HIP_VERSION
  )
endif()

# Create imported target
if(HIP_FOUND AND NOT TARGET HIP::HIP)
  add_library(HIP::HIP INTERFACE IMPORTED)
  set_target_properties(HIP::HIP PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES "${HIP_INCLUDE_DIRS}"
    INTERFACE_LINK_LIBRARIES "${HIP_LIBRARIES}"
  )
endif()

# Mark as advanced
mark_as_advanced(
  HIP_INCLUDE_DIR
  HIP_LIBRARY
  HIP_RUNTIME_LIBRARY
)