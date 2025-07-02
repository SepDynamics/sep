add_library(cuew_stub STATIC cuew_stub.c)
target_include_directories(cuew_stub PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
set_target_properties(cuew_stub PROPERTIES POSITION_INDEPENDENT_CODE ON)
