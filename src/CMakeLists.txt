# Configure C++ standard
# Configure include paths for all targets
set(SEP_INCLUDE_DIRS
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/extern/pipewire/spa/include
    ${PIPEWIRE_INCLUDE_DIRS}
)

# --- CORRECTED EXECUTABLE TARGET ---
add_executable(sep_engine
    server_main.cpp
)
set_target_properties(sep_engine PROPERTIES CXX_STANDARD 17)
target_include_directories(sep_engine PRIVATE ${SEP_INCLUDE_DIRS})

# Add component subdirectories
add_subdirectory(api)
add_subdirectory(audio)
add_subdirectory(blender)
add_subdirectory(compat)
add_subdirectory(core)
add_subdirectory(memory)
add_subdirectory(quantum)
add_subdirectory(embeddings)

# Link ALL necessary libraries in the correct order
target_link_libraries(sep_engine PRIVATE
    sep_api
    sep_blender
    sep_audio
    sep_quantum
    sep_memory
    sep_embeddings
    sep_compat
    sep_core
    Threads::Threads
    ${CURL_LIBRARIES}
    http_parser::http_parser
    hiredis
    pipewire-0.3
    fftw3f
    cycles_scene
    cycles_device
    cycles_kernel
    cycles_osl
    glog
    TBB::tbb
    pthread
    dl
    rt
    m
    z
)

if(TARGET CUDA::cudart)
    target_link_libraries(sep_engine PRIVATE CUDA::cudart)
endif()


