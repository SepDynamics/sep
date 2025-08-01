# - Try to find Torch
#
# Once done this will define
#
#  TORCH_FOUND - system has Torch
#  TORCH_INCLUDE_DIRS - the Torch include directories
#  TORCH_LIBRARIES - The libraries needed to use Torch

# Hardcode paths for container since PyTorch is installed there
# The main API include directory where torch/torch.h can be found
set(TORCH_INCLUDE_DIRS 
    "/usr/local/lib/python3.10/dist-packages/torch/include/torch/csrc/api/include"
    "/usr/local/lib/python3.10/dist-packages/torch/include")

# Set library directory and use find_library to locate them
set(TORCH_LIB_DIR "/usr/local/lib/python3.10/dist-packages/torch/lib")
link_directories(${TORCH_LIB_DIR})

# Use library names instead of absolute paths
set(TORCH_LIBRARIES torch c10 c10_cuda)

message(STATUS "TORCH_INCLUDE_DIRS: ${TORCH_INCLUDE_DIRS}")
message(STATUS "TORCH_LIB_DIR: ${TORCH_LIB_DIR}")
message(STATUS "TORCH_LIBRARIES: ${TORCH_LIBRARIES}")

# Always set found to true since we know PyTorch is installed
set(TORCH_FOUND TRUE)

if(TORCH_FOUND)
  add_library(Torch::torch INTERFACE IMPORTED)
  # Use INTERFACE_SYSTEM_INCLUDE_DIRECTORIES to avoid path checking
  set_property(TARGET Torch::torch PROPERTY INTERFACE_SYSTEM_INCLUDE_DIRECTORIES ${TORCH_INCLUDE_DIRS})
  # Link libraries by name, letting the linker find them in TORCH_LIB_DIR
  set_property(TARGET Torch::torch PROPERTY INTERFACE_LINK_LIBRARIES ${TORCH_LIBRARIES})
  # Add the library directory to the link directories
  set_property(TARGET Torch::torch PROPERTY INTERFACE_LINK_DIRECTORIES ${TORCH_LIB_DIR})
  # Ensure C++17 is used when linking against PyTorch
  set_property(TARGET Torch::torch PROPERTY INTERFACE_COMPILE_FEATURES cxx_std_17)
endif()

mark_as_advanced(TORCH_INCLUDE_DIRS TORCH_LIBRARIES)