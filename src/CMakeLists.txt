# Configure C++ standard
# Configure include paths for all targets
set(SEP_INCLUDE_DIRS
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/extern/pipewire/spa/include
    ${PIPEWIRE_INCLUDE_DIRS}
)

# Main server executable
add_executable(sep_server server_main.cpp)
target_include_directories(sep_server PRIVATE ${SEP_INCLUDE_DIRS})

# Add component subdirectories
add_subdirectory(api)
add_subdirectory(audio)
add_subdirectory(blender)
add_subdirectory(compat)
add_subdirectory(core)
add_subdirectory(memory)
add_subdirectory(quantum)
add_subdirectory(embeddings)

# Link dependencies
target_link_libraries(sep_server PRIVATE
    sep_api
    sep_audio
    sep_compat
    sep_core
    sep_memory
    sep_quantum
    sep_blender
    ${PIPEWIRE_LIBRARIES}
    fmt::fmt
)

target_link_libraries(sep_server PRIVATE
    CUDA::cudart
    CUDA::cuda_driver
)
