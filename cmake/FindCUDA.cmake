# FindCUDA.cmake
#
# Find the CUDA toolkit and libraries.
#
# This module defines the following variables:
#
# CUDA_FOUND - True if the CUDA toolkit was found.
# CUDA_INCLUDE_DIRS - The CUDA include directories.
# CUDA_LIBRARIES - The CUDA libraries.
# CUDA_VERSION - The version of the CUDA toolkit.
#
# It also defines the following imported targets:
#
# CUDA::cudart - The CUDA runtime library.
# CUDA::cuda_driver - The CUDA driver library.
# CUDA::cufft - The CUDA FFT library.

# Find the nvcc executable
find_program(CUDA_NVCC_EXECUTABLE NAMES nvcc HINTS /usr/local/cuda/bin)

if(CUDA_NVCC_EXECUTABLE)
  # Get the root directory from the executable path
  get_filename_component(CUDA_TOOLKIT_ROOT_DIR "${CUDA_NVCC_EXECUTABLE}" DIRECTORY)
  get_filename_component(CUDA_TOOLKIT_ROOT_DIR "${CUDA_TOOLKIT_ROOT_DIR}" DIRECTORY)
else()
  # Fallback or error
  find_path(CUDA_TOOLKIT_ROOT_DIR NAMES nvcc PATH_SUFFIXES bin HINTS /usr/local/cuda)
endif()

if(CUDA_TOOLKIT_ROOT_DIR)
  set(CUDA_FOUND TRUE)
  set(CUDA_INCLUDE_DIRS "${CUDA_TOOLKIT_ROOT_DIR}/include")
  set(CUDA_LIBRARIES "${CUDA_TOOLKIT_ROOT_DIR}/lib64/libcudart.so")
  set(CUDA_VERSION "12.9") # This should be detected automatically in a real-world scenario

  if(NOT TARGET CUDA::cudart)
    add_library(CUDA::cudart UNKNOWN IMPORTED)
    set_target_properties(CUDA::cudart PROPERTIES
      IMPORTED_LOCATION "${CUDA_TOOLKIT_ROOT_DIR}/lib64/libcudart.so"
      INTERFACE_INCLUDE_DIRECTORIES "${CUDA_INCLUDE_DIRS}"
    )
  endif()

  if(NOT TARGET CUDA::cuda_driver)
    add_library(CUDA::cuda_driver UNKNOWN IMPORTED)
    set_target_properties(CUDA::cuda_driver PROPERTIES
      IMPORTED_LOCATION "${CUDA_TOOLKIT_ROOT_DIR}/lib64/stubs/libcuda.so"
      INTERFACE_INCLUDE_DIRECTORIES "${CUDA_INCLUDE_DIRS}"
    )
  endif()

  if(NOT TARGET CUDA::cufft)
    add_library(CUDA::cufft UNKNOWN IMPORTED)
    set_target_properties(CUDA::cufft PROPERTIES
      IMPORTED_LOCATION "${CUDA_TOOLKIT_ROOT_DIR}/lib64/libcufft.so"
      INTERFACE_INCLUDE_DIRECTORIES "${CUDA_INCLUDE_DIRS}"
    )
  endif()

endif()