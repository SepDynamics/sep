# CMake config file for CUDA
set(CUDA_TOOLKIT_ROOT_DIR "/usr/local/cuda")
set(CUDA_INCLUDE_DIRS "${CUDA_TOOLKIT_ROOT_DIR}/include")
set(CUDA_LIBRARIES "${CUDA_TOOLKIT_ROOT_DIR}/lib64/libcudart.so")