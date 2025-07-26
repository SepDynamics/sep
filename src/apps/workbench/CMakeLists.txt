cmake_minimum_required(VERSION 3.16)

# Gather all workbench sources and demos
file(GLOB_RECURSE WB_SOURCES
    "demos/*.cpp"
    "core/*.cpp"
    "tabs/*.cpp"
    "panels/*.cpp"
    "signal_generator/*.cpp"
    "simple_renderer.cpp"
    "renderer.cpp"
    "config.cpp"
)

add_library(sep_workbench_lib STATIC ${WB_SOURCES})

# Public include directories for consumers  
target_include_directories(sep_workbench_lib PUBLIC
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/engine
    ${CMAKE_SOURCE_DIR}/src/api
    ${CMAKE_SOURCE_DIR}/src/apps/workbench
    ${CMAKE_SOURCE_DIR}/src/apps/common
    ${CMAKE_SOURCE_DIR}/src/connectors
    ${CMAKE_SOURCE_DIR}/src/glad
    ${imgui_SOURCE_DIR}
    ${imgui_SOURCE_DIR}/backends
    ${CMAKE_SOURCE_DIR}/third_party/implot
)

# Link against core components
target_link_libraries(sep_workbench_lib PUBLIC
    sep_engine
    sep_quantum
    sep_memory
    sep_connectors
    imgui
    implot
    glad
    OpenGL::GL
    glfw
    nlohmann_json::nlohmann_json
    fmt::fmt
    TBB::tbb
)

# Create the sep_workbench executable
add_subdirectory(backtester)
add_executable(sep_workbench core/workbench_main.cpp)
target_link_libraries(sep_workbench PRIVATE
    sep_workbench_lib
    sep_backtester
    imgui
    implot
    sep_audio
    sep_quantum
    sep_memory  
    sep_engine
    ${TBB_LIBRARIES}
    ${ALEMBIC_LIBRARIES}
    ${OPENPGL_LIBRARIES}
    ${CURL_LIBRARIES}
    ${HTTP_PARSER_LIBRARIES}
    glfw
    glad
    ${CUDA_LIBRARIES}
    OpenGL::GLX
    OpenGL::OpenGL
)
