
include_directories(${CMAKE_CURRENT_SOURCE_DIR}) 

# Configure include paths for all targets
set(SEP_INCLUDE_DIRS
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/extern/pipewire/spa/src
    ${PIPEWIRE_INCLUDE_DIRS}
)

# Add component subdirectories
add_subdirectory(core)
add_subdirectory(compat)
add_subdirectory(embeddings)
add_subdirectory(memory)
add_subdirectory(manifold)
add_subdirectory(quantum)
add_subdirectory(utils)
add_subdirectory(api)
# Audio is now a required component
add_subdirectory(audio)
# Cycles/Blender is now a required component
add_subdirectory(blender)

# add_executable(sep_engine
#     server_main.cpp
# )
# set_target_properties(sep_engine PROPERTIES CXX_STANDARD 17)
#
# target_include_directories(sep_engine PRIVATE ${SEP_INCLUDE_DIRS})
#
# target_link_libraries(sep_engine PRIVATE
#     sep_api
#     sep_quantum
#     sep_memory
#     sep_utils
#     sep_core
#     sep_compat
#     Threads::Threads
#     ${CMAKE_DL_LIBS}
# )
# if(TARGET CUDA::cudart)
#     target_link_libraries(sep_engine PRIVATE CUDA::cudart)
# endif()

# Add the workbench executable if enabled
if(SEP_BUILD_WORKBENCH)
    add_subdirectory(workbench)

    add_executable(sep_workbench
        workbench_main.cpp
    )
    set_target_properties(sep_workbench PROPERTIES CXX_STANDARD 17)
    
    target_include_directories(sep_workbench PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/src
        ${CMAKE_SOURCE_DIR}/third_party/imgui
        ${CMAKE_SOURCE_DIR}/third_party/imgui/backends
        ${GLEW_INCLUDE_DIRS}
    )

    # Link all libraries in the correct order.
    # Static libraries first, then the shared libraries they need.
    target_link_libraries(sep_workbench PRIVATE
        # --- SEP Static Libraries ---
        sep_workbench_lib
        sep_api
        sep_quantum
        sep_memory
        sep_core
        sep_audio
        sep_embeddings
        imgui

        # --- Blender/Cycles and its Dependencies ---
        # Link the shared library we built
        sep_blender

        # Transitive dependencies required by sep_blender
        cycles_device
        cycles_kernel
        cycles_scene
        cycles_util
        ${OSL_LIBRARIES}
        ${TBB_LIBRARIES}
        ${OpenImageIO_LIBRARIES}
        ${Alembic_LIBRARIES}
        ${OpenPGL_LIBRARIES}

        # --- System & Other Dependencies ---
        sep_compat # Must be linked as a shared lib if it provides CUDA symbols
        ${CURL_LIBRARIES}
        -lhiredis
        http_parser::http_parser
        ${GLEW_LIBRARIES}
        glfw
        OpenGL::GL
        ${CMAKE_DL_LIBS}
        Threads::Threads
        ${PIPEWIRE_LIBRARIES}
        ${FFTW_LIBRARIES}
    )
endif()
