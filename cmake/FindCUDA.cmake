# cmake/FindCUDA.cmake - CORRECTED
find_path(CUDA_TOOLKIT_ROOT_DIR 
    NAMES bin/nvcc  # Search in bin subdirectory!
    PATHS /usr/local/cuda-12.9 /usr/local/cuda
)