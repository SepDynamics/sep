cmake_minimum_required(VERSION 3.10)
project(sep_memory_verifier LANGUAGES CXX)

find_package(hiredis REQUIRED)

add_executable(sep_memory_verifier sep_memory_verifier.cpp)

target_link_libraries(sep_memory_verifier PRIVATE sep_memory hiredis benchmark::benchmark)