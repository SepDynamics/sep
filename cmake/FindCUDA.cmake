# - Find CUDA
# This module finds an installation of the CUDA toolkit.
#
# It sets the following variables:
#  CUDA_FOUND              - Set to true if CUDA is found.
#  CUDA_INCLUDE_DIRS       - Include directories for CUDA.
#  CUDA_LIBRARIES          - Libraries to link against for CUDA.
#  CUDA_HOST_COMPILER      - The host compiler used by nvcc.

find_path(CUDA_TOOLKIT_ROOT_DIR
    NAMES bin/nvcc  # Search in bin subdirectory!
    PATHS /usr/local/cuda-12.9 /usr/local/cuda
)

if(CUDA_TOOLKIT_ROOT_DIR)
  set(CUDA_FOUND TRUE)
  set(CUDA_INCLUDE_DIRS ${CUDA_TOOLKIT_ROOT_DIR}/include)
  set(CUDA_LIBRARIES ${CUDA_TOOLKIT_ROOT_DIR}/lib64/libcudart.so)
  # Host compiler is set in cuda-toolchain.cmake or via environment variable
endif()