# SEP Apps - Standalone Applications

# OANDA Trader Application
add_executable(oanda_trader
    oanda_trader/main.cpp
    oanda_trader/oanda_trader_app.cpp
)

target_link_libraries(oanda_trader
    sep_engine
    sep_connectors
    imgui
    glad
    OpenGL::GL
    glfw
    ${CMAKE_DL_LIBS}
)

target_include_directories(oanda_trader PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/third_party/imgui
    ${CMAKE_SOURCE_DIR}/third_party/imgui/backends
    ${CMAKE_SOURCE_DIR}/third_party/gl3w/include
    ${CMAKE_SOURCE_DIR}/src/glad
)

# Set properties
set_target_properties(oanda_trader PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

# Install target
install(TARGETS oanda_trader DESTINATION bin)
