# Gather all workbench sources and demos
file(GLOB_RECURSE WB_SOURCES
    "api_main.cpp"
)
# Add component subdirectories - only core components needed for quant processing
add_subdirectory(glad)
add_subdirectory(engine)
add_subdirectory(memory)
add_subdirectory(quantum)
add_subdirectory(connectors)
add_subdirectory(audio)
add_subdirectory(workbench)

# Create the sep_workbench executable
add_executable(sep_workbench workbench/core/workbench_main.cpp)
target_link_libraries(sep_workbench PRIVATE
    sep_workbench_lib
    imgui
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



