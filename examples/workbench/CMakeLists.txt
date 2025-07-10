cmake_minimum_required(VERSION 3.15)

# Locate required graphics packages
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glfw3 REQUIRED)
find_package(PkgConfig REQUIRED)

# Find GTK3 for UI components
pkg_check_modules(GTK3 REQUIRED gtk+-3.0)
include_directories(${GTK3_INCLUDE_DIRS})
link_directories(${GTK3_LIBRARY_DIRS})
add_definitions(${GTK3_CFLAGS_OTHER})
message(STATUS "GTK3 libraries: ${GTK3_LIBRARIES}")
message(STATUS "GTK3 include dirs: ${GTK3_INCLUDE_DIRS}")
message(STATUS "GTK3 library dirs: ${GTK3_LIBRARY_DIRS}")

# Find libdecor for window decoration
pkg_check_modules(LIBDECOR REQUIRED libdecor-0)
include_directories(${LIBDECOR_INCLUDE_DIRS})
link_directories(${LIBDECOR_LIBRARY_DIRS})
add_definitions(${LIBDECOR_CFLAGS_OTHER})

add_executable(sep_workbench
    main.cpp
    renderer.cpp
)

# Gather demo sources
file(GLOB DEMO_SOURCES demos/*.cpp)
target_sources(sep_workbench PRIVATE ${DEMO_SOURCES})

# Include Dear ImGui
set(IMGUI_DIR ${CMAKE_SOURCE_DIR}/third_party/imgui)
file(GLOB IMGUI_SOURCES
    ${IMGUI_DIR}/*.cpp
    ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
    ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
)

target_sources(sep_workbench PRIVATE ${IMGUI_SOURCES})

# Include directories
target_include_directories(sep_workbench PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${IMGUI_DIR}
    ${IMGUI_DIR}/backends
)

# Enable GLM experimental features
target_compile_definitions(sep_workbench PRIVATE GLM_ENABLE_EXPERIMENTAL SEP_WORKBENCH_DEMO)

# Find required libraries for Blender integration
find_package(Alembic REQUIRED)
find_package(OpenPGL REQUIRED)

# Link libraries
target_link_libraries(sep_workbench PRIVATE
    # Graphics and UI
    GLEW::GLEW
    glfw
    OpenGL::GL
    ${GTK3_LIBRARIES}

    # Engine libraries
    sep_api
    sep_core
    sep_compat
    sep_quantum
    sep_memory
    sep_blender
    sep_audio
    
    # Explicitly link Alembic and OpenPGL
    $<$<TARGET_EXISTS:Alembic::Abc>:Alembic::Abc>
    $<$<TARGET_EXISTS:Alembic::AbcGeom>:Alembic::AbcGeom>
    $<$<TARGET_EXISTS:Alembic::AbcCoreAbstract>:Alembic::AbcCoreAbstract>
    $<$<TARGET_EXISTS:Alembic::AbcCoreFactory>:Alembic::AbcCoreFactory>
    $<$<NOT:$<TARGET_EXISTS:Alembic::Abc>>:${Alembic_LIBRARIES}>
    
    $<$<TARGET_EXISTS:OpenPGL::openpgl>:OpenPGL::openpgl>
    $<$<NOT:$<TARGET_EXISTS:OpenPGL::openpgl>>:${OpenPGL_LIBRARIES}>
    
    # Direct linking as fallback
    -lAlembic
    -lopenpgl
    
    # Blender sky model library
    ${CMAKE_SOURCE_DIR}/lib/libbf_intern_sky.a
)

# Add rpath for the shared libraries
set_target_properties(sep_workbench PROPERTIES
    INSTALL_RPATH "/usr/lib64:/sep/lib"
    BUILD_WITH_INSTALL_RPATH TRUE
)
