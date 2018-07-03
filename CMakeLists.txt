cmake_minimum_required(VERSION 3.0)
project(lru_cache_project)

# Find source files
file(GLOB_RECURSE sources src/*.cpp)

add_executable(lru_cache ${sources})

target_compile_options(lru_cache PUBLIC -std=c++14 -O2 -march=native)
