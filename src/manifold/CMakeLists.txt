add_library(sep_manifold STATIC manifold_definitions.cpp)

target_include_directories(sep_manifold PUBLIC $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>)
