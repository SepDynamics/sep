# Configure include paths for all targets
set(SEP_INCLUDE_DIRS
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/extern/pipewire/spa/include
    ${PIPEWIRE_INCLUDE_DIRS}
)

# Component subdirectories
add_subdirectory(core)
add_subdirectory(compat)
add_subdirectory(embeddings)
add_subdirectory(memory)
add_subdirectory(quantum)
add_subdirectory(api)
if(SEP_WITH_AUDIO)
    add_subdirectory(audio)
endif()
if(SEP_WITH_CYCLES)
    add_subdirectory(blender)
endif()

# Main executable used for development testing
add_executable(sep_engine
    main.cpp
)
set_target_properties(sep_engine PROPERTIES CXX_STANDARD 17)

target_include_directories(sep_engine PRIVATE ${SEP_INCLUDE_DIRS})

target_link_libraries(sep_engine PRIVATE
    sep_api
    Threads::Threads
    ${CMAKE_DL_LIBS}
)
if(SEP_WITH_CYCLES)
    target_link_libraries(sep_engine PRIVATE sep_blender)
endif()
if(SEP_WITH_AUDIO)
    target_link_libraries(sep_engine PRIVATE sep_audio)
endif()
if(TARGET CUDA::cudart)
    target_link_libraries(sep_engine PRIVATE CUDA::cudart)
endif()
