add_library(sep_gpu_interface INTERFACE)
target_include_directories(sep_gpu_interface INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)

if(SEP_ENABLE_CUDA)
    add_library(sep_gpu_cuda STATIC
        cuda_api_unified.cpp
    )
    target_link_libraries(sep_gpu_cuda
        PRIVATE
            CUDA::cudart
        PUBLIC
            sep_gpu_interface
    )
    add_library(sep::gpu ALIAS sep_gpu_cuda)
endif()
