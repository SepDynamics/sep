
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
#    ${PROJECT_SOURCE_DIR}/third_party/imgui
)

#add_library(glad STATIC ${PROJECT_SOURCE_DIR}/src/glad/glad.c)

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
# Completely removed for minimal quant build:
# - audio: not needed for data processing
# - blender: visualization only
# - workbench: UI/demo framework



add_executable(sep_engine
    api_main.cpp
)
set_target_properties(sep_engine PROPERTIES CXX_STANDARD 17)
#target_sources(sep_engine PRIVATE
#    /sep/third_party/imgui/backends/imgui_impl_opengl3.cpp
#    /sep/third_party/imgui/backends/imgui_impl_glfw.cpp
#    /sep/third_party/imgui/imgui_tables.cpp
#    /sep/third_party/imgui/imgui_widgets.cpp
#    /sep/third_party/imgui/imgui.cpp
#    /sep/third_party/imgui/imgui_demo.cpp
#    /sep/third_party/imgui/imgui_draw.cpp
#)

target_include_directories(sep_engine PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/src
)

# Define OpenGL loader to use GLAD instead of GLEW
#target_compile_definitions(sep_engine PRIVATE
#    IMGUI_IMPL_OPENGL_LOADER_GLAD
#)

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


# Add the workbench executable if enabled
if(SEP_BUILD_WORKBENCH)
    add_subdirectory(workbench)

    add_executable(sep_workbench
        workbench/core/workbench_main.cpp
    )
    set_target_properties(sep_workbench PROPERTIES CXX_STANDARD 17)
    
    # Also use GLAD for the main executable
    target_compile_definitions(sep_workbench PRIVATE
        IMGUI_IMPL_OPENGL_LOADER_GLAD
    )
    
    target_include_directories(sep_workbench PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/src
        ${CMAKE_SOURCE_DIR}/third_party/imgui
        ${CMAKE_SOURCE_DIR}/third_party/imgui/backends
        ${PROJECT_SOURCE_DIR}/include/glad
    )

    target_link_libraries(sep_workbench PRIVATE
        # --- Workbench & Application Logic ---
        sep_workbench_lib
        imgui

        # --- SEP Static Libraries (High-level to Low-level) ---
        sep_api
        sep_blender
        sep_audio
        sep_quantum
        sep_memory
        sep_embeddings
        sep_core
        sep_compat

        # --- Cycles and its Dependencies ---
        cycles_device
        cycles_kernel
        cycles_scene
        cycles_util
        ${OSL_LIBRARIES}
        ${TBB_LIBRARIES}
        ${OpenImageIO_LIBRARIES}
        ${Alembic_LIBRARIES}
        ${OpenPGL_LIBRARIES}

        # --- System & Core External Dependencies ---
        ${CURL_LIBRARIES}
        hiredis
        http_parser::http_parser
        glfw
        glad
        OpenGL::GL
        Threads::Threads
        ${PIPEWIRE_LIBRARIES}
        ${FFTW_LIBRARIES}
        CUDA::cudart
        CUDA::cuda_driver
        ${CMAKE_DL_LIBS}
    )
endif()