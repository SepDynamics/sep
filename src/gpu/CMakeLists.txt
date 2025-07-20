add_library(sep_gpu_interface INTERFACE)
target_include_directories(sep_gpu_interface INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)

install(DIRECTORY ${CMAKE_SOURCE_DIR}/src/gpu/
        DESTINATION src/gpu
        FILES_MATCHING
        PATTERN "*.h"
        PATTERN "*.hpp")


if(SEP_ENABLE_CUDA)
    add_library(sep_gpu_cuda STATIC
        cuda_api_unified.cpp
    )
    set_target_properties(sep_gpu_cuda PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
    )
    target_link_libraries(sep_gpu_cuda
        PRIVATE
            CUDA::cudart
        PUBLIC
            sep_gpu_interface
    )
    # Create both aliases for compatibility
    add_library(sep::gpu ALIAS sep_gpu_cuda)
    add_library(sep_gpu ALIAS sep_gpu_cuda)
    
    # Install the libraries
    install(TARGETS sep_gpu_cuda sep_gpu_interface
        EXPORT sep-targets
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    )
endif()

