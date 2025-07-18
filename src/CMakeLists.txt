
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
)


# Configure include paths for all targets
set(SEP_INCLUDE_DIRS
    ${CMAKE_SOURCE_DIR}/src
)

# Add minimal build configuration
if(SEP_MINIMAL)
    add_definitions(-DSEP_MINIMAL=1)
endif()

# Add component subdirectories - only core components needed for quant processing
add_subdirectory(core)
add_subdirectory(compat)
add_subdirectory(embeddings)
add_subdirectory(memory)
add_subdirectory(gpu)
add_subdirectory(quantum)
add_subdirectory(api)



add_executable(sep_engine
    api_main.cpp
)
set_target_properties(sep_engine PROPERTIES CXX_STANDARD 17)


target_include_directories(sep_engine PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/src
)


target_link_libraries(sep_engine PRIVATE
    sep_api
    sep_quantum
    sep_memory
    sep_embeddings
    sep_core
    sep_compat
    sep_gpu
    Threads::Threads
    CUDA::cudart
    CUDA::cuda_driver
    ${CURL_LIBRARIES}
    http_parser::http_parser
)
