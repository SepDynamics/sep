# CMakeLists.txt for the fixed Crow headers

# Add this directory to the include path
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# Make sure our fixed headers are included before the third-party ones
include_directories(BEFORE ${CMAKE_CURRENT_SOURCE_DIR})

# Add a custom target to make this directory visible in IDEs
add_custom_target(crow_fixed_headers SOURCES
    app.h
    asio_isolation.h
    common.h
    crow_isolation.h
    http_request.h
    http_response.h
    logging.h
    socket_adaptors.h
)

# Add a message to indicate that our fixed headers are being used
message(STATUS "Using fixed Crow headers from ${CMAKE_CURRENT_SOURCE_DIR}")