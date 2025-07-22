add_executable(oanda_trader
    main.cpp
    oanda_trader_app.cpp
)

target_include_directories(oanda_trader PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/third_party/imgui
)

target_link_libraries(oanda_trader PUBLIC
    sep_engine
    sep_connectors
    imgui
    glad
    OpenGL::GL
    glfw
    Threads::Threads
    ${CMAKE_DL_LIBS}
)

set_target_properties(oanda_trader PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

install(TARGETS oanda_trader DESTINATION bin)