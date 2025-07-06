# Configure C++ standard
# Configure include paths for all targets
set(SEP_INCLUDE_DIRS
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/extern/pipewire/spa/include
    ${PIPEWIRE_INCLUDE_DIRS}
)

# Main executable for minimal CLI test
add_executable(sep_engine_cli main.cpp)
target_include_directories(sep_engine_cli PRIVATE ${SEP_INCLUDE_DIRS})

# Add component subdirectories
add_subdirectory(api)
add_subdirectory(audio)
add_subdirectory(blender)
add_subdirectory(compat)
add_subdirectory(core)
add_subdirectory(memory)
add_subdirectory(quantum)
add_subdirectory(embeddings)

# Link minimal dependencies
target_link_libraries(sep_engine_cli PRIVATE
    sep_core
    sep_quantum
)
