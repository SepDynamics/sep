# SEP Demo Application
cmake_minimum_required(VERSION 3.16)

# Find required packages
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
# GLAD is built as part of the project

# Demo application executable
add_executable(sep_demo
    main.cpp
    sep_demo_app.cpp
)

# Include directories
target_include_directories(sep_demo PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/third_party/imgui
    ${CMAKE_SOURCE_DIR}/third_party/imgui/backends
)

# Link libraries - order matters for static libraries
# Libraries that use symbols must come before libraries that provide them
target_link_libraries(sep_demo PRIVATE
    # Libraries with dependencies first
    sep_memory
    sep_quantum
    # Then the library that provides symbols they need
    sep_engine
    # External libraries
    imgui
    OpenGL::GL
    glfw
    glad
    fmt::fmt
    TBB::tbb
    Threads::Threads
    ${CMAKE_DL_LIBS}
)

# Add CUDA libraries if enabled
if(SEP_USE_CUDA)
    target_link_libraries(sep_demo PRIVATE
        cudadevrt
        cudart_static
        rt
        pthread
        dl
    )
endif()

# Set C++ standard
set_target_properties(sep_demo PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

# Platform-specific settings
if(APPLE)
    target_link_libraries(sep_demo PRIVATE
        "-framework Cocoa"
        "-framework IOKit"
        "-framework CoreVideo"
    )
endif()

# Install target
install(TARGETS sep_demo
    RUNTIME DESTINATION bin
)