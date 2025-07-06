cmake_minimum_required(VERSION 3.16)

# Add the workbench executable with its source files
# Build the full-featured server/visualizer
add_executable(sep_workbench
    main.cpp
    config.cpp
    demo_manager.cpp
    demos/genesis_pattern.cpp
    # other demo files
)

set_target_properties(sep_workbench PROPERTIES CXX_STANDARD 17)

# Include paths for engine headers and Cycles
target_include_directories(sep_workbench PRIVATE
    ${PROJECT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/extern/cycles/src
)

# Link against engine libraries and Cycles dependencies
# This mirrors the old sep_server target
target_link_libraries(sep_workbench PRIVATE
    sep_api
    sep_blender
    sep_audio
    sep_embeddings
    sep_quantum
    sep_memory
    sep_compat
    sep_core
    cycles_session
    cycles_scene
    cycles_device
    cycles_kernel_osl
    cycles_subd
    cycles_util
    extern_sky
    extern_cuew
    extern_hipew
    OpenImageIO
    ${OPENVDB_LIBRARIES}
    ${OPENSUBDIV_LIBRARIES}
    openvdb
    bf_intern_opensubdiv
    ${OSL_LIBRARIES}
    ${TBB_LIBRARIES}
    ${CURL_LIBRARIES}
    http_parser::http_parser
    fmt::fmt
    spdlog::spdlog
    hiredis
    pipewire-0.3
    ${EMBREE_LIBRARIES}
    Imath
    ${OpenPGL_LIBRARIES}
    ${OpenColorIO_LIBRARIES}
    ${ALEMBIC_LIBRARIES}
    ${CMAKE_DL_LIBS}
    ${Threads_LIBRARIES}
    ${CUDAToolkit_LIBRARIES}
    pthread
    dl
    rt
    m
    z
)
